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
    virtual std::string header_type(const std::string& additiona_text, bool append_new_line = true) {
      std::string retval =  "long long int ";
      retval += m_name;
      if(append_new_line) {
        retval += ";\n";
      } else {
        retval += ";";
      }
      return retval;
    }
    /**
     * Returns the optional header we want to use
     * */
    virtual std::string optional_name_type(const std::string& additional_text, bool append_new_line = true) {
      std::string retval;
      if(m_optional && !m_optional_name.empty()) {
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
      retval += m_name;
      retval += "(0)";
      if (m_optional && !m_optional_name.empty()) {
        retval += ", ";
        retval += m_optional_name;
        retval += "(false)";
      }
      return retval;
    }
  };
}
#endif
