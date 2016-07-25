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
#ifndef UMISON_UMIXMLTYPE_H
#define UMISON_UMIXMLTYPE_H

#include <string>
#include <sstream>
#include "umisonforwarddeclaration.h"


namespace umi {
  /**
   * Parent class for the whole xml types supported in the program
   * */
  class umixmltype {
  public:
    static std::string attribute_prepocess(const std::string &name) {
      return name + "_";
    }

  public:
    /**
     * Basic constructor
     * */
    umixmltype() {
    }

    /**
     * Destructor
     * */
    virtual ~umixmltype() {
    }

    /**
     * Will inform if the type is a boolean or not
     * */
    virtual bool isBoolean() const {
      return false;
    }

    /**
     * Will inform if the type is a 64 bits integer
     * */
    virtual bool isInteger() const {
      return false;
    }

    /**
     * Will inform if the type is a 32 bits integer
     * */
    virtual bool isInteger32() const {
      return false;
    }

    /**
     * Will inform if the type is a float
     * */
    virtual bool isFloat() const {
      return false;
    }

    /**
     * Will inform if the type is a String
     * */
    virtual bool isString() const {
      return false;
    }

    /**
     * Will inform if the type is a list
     * */
    virtual bool isList() const {
      return false;
    }

    /**
     * Will inform if the type is a class
     * */
    virtual bool isClass() const {
      return false;
    }

    /**
     * Will inform if the type is a referenced class
     * */
    virtual bool isRefClass() const {
      return false;
    }

    /**
     * Will inform if the type is a map
     * */
    virtual bool isMap() const {
      return false;
    }

    /**
     * Gets Name we want to use in the json reading and the data structure
     * */
    inline const std::string &name() const {
      return m_name;
    }

    /**
     * Sets the name we want to use in the json reading and the data structure
     * */
    inline void name(const std::string &name) {
      m_name = name;
    }

    /**
     * Gets the condition to use in the json reading and the data structure
     * */
    inline const std::string &condition() const {
      return m_condition;
    }

    /**
     * Sets the condition to use in the json reading and the data structure
     * */
    inline void condition(const std::string &cnd) {
      m_condition = cnd;
    }

    /**
     * Gets tje referenced class to be used(useful for arrays)
     * */
    inline const std::string &refclass() const {
      return m_refclass;
    }

    /**
     * Sets the referenced class to be used
     * */
    inline void refclass(const std::string &refclass) {
      m_refclass = refclass;
    }

    /**
     * Gets if this entry is optional to be read
     * */
    inline bool optional() {
      return m_optional;
    }

    /**
     * Sets if the entry is optional to be read
     * */
    inline void optional(bool value) {
      m_optional = value;
    }

    /**
     * Gets the entry optional name
     * */
    inline const std::string &optional_name() const {
      return m_optional_name;
    }

    /**
     * Sets the entry optional name
     * */
    inline void optional_name(const std::string &val) {
      m_optional_name = val;
    }

    /**
     * Returns the type we want to use in the header, it will
     * append the new line
     * */
    virtual std::string header_type(const std::string &, bool) {
      return std::string();
    }

    /**
     * Returns the optional header we want to use
     * */
    virtual std::string optional_name_type(const std::string &, bool) {
      return std::string();
    }

    /**
     * Returns the initialization element in the constructor
     * */
    virtual std::string constructor_initializer() {
      return std::string();
    }

    /**
     * Returns the get method
     * */
    virtual std::string getter_method(const std::string &, bool, const std::string &indentation,
                                      int basic_indentation) {
      (void) indentation;
      (void) basic_indentation;
      return std::string();
    }

    /**
     * Returns the set method
     * */
    virtual std::string setter_method(const std::string &, bool, const std::string &indentation,
                                      int basic_indentation) {
      (void) indentation;
      (void) basic_indentation;
      return std::string();
    }

    /**
     * Returns the mutable method
     * */
    virtual std::string mutable_method(const std::string &, bool, const std::string &indentation,
                                       int basic_indentation) {
      (void) indentation;
      (void) basic_indentation;
      return std::string();
    }

    /**
     * Returns the get method
     * */
    virtual std::string getter_method_optional(const std::string &, bool append_new_line,
                                               const std::string &indentation,
                                               int basic_indentation) {
      std::string retval;
      if (optional()) {
        for (int i = 0; i < basic_indentation; ++i) {
          retval += indentation;
        }
        retval += "inline ";
        retval += "bool get_";
        retval += m_optional_name;
        retval += "() const {\n";
        for (int i = 0; i < basic_indentation + 1; ++i) {
          retval += indentation;
        }
        retval += "return ";
        retval += m_optional_name;
        retval += ";\n";
        for (int i = 0; i < basic_indentation; ++i) {
          retval += indentation;
        }
        retval += "}";
        if (append_new_line) {
          retval += "\n";
        }
      }
      return retval;
    }

    /**
     * Returns the set method
     * */
    virtual std::string setter_method_optional(const std::string &, bool append_new_line,
                                               const std::string &indentation,
                                               int basic_indentation) {
      std::string retval;
      if (optional()) {
        for (int i = 0; i < basic_indentation; ++i) {
          retval += indentation;
        }
        retval += "inline ";
        retval += "void set_";
        retval += m_optional_name;
        retval += "(bool val) {\n";
        for (int i = 0; i < basic_indentation + 1; ++i) {
          retval += indentation;
        }
        retval += m_optional_name;
        retval += " = val;\n";
        for (int i = 0; i < basic_indentation; ++i) {
          retval += indentation;
        }
        retval += "}";
        if (append_new_line) {
          retval += "\n";
        }
      }
      return retval;
    }

    /**
     * Returns the mutable method
     * */
    virtual std::string mutable_method_optional(const std::string &, bool append_new_line,
                                                const std::string &indentation,
                                                int basic_indentation) {
      std::string retval;
      if (optional()) {
        for (int i = 0; i < basic_indentation; ++i) {
          retval += indentation;
        }
        retval += "inline ";
        retval += "bool& mutable_";
        retval += m_optional_name;
        retval += "() {\n";
        for (int i = 0; i < basic_indentation + 1; ++i) {
          retval += indentation;
        }
        retval += "return ";
        retval += m_optional_name;
        retval += ";\n";
        for (int i = 0; i < basic_indentation; ++i) {
          retval += indentation;
        }
        retval += "}";
        if (append_new_line) {
          retval += "\n";
        }
      }
      return retval;
    }

    /**
     * Returns the write method into a string
     * */
    virtual std::string write_to_string(
        const std::string &,
        bool ,
        const std::string &,
        int,
        const std::string &,
        int,
        const std::string &) {
      return std::string();
    }
  protected:

    /**
     * Name of the element in the output structure and json to be read
     * */
    std::string m_name;
    /**
     * Condition to be used in the parsing of this element
     * */
    std::string m_condition;
    /**
     * Referenced class to be used in this element
     * */
    std::string m_refclass;
    /**
     * Is this element optional
     * */
    bool m_optional = false;
    /**
     * Name for the optional element
     * */
    std::string m_optional_name;
  };
}

#endif
