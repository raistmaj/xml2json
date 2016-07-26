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
#ifndef UMISON_UMIXMLTYPELIST_H
#define UMISON_UMIXMLTYPELIST_H

#include "umixmltype.h"
#include "umisontypetocpp.h"
#include <vector>
#include <memory>

namespace umi {
  /**
   * List type on the xml template, reference array json object
   * */
  class umixmltypelist : public umixmltype {
  public:
    /**
     * Constructor
     * */
    umixmltypelist() : umixmltype() {
    }

    /**
     * Destructor
     * */
    virtual ~umixmltypelist() {
    }

    /**
     * It is a list
     * */
    virtual bool isList() const final {
      return true;
    }

    /**
     * Returns the type we want to use in the header, it will
     * append the new line
     * */
    virtual std::string header_type(const std::string &additiona_text, bool append_new_line = true) {
      umi::type_to_cpp tcpp;
      std::string retval = "std::vector<";
      std::string cpp_type = tcpp.get_type(m_refclass);
      if (cpp_type.empty()) {
        retval += "__internal__umison";
        if (!additiona_text.empty()) {
          retval += additiona_text;
        }
        retval += "::";
        retval += m_refclass;
      } else {
        retval += cpp_type;
      }
      retval += "> ";
      retval += umi::umixmltype::attribute_prepocess(m_name);
      if (append_new_line) {
        retval += ";\n";
      } else {
        retval += ";";
      }
      return retval;
    }

    /**
     * Returns the optional header we want to use
     * */
    virtual std::string optional_name_type(const std::string &, bool append_new_line = true) {
      std::string retval;
      if (m_optional && !m_optional_name.empty()) {
        retval = "bool ";
        retval += m_optional_name;
        if (append_new_line) {
          retval += ";\n";
        } else {
          retval += ";";
        }
      }
      return retval;
    }

    /**
     * Returns the initialization element in the constructor
     * */
    virtual std::string constructor_initializer() {
      std::string retval;
      retval += umi::umixmltype::attribute_prepocess(m_name);
      retval += "()";
      if (m_optional && !m_optional_name.empty()) {
        retval += ", ";
        retval += m_optional_name;
        retval += "(false)";
      }
      return retval;
    }

    /**
     * Returns the get method
     * */
    virtual std::string getter_method(const std::string &additiona_text, bool append_new_line,
                                      const std::string &indentation,
                                      int basic_indentation) {
      umi::type_to_cpp tcpp;
      std::string retval;
      std::string cpp_type = tcpp.get_type(m_refclass);
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "inline const std::vector<";
      if (cpp_type.empty()) {
        retval += "__internal__umison";
        if (!additiona_text.empty()) {
          retval += additiona_text;
        }
        retval += "::";
        retval += m_refclass;
      } else {
        retval += cpp_type;
      }
      retval += ">& get_";
      retval += m_name;
      retval += "() const {\n";
      for (int i = 0; i < basic_indentation + 1; ++i) {
        retval += indentation;
      }
      retval += "return ";
      retval += umi::umixmltype::attribute_prepocess(m_name);
      retval += ";\n";
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "}";
      if (append_new_line) {
        retval += "\n";
      }
      return retval;
    }

    /**
     * Returns the set method
     * */
    virtual std::string setter_method(const std::string &additiona_text, bool append_new_line,
                                      const std::string &indentation,
                                      int basic_indentation) {
      umi::type_to_cpp tcpp;
      std::string retval;
      std::string cpp_type = tcpp.get_type(m_refclass);
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "inline void set_";
      retval += m_name;
      retval += "(const std::vector<";
      if (cpp_type.empty()) {
        retval += "__internal__umison";
        if (!additiona_text.empty()) {
          retval += additiona_text;
        }
        retval += "::";
        retval += m_refclass;
      } else {
        retval += cpp_type;
      }
      retval += "> &val) {\n";
      for (int i = 0; i < basic_indentation + 1; ++i) {
        retval += indentation;
      }
      retval += umi::umixmltype::attribute_prepocess(m_name);
      retval += " = val;\n";
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "}";
      if (append_new_line) {
        retval += "\n";
      }
      return retval;
    }

    /**
     * Returns the mutable method
     * */
    virtual std::string mutable_method(const std::string &additiona_text, bool append_new_line,
                                       const std::string &indentation,
                                       int basic_indentation) {
      umi::type_to_cpp tcpp;
      std::string retval;
      std::string cpp_type = tcpp.get_type(m_refclass);
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "inline std::vector<";
      if (cpp_type.empty()) {
        retval += "__internal__umison";
        if (!additiona_text.empty()) {
          retval += additiona_text;
        }
        retval += "::";
        retval += m_refclass;
      } else {
        retval += cpp_type;
      }
      retval += ">& mutable_";
      retval += m_name;
      retval += "() {\n";
      for (int i = 0; i < basic_indentation + 1; ++i) {
        retval += indentation;
      }
      retval += "return ";
      retval += umi::umixmltype::attribute_prepocess(m_name);
      retval += ";\n";
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "}";
      if (append_new_line) {
        retval += "\n";
      }
      return retval;
    }
    /**
     * Returns the write method into a string
     * */
    std::string write_to_string(const std::string &input_string, bool append_new_line, const std::string &indentation,
                                int basic_indentation, const std::string &aux_array, int size_array,
                                const std::string &out_error) {
      umi::type_to_cpp tcpp;
      std::string cpp_type = tcpp.get_type(m_refclass);
      std::stringstream retval;
      std::string base_indentation;
      std::string base_indentation_1p;
      std::string base_indentation_2p;
      std::string base_indentation_3p;
      std::string quoted_name;
      for (int i = 0; i < basic_indentation; ++i) {
        base_indentation += indentation;
      }
      base_indentation_1p += base_indentation + indentation;
      base_indentation_2p += base_indentation_1p + indentation;
      base_indentation_3p += base_indentation_2p + indentation;
      quoted_name = "\\\"";
      quoted_name += m_name;
      quoted_name += "\\\"";

      retval << base_indentation << "{\n"
             << base_indentation_1p << input_string << " += \"" << quoted_name << ": [\";\n";

      if (cpp_type.empty()) {
        retval << base_indentation_1p << "std::size_t position;\n"
               << base_indentation_1p << "for (position = 0; position < "
               << umi::umixmltype::attribute_prepocess(m_name) << ".size(); ++position) {\n"
               << base_indentation_2p << "if (position > 0) { " << input_string << " += \",\"; }\n"
               << base_indentation_2p << input_string << " += \"{\";\n"
               << base_indentation_2p << input_string << " += \"}\";\n";
        retval << base_indentation_1p << "}\n";
      } else {
        retval << base_indentation_1p << "std::size_t position;\n"
               << base_indentation_1p << "for (position = 0; position < "
                                      << umi::umixmltype::attribute_prepocess(m_name) << ".size(); ++position) {\n"
               << base_indentation_2p << "if (position > 0) { " << input_string << " += \",\"; }\n";
        if (cpp_type == "std::string") {
          retval << base_indentation_2p << input_string << " += \"\\\"\";\n"
                 << base_indentation_2p << input_string << " += " << umi::umixmltype::attribute_prepocess(m_name)
                 << "[position];\n"
                 << base_indentation_2p << input_string << " += \"\\\"\";\n";
        } else {
          if (cpp_type == "long long int") {
            retval << base_indentation_2p << "memset(" << aux_array << ", 0, " << size_array << ");\n"
                   << base_indentation_2p << "int aux_retval = snprintf(" << aux_array
                   << ", sizeof(" << aux_array << ") - 1, \"%\"PRId64\"\", "
                   << umi::umixmltype::attribute_prepocess(m_name) << "[position]);\n"
                   << base_indentation_2p << "if (aux_retval < 0) {\n"
                   << base_indentation_3p << out_error << " << \"Error in line \" << __LINE__;\n"
                   << base_indentation_3p << "return false;\n"
                   << base_indentation_2p << "}\n"
                   << base_indentation_2p << input_string << " += " << aux_array << ";\n";
          } else if (cpp_type == "int") {
            retval << base_indentation_2p << "memset(" << aux_array << ", 0, " << size_array << ");\n"
                   << base_indentation_2p << "int aux_retval = snprintf(" << aux_array
                   << ", sizeof(" << aux_array << ") - 1, \"%d\", "
                   << umi::umixmltype::attribute_prepocess(m_name) << "[position]);\n"
                   << base_indentation_2p << "if (aux_retval < 0) {\n"
                   << base_indentation_3p << out_error << " << \"Error in line \" << __LINE__;\n"
                   << base_indentation_3p << "return false;\n"
                   << base_indentation_2p << "}\n"
                   << base_indentation_2p << input_string << " += " << aux_array << ";\n";
          } else if (cpp_type == "double") {
            retval << base_indentation_2p << "memset(" << aux_array << ", 0, " << size_array << ");\n"
                   << base_indentation_2p << "int aux_retval = snprintf(" << aux_array
                   << ", sizeof(" << aux_array << ") - 1, \"%f\", "
                   << umi::umixmltype::attribute_prepocess(m_name) << "[position]);\n"
                   << base_indentation_2p << "if (aux_retval < 0) {\n"
                   << base_indentation_3p << out_error << " << \"Error in line \" << __LINE__;\n"
                   << base_indentation_3p << "return false;\n"
                   << base_indentation_2p << "}\n"
                   << base_indentation_2p << input_string << " += " << aux_array << ";\n";
          } else if (cpp_type == "bool") {
            retval << base_indentation_2p << "if ("
                      << umi::umixmltype::attribute_prepocess(m_name) << "[position]) {\n"
                   << base_indentation_3p << input_string << " += \"true\";\n"
                   << base_indentation_2p << "} else {\n"
                   << base_indentation_3p << input_string << " += \"false\";\n"
                   << base_indentation_2p << "}\n";
          }
        }
        retval << base_indentation_1p << "}\n";
      }

      retval << base_indentation_1p << input_string << " += \"]\";\n";
      retval << base_indentation << "}\n";

      if (append_new_line) {
        retval << "\n";
      }
      return retval.str();
    }
  };
}

#endif
