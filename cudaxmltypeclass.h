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
#ifndef CUDASON_CUDAXMLTYPECLASS_H
#define CUDASON_CUDAXMLTYPECLASS_H

#include "cudaxmltype.h"
#include <memory>
#include <vector>

namespace cuda {
  /**
   * This class is used to store multiple children of one node
   *
   * Don't get confused as we will never have a possible "class",
   * only refclass on the json, this is used to store the templates
   * of possible referenced classes
   * */
  class cudaxmltypeclass : public cudaxmltype {
  public:
    /**
     * Constructor of the class
     * */
    cudaxmltypeclass() : cudaxmltype() {
    }
    /**
     * Destructor
     * */
    virtual ~cudaxmltypeclass() {
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
    const std::vector<std::shared_ptr<cudaxmltype>> &getChildren() const {
      return m_children;
    }
  protected:
    /**
     * Array of children are included within the class. It is a vector as we
     * want to keep the order
     * */
    std::vector<std::shared_ptr<cudaxmltype>> m_children;
  };
}

#endif //CUDASON_CUDAXMLTYPECLASS_H
