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
#ifndef CUDASON_CUDASONTYPETOCPP_H
#define CUDASON_CUDASONTYPETOCPP_H

#include <unordered_map>

namespace cuda {
  /**
   * Class used to transform from one type to a cpp type only basic types
   * */
  class type_to_cpp {
  public:
    /**
     * Constructor
     * */
    type_to_cpp() { }
    /**
     * Destructor
     * */
    ~type_to_cpp() { }
    /**
     * Get the type we want
     * */
    std::string get_type(const std::string& val) {
      std::string retval;
      auto val_it = m_mapType.find(val);
      if(val_it != m_mapType.end()) {
        retval = val_it->second;
      }
      return retval;
    }
  protected:
    std::unordered_map <std::string, std::string> m_mapType{{"string",  "std::string"},
                                                            {"integer", "long long int"},
                                                            {"int32",   "int"},
                                                            {"float",   "double"},
                                                            {"boolean", "bool"}};
  };
}

#endif //CUDASON_CUDASONTYPETOCPP_H
