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
          stream << "\n" << TABS << TABS
          << "// read one input string and fill the data, errors are reported on stderr\n"
          << TABS << TABS << "bool read_data(const std::string &input_text);\n"
          << "\n" << TABS << TABS
          << "// read one input string and fill the data, errors are reported on out_stream\n"
          << TABS << TABS << "bool read_data(const std::string &input_text, std::ostream &out_stream);\n"
          << "\n" << TABS << TABS
          << "// read one input file and fill the data, errors are reported on stderr\n"
          << TABS << TABS << "bool read_data_from_file(const std::string &input_file);\n"
          << "\n" << TABS << TABS
          << "// read one input file and fill the data, errors are reported on out_stream\n"
          << TABS << TABS << "bool read_data_from_file(const std::string &input_file, std::ostream &out_stream);\n"
          << "\n" << TABS << TABS
          << "// writes the json into a string, errors are reported on out_stream"
          << TABS << TABS << "bool write_data_to_string(std::string &output, std::ostream &out_stream) const;"
          << "\n" << TABS << TABS
          << "// writes the json into a string, errors are reported on stderr"
          << TABS << TABS << "bool write_data_to_string(std::string &output) const;";

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

    template<typename stream_out>
    void implement_type_declaration_writer_basic_types(
        stream_out &stream, int indentation_level, const std::string &indentation) {
      std::string indentation_base;
      std::string indentation_base_1p;
      std::string indentation_base_2p;
      for (int i = 0; i < indentation_level; ++i) {
        indentation_base += indentation;
      }
      indentation_base_1p = indentation_base + indentation;
      indentation_base_2p = indentation_base_1p + indentation;
      // int
      stream << indentation_base
             << "template<typename Stream>\n"
             << indentation_base
             << "bool __write_type(const int &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "char aux_array[512] = {};\n"
             << indentation_base_1p
             << "int aux_retval = snprintf(aux_array, sizeof(aux_array) - 1, \"%d\", arr);\n"
             << indentation_base_1p
             << "if (aux_retval < 0) {\n"
             << indentation_base_2p
             << "return false;\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "out_str += aux_array;\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
      // long long int
      stream << indentation_base
             << "template<typename Stream>\n"
             << indentation_base
             << "bool __write_type(const long long int &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "char aux_array[512] = {};\n"
             << indentation_base_1p
             << "int aux_retval = snprintf(aux_array, sizeof(aux_array) - 1, \"%\"PRId64\"\", arr);\n"
             << indentation_base_1p
             << "if (aux_retval < 0) {\n"
             << indentation_base_2p
             << "return false;\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "out_str += aux_array;\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
      // double
      stream << indentation_base
             << "template<typename Stream>\n"
             << indentation_base
             << "bool __write_type(const double &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "char aux_array[512] = {};\n"
             << indentation_base_1p
             << "int aux_retval = snprintf(aux_array, sizeof(aux_array) - 1, \"%f\", arr);\n"
             << indentation_base_1p
             << "if (aux_retval < 0) {\n"
             << indentation_base_2p
             << "return false;\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "out_str += aux_array;\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
      // bool
      stream << indentation_base
             << "template<typename Stream>\n"
             << indentation_base
             << "bool __write_type(const bool &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "if (arr == true) {\n"
             << indentation_base_2p
             << "out_str += \"true\";\n"
             << indentation_base_1p
             << "} else {\n"
             << indentation_base_2p
             << "out_str += \"false\";\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
      // std::string
      stream << indentation_base
             << "template<typename Stream>\n"
             << indentation_base
             << "bool __write_type(const std::string &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "out_str += \"\\\"\";\n"
             << indentation_base_1p
             << "out_str += arr;\n"
             << indentation_base_1p
             << "out_str += \"\\\"\";\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
    }

    template<typename stream_out>
    void forward_type_declaration_writer_custom_types(
        std::shared_ptr<umi::umixml> &ff,
        stream_out &stream,
        int indentation_level,
        const std::string &indentation) {
      std::string indentation_base;
      std::string indentation_base_1p;
      std::string indentation_base_2p;
      std::string additional_string;

      if (!m_additional_string.empty()) {
        additional_string += "::";
        additional_string += m_additional_string;
      }

      for (int i = 0; i < indentation_level; ++i) {
        indentation_base += indentation;
      }
      indentation_base_1p = indentation_base + indentation;
      indentation_base_2p = indentation_base_1p + indentation;

      for (auto &element : ff->getClassMap()) {
        stream << indentation_base
               << "template<typename Stream>\n"
               << indentation_base
               << "bool __write_type(const "
               << "__internal__umison"
               << additional_string
               << "::"
               << element.first
               << " &arr, std::string &out_str, Stream &ss);\n";
      }
    }

    template<typename stream_out>
    void implement_type_declaration_writer_custom_types(
        std::shared_ptr<umi::umixml> &ff,
        stream_out &stream,
        int indentation_level,
        const std::string &indentation) {
      std::string indentation_base;
      std::string indentation_base_1p;
      std::string indentation_base_2p;
      std::string additional_string;

      if (!m_additional_string.empty()) {
        additional_string += "::";
        additional_string += m_additional_string;
      }

      for (int i = 0; i < indentation_level; ++i) {
        indentation_base += indentation;
      }
      indentation_base_1p = indentation_base + indentation;
      indentation_base_2p = indentation_base_1p + indentation;

      for (auto &element : ff->getClassMap()) {
        stream << indentation_base
               << "template<typename Stream>\n"
               << indentation_base
               << "bool __write_type(const "
               << "__internal__umison"
               << additional_string
               << "::"
               << element.first
               << " &arr, std::string &out_str, Stream &ss) {\n";
        stream << indentation_base_1p
               << "out_str += \"{\";\n";

        for (auto &element_child : element.second->getChildren()) {
          stream << indentation_base_1p
                 << "if (!__write_type(\""
                 << element_child->name()
                 << "\", out_str, ss)) { return false; }\n"
                 << indentation_base_1p
                 << "out_str += \":\";\n"
                 << indentation_base_1p
                 << "if (!__write_type("
                 << umi::umixmltype::attribute_prepocess(element_child->name())
                 << ", out_str, ss)) { return false; }\n\n";
        }
        stream << indentation_base_1p
               << "out_str += \"}\";\n";
        stream << indentation_base
               << "}\n\n";
      }

    }

    template<typename stream_out>
    void implement_type_declaration_writer_list(
        stream_out &stream, int indentation_level, const std::string &indentation) {
      std::string indentation_base;
      std::string indentation_base_1p;
      std::string indentation_base_2p;
      for (int i = 0; i < indentation_level; ++i) {
        indentation_base += indentation;
      }
      indentation_base_1p = indentation_base + indentation;
      indentation_base_2p = indentation_base_1p + indentation;

      stream << indentation_base
             << "template<typename Stream>\n"
             << indentation_base
             << "bool __write_type(const std::vector<bool> &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "out_str += \"[\";\n"
             << indentation_base_1p
             << "std::size_t position;\n"
             << indentation_base_1p
             << "for (position = 0; position < arr.size(); ++position) { \n"
             << indentation_base_2p
             << "if (position > 0) { out_str += \",\"; }\n"
             << indentation_base_2p
             << "__write_type(static_cast<bool>(arr[position]), out_str, ss);\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "out_str += \"]\";\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
      stream << indentation_base
             << "template<typename Type, typename Stream>\n"
             << indentation_base
             << "bool __write_type(const std::vector<Type> &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "out_str += \"[\";\n"
             << indentation_base_1p
             << "std::size_t position;\n"
             << indentation_base_1p
             << "for (position = 0; position < arr.size(); ++position) { \n"
             << indentation_base_2p
             << "if (position > 0) { out_str += \",\"; }\n"
             << indentation_base_2p
             << "if (!__write_type(arr[position], out_str, ss)) { return false; }\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "out_str += \"]\";\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
    }

    template<typename stream_out>
    void implement_type_declaration_writer_map(
        stream_out &stream, int indentation_level, const std::string &indentation) {
      std::string indentation_base;
      std::string indentation_base_1p;
      std::string indentation_base_2p;
      for (int i = 0; i < indentation_level; ++i) {
        indentation_base += indentation;
      }
      indentation_base_1p = indentation_base + indentation;
      indentation_base_2p = indentation_base_1p + indentation;
      stream << indentation_base
             << "template<typename Type, typename Stream>\n"
             << indentation_base
             << "bool __write_type(const std::map<std::string, Type> &arr, std::string &out_str, Stream &ss) {\n"
             << indentation_base_1p
             << "out_str += \"{\";\n"
             << indentation_base_1p
             << "std::size_t position;\n"
             << indentation_base_1p
             << "for (std::map<std::string, Type::const_iterator it = arr.begin(), position = 0; it != arr.end(); ++it, ++position) { \n"
             << indentation_base_2p
             << "if (position > 0) { out_str += \",\"; }\n"
             << indentation_base_2p
             << "if (!__write_type(it->first, out_str, ss)) { return false; }\n"
             << indentation_base_2p
             << "out_str += \":\";\n"
             << indentation_base_2p
             << "if (!__write_type(it->second, out_str, ss)) { return false; }\n"
             << indentation_base_1p
             << "}\n"
             << indentation_base_1p
             << "out_str += \"}\";\n"
             << indentation_base_1p
             << "return true;\n"
             << indentation_base
             << "}\n\n";
    }
    template<typename stream_out>
    void forward_type_declaration_writer(const std::string &type, stream_out &stream,
                                         int indentation_level, const std::string &indentation) {
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "template<typename Stream>\n";
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "bool __write_type(const " << type << " &arr, std::string &out_str, Stream &ss);\n";
    }

    template<typename stream_out>
    void forward_list_declaration_writter(stream_out &stream, int indentation_level, const std::string &indentation) {
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "template<typename Stream>\n";
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "bool __write_type(const std::vector<bool> &arr, std::string &out_str, Stream &ss);\n";
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "template<typename Type, typename Stream>\n";
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "bool __write_type(const std::vector<Type> &arr, std::string &out_str, Stream &ss);\n";
    }

    template<typename stream_out>
    void forward_map_declaration_writter(stream_out &stream, int indentation_level, const std::string &indentation) {
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "template<typename Type, typename Stream>\n";
      for (int i = 0; i < indentation_level; ++i) {
        stream << indentation;
      }
      stream << "bool __write_type(const std::map<std::string, Type> &arr, std::string &out_str, Stream &ss);\n";
    }

    template<typename stream_out>
    void print_write_data_common_string(std::shared_ptr<umi::umixml> &ff, stream_out &stream) {
      int actual_ind = 0;
      std::vector<std::string> basic_types { "bool", "int" , "long long int", "double", "std::string" };
      std::string additional_string;
      std::string indentation;
      std::string indentation_1p(indentation + TABS);

      if (!m_additional_string.empty()) {
        additional_string += "::";
        additional_string += m_additional_string;
      }

      stream << "namespace __internal__umison" << additional_string << " {\n\n";
      actual_ind += 1;
      // Create forward declarations from basic types
      stream << indentation_1p << "// Forward declaration of writing functions\n";
      for(auto &ty : basic_types) {
        forward_type_declaration_writer(ty, stream, actual_ind, TABS);
      }
      forward_type_declaration_writer_custom_types(ff, stream, actual_ind, TABS);

      forward_list_declaration_writter(stream, actual_ind, TABS);
      forward_map_declaration_writter(stream, actual_ind, TABS);

      stream << "\n";
      implement_type_declaration_writer_basic_types(stream, actual_ind, TABS);
      implement_type_declaration_writer_custom_types(ff, stream, actual_ind, TABS);
      implement_type_declaration_writer_list(stream, actual_ind, TABS);
      implement_type_declaration_writer_map(stream, actual_ind, TABS);

      actual_ind -= 1;
      // close namespace
      stream << "}\n\n";
    }

    template<typename stream_out>
    void print_write_data_on_string(std::shared_ptr<umi::umixml> &ff, stream_out &stream) {
      int actual_ind = 0;
      std::string additional_string;
      std::string indentation;
      std::string indentation_1p(indentation + TABS);

      if (!m_additional_string.empty()) {
        additional_string += "::";
        additional_string += m_additional_string;
      }

      for (auto &json : ff->getJsonArray()) {
        // bool write_data_to_string(std::string &output) const;
        stream << indentation << "bool umison" << additional_string
               << json->name()
               << "::write_data_to_string(std::string &output) const {\n"
               << indentation_1p << "return write_data_to_string(output, std::cerr);\n"
               << indentation << "}\n\n";
        // bool write_data_to_string(std::string &output, std::ostream &out_stream) const;
        stream << indentation << "bool umison"  << additional_string
               << json->name()
               << "::write_data_to_string(std::string &output, std::ostream &out_stream) const {\n"
               << indentation_1p << "char aux_array[512];\n";
        actual_ind += 1;
        if (json->getChildren().size() == 1) {

        } else {
          stream << indentation_1p << "output += \'{\';\n";

          for (decltype(json->getChildren().size()) position = 0;
               position < json->getChildren().size();
              ++position) {
            auto &child = json->getChildren()[position];
            if (position > 0) {
              stream << indentation_1p << "output << \",\";\n";
            }
            stream << child->write_to_string("output", true, TABS, actual_ind, "aux_array", 512, "out_stream");
          }

          stream << indentation_1p << "output += \'}\';\n";
        }
        actual_ind -= 1;
        stream << indentation_1p << "return true;\n"
               << indentation << "}\n";
      }
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
