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

#include "umison.h"
#include "umisonoutputengines.h"
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

umi::umison::umison(int argc, char **argv) : m_inputStream(std::cin.rdbuf()),
                                             m_cpp_stream(std::cout.rdbuf()),
                                             m_h_stream(std::cout.rdbuf()) {
  boost::program_options::options_description description("umison allowed options");
  description.add_options()
      ("help,h", "produce this help message")
      ("write-file-h,e", boost::program_options::value<std::string>(),
       "The .h output will be written to this file")
      ("write-file-cpp,p", boost::program_options::value<std::string>(),
       "The .cpp output will be written to this file")
      ("read-file,i", boost::program_options::value<std::string>(), "Use this file as input xml template")
      ("append-string,a", boost::program_options::value<std::string>(),
       "String to be appended in the internal namespace to avoid collisions with existing code");

  boost::program_options::variables_map variables_map;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), variables_map);

  if (variables_map.count("help")) {
    std::cout << description << "\n";
    exit(1);
  }

  if (variables_map.count("write-file-h")) {
    m_h_filename = variables_map["write-file-h"].as<std::string>();
    m_h_file.open(m_h_filename);
    if (!m_h_file) {
      std::cerr << "Error opening " << variables_map["write-file-h"].as<std::string>() <<
      " please check your rights\n";
      exit(1);
    }
    m_h_stream.rdbuf(m_h_file.rdbuf());
  }

  if (variables_map.count("write-file-cpp")) {
    std::string file(variables_map["write-file-cpp"].as<std::string>());
    m_cpp_file.open(file);
    if (!m_cpp_file) {
      std::cerr << "Error opening " << variables_map["write-file-cpp"].as<std::string>() <<
      " please check your rights\n";
      exit(1);
    }
    m_cpp_stream.rdbuf(m_cpp_file.rdbuf());
  }

  if (variables_map.count("read-file")) {
    std::string file(variables_map["read-file"].as<std::string>());
    m_input_file.open(file);
    if (!m_input_file) {
      std::cerr << "Error opening " << variables_map["read-file"].as<std::string>() <<
      " please check your rights\n";
      exit(1);
    }
    m_inputStream.rdbuf(m_input_file.rdbuf());
  }

  if (variables_map.count("apped-string")) {
    m_append_string = variables_map["apped-string"].as<std::string>();
  }
}
umi::umison::~umison() {
  m_inputStream.rdbuf(std::cin.rdbuf());
  m_h_stream.rdbuf(std::cout.rdbuf());
  m_cpp_stream.rdbuf(std::cout.rdbuf());

  m_input_file.close();
  m_h_file.close();
  m_cpp_file.close();
}
void umi::umison::run() {
  std::string tmpString;
  {
    while (!m_inputStream.eof()) {
      char buf[1024];
      m_inputStream.getline(buf, sizeof(buf) - 1);
      tmpString += boost::algorithm::trim_copy(std::string(buf));
    }
  }
  auto xml = std::make_shared<umi::umixml>(tmpString);
  umi::output_engine_rapid_json<std::ostream, std::ostream> oe(m_h_stream, m_cpp_stream);
  oe.h_filename(m_h_filename);
  oe.additional_string(m_append_string);
  oe.write(xml);
}
