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
#ifndef UMISON_UMIXMLTYPEINTEGER_H
#define UMISON_UMIXMLTYPEINTEGER_H

#include "umixmltype.h"
#include <cstdint>

namespace umi {
  /**
   * Integer type of the xml template
   * */
  class umixmltypeinteger : public umixmltype {
  public:
    /**
     * Constructor
     * */
    umixmltypeinteger() : umixmltype() {
    }

    /**
     * Destructor
     * */
    virtual ~umixmltypeinteger() {
    }

    /**
     * It is an integer
     * */
    virtual bool isInteger() const final {
      return true;
    }

    /**
     * Returns the type we want to use in the header, it will
     * append the new line
     * */
    virtual std::string header_type(const std::string &, bool append_new_line = true) {
      std::string retval = "long long int ";
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
      retval += "(0)";
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
    virtual std::string getter_method(const std::string &, bool append_new_line, const std::string &indentation,
                                      int basic_indentation) {
      std::string retval;
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "inline ";
      retval += "long long int get_";
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
    virtual std::string setter_method(const std::string &, bool append_new_line, const std::string &indentation,
                                      int basic_indentation) {
      std::string retval;
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "inline ";
      retval += "void set_";
      retval += m_name;
      retval += "(long long int val) {\n";
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
    virtual std::string mutable_method(const std::string &, bool append_new_line, const std::string &indentation,
                                       int basic_indentation) {
      std::string retval;
      for (int i = 0; i < basic_indentation; ++i) {
        retval += indentation;
      }
      retval += "inline ";
      retval += "long long int& mutable_";
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
      std::stringstream retval;
      std::string base_indentation;
      std::string base_indentation_1p;
      std::string base_indentation_2p;
      std::string quoted_name;
      for (int i = 0; i < basic_indentation; ++i) {
        base_indentation += indentation;
      }
      base_indentation_1p += base_indentation + indentation;
      base_indentation_2p += base_indentation_1p + indentation;
      quoted_name = "\\\"";
      quoted_name += m_name;
      quoted_name += "\\\"";
      retval << base_indentation << "{\n"
             << base_indentation_1p << "memset(" << aux_array << ", 0, " << size_array << ");\n"
             << base_indentation_1p << "int aux_retval = snprintf(" << aux_array
             << ", sizeof(" << aux_array << ") - 1, \"%\"PRId64\"\", "
             << umi::umixmltype::attribute_prepocess(m_name) << ");\n"
             << base_indentation_1p << "if (aux_retval < 0) {\n"
             << base_indentation_2p << out_error << " << \"Error in line \" << __LINE__;\n"
             << base_indentation_2p << "return false;\n"
             << base_indentation_1p << "}\n"
             << base_indentation_1p << input_string << " += \"" << quoted_name << ": \";\n"
             << base_indentation_1p << input_string << " += " << aux_array << ";\n"
             << base_indentation << "}\n";
      if (append_new_line) {
        retval << "\n";
      }
      return retval.str();
    }
  };
}
#endif
