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
#ifndef UMISON_UMIXMLTYPEBOOLEAN_H
#define UMISON_UMIXMLTYPEBOOLEAN_H

#include "umixmltype.h"

namespace umi {
  /**
   * xml type to reference a boolean
   * */
  class umixmltypeboolean : public umixmltype {
  public:
    /**
     * Constructor of the boolean
     * */
    umixmltypeboolean() : umixmltype() {
    }

    /**
     * Destructor of the boolean
     * */
    virtual ~umixmltypeboolean() {
    }

    /**
     * Is true for this class
     * */
    virtual bool isBoolean() const final {
      return true;
    }

    /**
     * Returns the type we want to use in the header, it will
     * append the new line
     * */
    virtual std::string header_type(const std::string &, bool append_new_line = true) {
      std::string retval = "bool ";
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
      retval += "(false)";
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
      retval += "bool get_";
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
      retval += "(bool val) {\n";
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
      retval += "bool& mutable_";
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
                                int basic_indentation, const std::string &, int, const std::string &) {
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
      quoted_name += "\\\"";
      quoted_name += m_name;
      quoted_name += "\\\"";

      retval << base_indentation << "{\n"
      << base_indentation_1p << "if (" << umi::umixmltype::attribute_prepocess(m_name) << ") {\n"
      << base_indentation_2p << input_string << " += \"" << quoted_name << ": true\";\n"
      << base_indentation_1p << "else {\n"
      << base_indentation_2p << input_string << " += \"" << quoted_name << ": false\";\n"
      << base_indentation_1p << "}\n"
      << base_indentation << "}\n";
      if (append_new_line) {
        retval << "\n";
      }
      return retval.str();
    }
  };
}

#endif
