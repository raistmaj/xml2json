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
#ifndef CUDASON_CUDAXMLTYPE_H
#define CUDASON_CUDAXMLTYPE_H

#include <string>

namespace cuda {
  /**
   * Parent class for the whole xml types supported in the program
   * */
  class cudaxmltype {
  public:
    /**
     * Basic constructor
     * */
    cudaxmltype() {
    }
    /**
     * Destructor
     * */
    virtual ~cudaxmltype() {
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
     * Gets tje referenced class to be used(usefull for arrays)
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
  };
}

#endif //CUDASON_CUDAXMLTYPE_H
