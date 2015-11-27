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
#ifndef UMISON_UMISONOUTPUTENGINE_H
#define UMISON_UMISONOUTPUTENGINE_H

#include "umisonfileprinter.h"
#include "umixml.h"
#include "umixmltags.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>

namespace umi {
  // Forward declaration of the umixml
  class umixml;

  /**
     * Checks if the type is an internal type
     * */
  static bool is_internal(const std::string &val) {
    if (val == UMI_XMLTAGS_INTEGER) {
      return true;
    }
    if (val == UMI_XMLTAGS_BOOLEAN) {
      return true;
    }
    if (val == UMI_XMLTAGS_FLOAT) {
      return true;
    }
    if (val == UMI_XMLTAGS_STRING) {
      return true;
    }
    if (val == UMI_XMLTAGS_INTEGER32) {
      return true;
    }
    return false;
  }

  static std::string internal_to_rapidjson(const std::string &val) {
    if (val == UMI_XMLTAGS_INTEGER) {
      return "Int64";
    }
    if (val == UMI_XMLTAGS_BOOLEAN) {
      return "Bool";
    }
    if (val == UMI_XMLTAGS_FLOAT) {
      return "Double";
    }
    if (val == UMI_XMLTAGS_STRING) {
      return "String";
    }
    if (val == UMI_XMLTAGS_INTEGER32) {
      return "Int";
    }
    return "Object";
  }

  /**
   * Output engine parent class used as interface for the rest
   * of classed used for this purpose
   *
   * The class accepts to possible templates as both output streams (the .h and .cpp) can differ
   * in its internal implementation, for example an unix socket and a fstream
   * */
  template<typename T1, typename T2>
  class output_engine {
  public:
    /**
     * Destructor
     * */
    virtual ~output_engine() {
    }
    /**
     * Writes one xml into json. The additional options must be set in
     * other place like the constructor etc.
     * */
    bool write(std::shared_ptr<umi::umixml> &ff) {
      file_printer printer;
      printer.print(ff, m_h_streamer);
      return this->internal_write(ff);
    };
    /**
     * Sets the additional string we want to append to our namespace
     *
     * \param value with the string we will append
     * */
    void additional_string(const std::string &value) {
      m_additional_string = value;
    }
    /**
     * Sets the .h filename we want to print the file
     *
     * \param file with the string we will use in the include .cpp
     * */
    void h_filename(const std::string &value) {
      m_h_name = value;
    }

  protected:
    /**
     * Basic constrcutor, it is used to store the references of the input streams
     *
     * \param _H_streamer used to output the .h file
     * \param _CPP_streamer used to output the .cpp file
     * */
    output_engine(T1 &_H_streamer, T2 &_CPP_streamer) : m_h_streamer(_H_streamer), m_cpp_streamer(_CPP_streamer) {
    }
    /**
     * Implement on subclasses to include logic for the cpp
     *
     * \param ff is pointer with the xml template already parsed
     * */
    virtual bool internal_write(std::shared_ptr<umi::umixml> &ff) {
      return true;
    }

    /**
     * First output stream used for the .h file
     * */
    T1 &m_h_streamer;
    /**
     * Second output stream used for the .cpp file
     * */
    T2 &m_cpp_streamer;
    /**
     * Name used for the .h file if we write it to a file
     * */
    std::string m_h_name;
    /**
     * Additional string used in the process
     * */
    std::string m_additional_string;
  };
}

#endif
