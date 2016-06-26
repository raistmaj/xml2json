/****************************************************************************************
 *	Copyright (c) 2015, Jose Gerardo Palma Duran
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without modification,
 *	are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright notice,
 *	   this list of conditions and the following disclaimer.
 *
 *	2. Redistributions in binary form must reproduce the above copyright notice, this
 *	   list of conditions and the following disclaimer in the documentation and/or
 *	   other materials provided with the distribution.
 *
 *	 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *	 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *	 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *	 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *	 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *	 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *	 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	 POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************************/
#ifndef UMISON_UMISONFILEPRINTER_H
#define UMISON_UMISONFILEPRINTER_H

#include "umixml.h"
#include "umixmltags.h"
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace umi {
  /**
   * Class used by the output engine common to all subengines-
   * It will create the basic .h template
   *
   * The idea behind this is to use always the same .h and the internals
   * engine will write the .cpp following those name conventions.
   *
   * Thanks to this approach we can abstract the output subengine
   * as all must be independent, may produce some problems in the future
   * but as first decision looks correct.
   * */
  template<typename streamer>
  class file_printer {
  public:
    /**
     * Basic constructor, this class uses pure RAII
     * */
    file_printer() { }

    /**
     * Prints the .h file
     *
     * \param ff pointer to the umixml instance we want to write within stream
     * \param stream used to output the json representation
     * \param headers vector of additional headers we want to include
     * \param methods vector of additional methods we want to include
     * */
    void print(std::shared_ptr<umi::umixml> &ff, streamer &stream, const std::vector<std::string> &headers,
               const std::vector<std::string> &methods) {
      auto &localAdditionalString = m_additional_string;
      stream << DISCLAIMER << "\n#pragma once\n"
      << "\n#include <ostream>\n#include <string>\n#include <vector>\n#include <map>\n";
      if (!headers.empty()) {
        std::for_each(
          headers.begin(),
          headers.end(),
          [&stream](const auto &header) {
            stream << "#include <" << header << ">\n";
          });
      }
      stream << "\n";
      if (!ff->getClassMap().empty()) {
        stream << "// Internal namespace declaration\nnamespace __internal__umison"
        << m_additional_string << " {\n\n"
        << TABS << "// Forward declaration\n";
        auto classMap = ff->getClassMap();
        std::for_each(classMap.begin(),
                      classMap.end(),
                      [&stream](const auto &classMapIt) {
                        stream << TABS << "struct " << classMapIt.first << ";\n";
                      });
        stream << "\n";
        std::for_each(
          classMap.begin(),
          classMap.end(),
          [&stream, &localAdditionalString, this](const auto &classMapIt) {
            stream << TABS << "// struct " << classMapIt.first << "\n"
            << TABS << "struct " << classMapIt.first << " {\n";
            this->set_public(stream);
            this->create_constructors(classMapIt.second, stream);
            this->create_destructors(classMapIt.second, stream);
            this->create_properties(classMapIt.second, stream);
            stream << "\n";
            this->set_private(stream);
            stream << TABS << TABS << "// Attributes declaration\n";
            auto classChildren = classMapIt.second->getChildren();
            std::for_each(
              classChildren.begin(),
              classChildren.end(),
              [&stream, &localAdditionalString](const auto &childrenIt) {
                stream << TABS << TABS
                << childrenIt->header_type(localAdditionalString, true);
                if (childrenIt->optional()) {
                  stream << TABS << TABS
                  << childrenIt->optional_name_type(localAdditionalString, true);
                }
              });
            stream << TABS << "};\n\n";
          });
        stream << "}\n\n"; // For the internal namespace
      }

      stream << "// Namespace where we store our final jsons\nnamespace umison {\n\n"
      << TABS << "// Forward declaration\n";
      auto &jsonArray = ff->getJsonArray();
      std::for_each(
        jsonArray.begin(),
        jsonArray.end(),
        [&stream](auto &jsonArrayIt) {
          stream << TABS << "struct " << jsonArrayIt->name() << ";\n";
        });
      stream << "\n";
      std::for_each(
        jsonArray.begin(),
        jsonArray.end(),
        [&stream, &localAdditionalString, &methods, this](auto &jsonArrayIt) {
          stream << TABS << "// struct " << jsonArrayIt->name() << "\n"
          << TABS << "struct " << jsonArrayIt->name() << " {\n";
          this->set_public(stream);
          this->create_constructors(jsonArrayIt, stream);
          this->create_destructors(jsonArrayIt, stream);
          stream << "\n" << TABS << TABS <<
          "// read one input string and fill the data, errors are reported on stderr\n"
          << TABS << TABS << "bool read_data(const std::string& input_text);\n";
          stream << "\n" << TABS << TABS <<
          "// read one input string and fill the data, errors are reported on out_stream\n"
          << TABS << TABS << "bool read_data(const std::string &input_text, std::ostream &out_stream);\n";

          this->create_properties(jsonArrayIt, stream);

          if (!methods.empty()) {
            stream << "\n" << TABS << TABS << "// Additional specific engine methods\n";
            std::for_each(
              methods.begin(),
              methods.end(),
              [&stream](auto &method) {
                stream << TABS << TABS << method << "\n";
              }
            );
          }
          stream << "\n";

          this->set_private(stream);
          stream << TABS << TABS << "// Attributes declaration\n";
          auto classChildren = jsonArrayIt->getChildren();
          std::for_each(
            classChildren.begin(),
            classChildren.end(),
            [&stream, &localAdditionalString](auto &childrenIt) {
              stream << TABS << TABS
              << childrenIt->header_type(localAdditionalString, true);
              if (childrenIt->optional()) {
                stream << TABS << TABS
                << childrenIt->optional_name_type(localAdditionalString, true);
              }
            }
          );

          stream << TABS << "};\n\n";
        }
      );
      stream << "}\n"; // For the umison namespace
    }

    /**
     * String to append in namespace and class
     * */
    std::string m_additional_string;
  protected:
    void create_constructors(const
                             std::shared_ptr<umi::umixmltypeclass> &elem, streamer
                             &stream) {
      stream << TABS << TABS << "// Default constructor\n"
      << TABS << TABS << elem->name() << "()";
      auto classChildren = elem->getChildren();
      if (!classChildren.empty()) {
        stream << " : ";
        for (std::size_t i = 0; i < classChildren.size(); ++i) {
          std::string childText(classChildren[i]->constructor_initializer());
          if (!childText.empty()) {
            stream << childText;
          }
          if (i + 1 < classChildren.size()) {
            stream << ",\n" << TABS << TABS << TABS;
          }
        }
        stream << " {}\n";
      } else {
        stream << " {}\n";
      }
    }

    void create_destructors(const std::shared_ptr<umi::umixmltypeclass> &elem, streamer &stream) {
      stream << "\n" << TABS << TABS << "// Destructor\n"
      << TABS << TABS << "virtual ~" << elem->name() << "() {}\n";
    }

    void set_public(streamer &stream) {
      stream << TABS << "public:\n";
    }

    void set_private(streamer &stream) {
      stream << TABS << "private:\n";
    }

    void create_properties(const std::shared_ptr<umi::umixmltypeclass> &elem, streamer &stream) {
      auto &localAdditionalString = m_additional_string;
      auto classChildren = elem->getChildren();
      stream << "\n" << TABS << TABS << "// Properties declaration\n";
      std::for_each(
        classChildren.begin(),
        classChildren.end(),
        [&stream, &localAdditionalString](const auto &child) {
          stream << child->getter_method(localAdditionalString, true, TABS, 2);
          stream << child->setter_method(localAdditionalString, true, TABS, 2);
          stream << child->mutable_method(localAdditionalString, true, TABS, 2);
          stream << child->getter_method_optional(localAdditionalString, true, TABS, 2);
          stream << child->setter_method_optional(localAdditionalString, true, TABS, 2);
          stream << child->mutable_method_optional(localAdditionalString, true, TABS, 2);
          stream << "\n";
        });

    }
  };
}

#endif
