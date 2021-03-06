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
#ifndef UMISON_UMIXMLTYPECLASS_H
#define UMISON_UMIXMLTYPECLASS_H

#include "umixmltype.h"
#include <memory>
#include <vector>

namespace umi {
  /**
   * This class is used to store multiple children of one node
   *
   * Don't get confused as we will never have a possible "class",
   * only refclass on the json, this is used to store the templates
   * of possible referenced classes
   * */
  class umixmltypeclass : public umixmltype {
  public:
    /**
     * Constructor of the class
     * */
    umixmltypeclass() : umixmltype() {
    }

    /**
     * Destructor
     * */
    virtual ~umixmltypeclass() {
    }

    /**
     * It is a class
     * */
    virtual bool isClass() const final {
      return true;
    }

    /**
     * Adds one children to the list
     * */
    template<typename T>
    inline void addChildren(T &&val) {
      m_children.push_back(val);
    }

    /**
     * Gets the list of children we have in the class
     * */
    const std::vector<std::shared_ptr<umixmltype>> &getChildren() const {
      return m_children;
    }

    /**
     * Returns the type we want to use in the header, it will
     * append the new line
     * */
    virtual std::string header_type(const std::string &, bool append_new_line = true) {
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
    virtual std::string getter_method(const std::string &, bool append_new_line, const std::string &indentation,
                                      int basic_indentation) {
      (void) append_new_line;
      (void) indentation;
      (void) basic_indentation;
      return std::string();
    }

    /**
     * Returns the write method into a string
     * */
    std::string write_to_string(const std::string &, bool , const std::string &, int ) {
      return std::string();
    }
  protected:
    /**
     * Array of children are included within the class. It is a vector as we
     * want to keep the order
     * */
    std::vector<std::shared_ptr<umixmltype>> m_children;
  };
}

#endif
