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
#ifndef CUDASON_CUDAXML_H
#define CUDASON_CUDAXML_H

#include "cudaxmltypes.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace cuda {
  /**
   * Class used to store our XML tree with its properties
   * and transform it afterwards to json
   * */
  class cudaxml {
  public:
    /**
     * Creates a full xml tree ready to be written
     * on disk based on the content string
     * */
    explicit cudaxml(const std::string &content);
    /**
     * Forbid copy
     * */
    cudaxml(const cudaxml &) = delete;
    /**
     * Clean the resources
     * */
    ~cudaxml();
    /**
     * Forbid assignement
     * */
    cudaxml &operator=(const cudaxml &) = delete;
    /**
     * Gets the class map
     * */
    inline const std::vector<std::pair<std::string, std::shared_ptr<cuda::cudaxmltypeclass>>> &getClassMap() const {
      return m_classMap;
    }
    /**
     * Gets the json array map
     * */
    inline const std::vector<std::shared_ptr<cuda::cudaxmltypeclass>> &getJsonArray() const {
      return m_jsonArray;
    }
  protected:
    /**
     * List of classes we will use in the creation of the json, we use a vector of pairs as we need to keep
     * the dependencies declared on the input template, anyway the intention is to use it as a map
     * */
    std::vector<std::pair<std::string, std::shared_ptr<cuda::cudaxmltypeclass>>> m_classMap;
    /**
     * List of json documents we will output
     * */
    std::vector<std::shared_ptr<cuda::cudaxmltypeclass>> m_jsonArray;
  };
}

#endif //CUDASON_CUDAXML_H
