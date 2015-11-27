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
#ifndef CUDASON_CUDAXMLTYPEREFCLASS_H
#define CUDASON_CUDAXMLTYPEREFCLASS_H

#include "cudaxmltype.h"

namespace cuda {
  /**
   * Referenced class xml type, is an object on the json
   * */
  class cudaxmltyperefclass : public cuda::cudaxmltype {
  public:
    /**
     * Constructor
     * */
    cudaxmltyperefclass() : cudaxmltype() {
    }
    /**
     * Destructor
     * */
    virtual ~cudaxmltyperefclass() {
    }
    /**
     * Is a referenced class
     * */
    virtual bool isRefClass() const final {
      return true;
    }
    /**
     * Returns the type we want to use in the header, it will
     * append the new line
     * */
    virtual std::string header_type(const std::string& additiona_text, bool append_new_line = true) {
      cuda::type_to_cpp tcpp;
      std::string retval;
      std::string cpp_type = tcpp.get_type(m_refclass); // Suport for reference of basic types
      if(cpp_type.empty()) {
        retval += "__internal__cudason";
        if(!additiona_text.empty()) {
          retval += additiona_text;
        }
        retval += "::";
        retval += m_refclass;
      } else {
        retval += cpp_type;
      }
      retval += " ";
      retval += m_name;
      if(append_new_line) {
        retval += ";\n";
      }
      return retval;
    }
  };
}

#endif //CUDASON_CUDAXMLTYPEREFCLASS_H
