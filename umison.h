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
#ifndef UMISON_UMISON_H
#define UMISON_UMISON_H

#include <fstream>
#include <string>
#include <sstream>

namespace umi {
  /**
   * Main class of the application. umison will be used to
   * encapsulate the initialization logic.
   *
   * Depending on the input parameters the behavior will change.
   * By default the input will be stdin and the output stdout, both
   * files will share the same output.
   *
   * If we want a different solution, like an input file or split
   * the header and the implementation in different files
   * we can use different parameters
   *
   * umison allowed options:
     * -h [ --help ]               produce this help message
     * -e [ --write-file-h ] arg   The .h output will be written to this file
     * -p [ --write-file-cpp ] arg The .cpp output will be written to this file
     * -i [ --read-file ] arg      Use this file as input xml template
     * -a [ --append-string ] arg  String to be appended in the internal namespace
     *                             to avoid collisions with existing code
   * */
  class umison {
  public:
    /**
     * Default constructor of the class, will accept a standar
     * input from the terminal
     *
     * \argc number of arguments
     * \argv arguments
     * */
    umison(int argc, char **argv);
    /**
     * Releases the resources used by the instance
     * */
    ~umison();
    /**
     * Run the program, in this case we will read the input stream
     * and output in the outputs stream
     * */
    void run();
  protected:
    /**
     * Stream used to enter data in the application, by default will
     * be std::cin
     * */
    std::istream m_inputStream;
    /**
     * Stream used to output data in the application, this stream
     * will be used to print the .h file
     *
     * By default is std::cout
     * */
    std::ostream m_h_stream;
    /**
     * Stream used to output data in the application, this stream
     * will be used to print the .cpp file
     *
     * By default is std::cout
     * */
    std::ostream m_cpp_stream;

    /**
     * Input file used to read the template we want to process
     * */
    std::ifstream m_input_file;
    /**
     * Stream to keep and manipulate the .h file
     * */
    std::ofstream m_h_file;
    /**
     * Stream to keep and manipulate the .cpp file
     * */
    std::ofstream m_cpp_file;
    /**
     * String used to store the file of the .h
     * */
    std::string m_h_filename;
    /**
     * String used to append data to the namespace
     * */
    std::string m_append_string;
    /**
     * String custom interface engine
     * */
    std::string m_custom_interface_engine;
  };
}

#endif
