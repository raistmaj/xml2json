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
#ifndef CUDASON_CUDASONOUTENGINERAPIDJSON_H_H
#define CUDASON_CUDASONOUTENGINERAPIDJSON_H_H

#include "cudasonoutputengine.h"

namespace cuda {
  /**
   * rapid json output engine will be used to write the .cpp file using rapid json
   *
   * It heavily uses macros to reduce the size of the code
   * */
  template<typename T1, typename T2>
  class output_engine_rapid_json : public output_engine<T1, T2> {
  public:
    /**
     * Constructor, internally will call the output_engine constructor
     * */
    output_engine_rapid_json(T1 &_H_streamer, T2 &_CPP_streamer) : output_engine<T1, T2>(_H_streamer, _CPP_streamer) {
    }
    /**
     * Releases the resources used by this engine
     * */
    virtual ~output_engine_rapid_json() {
    }
  protected:
    /**
     * Method used to write the .cpp content to the cpp stream
     *
     * \param ff with the XML template already built
     * */
    virtual bool internal_write(std::shared_ptr<cuda::cudaxml> &ff) {
      create_disclaimer();
      create_includes();
      // Create the internal namespace
      output_engine<T1, T2>::m_cpp_streamer << "namespace __internal__cudason"
      << output_engine<T1, T2>::m_additional_string << " {\n\n";

      // Create forward data
      create_forward_declarations(ff);
      // Create the array parsers
      create_array_readers(ff);
      // Create the map readers
      create_map_readers(ff);
      // Create the json parsers
      create_data_readers(ff);
      // Close the namespace
      output_engine<T1, T2>::m_cpp_streamer << "}\n";
      // Create the final parser
      create_json_readers(ff);

      return true;
    }
    /**
     * Create the disclaimer at the top of the file
     * */
    void create_disclaimer() {
      output_engine<T1, T2>::m_cpp_streamer << DISCLAIMER << "\n";
    }
    /**
     * Create the include of the files in the .cpp file
     * */
    void create_includes() {
      if (!output_engine<T1, T2>::m_h_name.empty()) {
        output_engine<T1, T2>::m_cpp_streamer << "#include \""
        << output_engine<T1, T2>::m_h_name << "\"\n";
      }
      output_engine<T1, T2>::m_cpp_streamer << "#include \"rapidjson/rapidjson.h\"\n"
      << "#include \"rapidjson/document.h\"\n"
      << "#include <iostream>\n"
      << "#include <map>\n"
      << "#include <vector>\n\n";
    }
    /**
     * Create the forward declaration if we want to use any of the elements
     * afterwards
     * */
    void create_forward_declarations(std::shared_ptr<cuda::cudaxml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Forward declaration of parse functions\n";
      create_forward_declaration_basic_types();
      create_forward_declaration_classes(ff);
      create_forward_declaration_map_basic_types();
      create_forward_declaration_map_classes(ff);
      create_forward_declaration_parse_classes(ff);
      output_engine<T1, T2>::m_cpp_streamer << "\n";
    }
    /**
     * Create the forward declaration of the basic types
     * */
    void create_forward_declaration_basic_types() {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
      << TABS << "bool _read_list(std::vector<int> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_list(std::vector<long long int> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_list(std::vector<double> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_list(std::vector<bool> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_list(std::vector<std::string> &str, T &data);\n";
    }
    /**
     * Create the forward declaration of the array of classes
     * */
    void create_forward_declaration_classes(std::shared_ptr<cuda::cudaxml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
        << TABS << "bool _read_list(std::vector<__internal__cudason"
        << output_engine<T1, T2>::m_additional_string << "::" << class_map_it.first << "> &str, T &data);\n";
      }
    }
    /**
     * Create the forward declaration of the maps with basic types
     * */
    void create_forward_declaration_map_basic_types() {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
      << TABS << "bool _read_map(std::multimap<std::string, int> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_map(std::multimap<std::string, long long int> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_map(std::multimap<std::string, double> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_map(std::multimap<std::string, bool> &str, T &data);\n"
      << TABS << "template<typename T>\n"
      << TABS << "bool _read_map(std::multimap<std::string, std::string> &str, T &data);\n";
    }

    /**
     * Creates the forward declaration of the parse method of the classes
     * */
    void create_forward_declaration_map_classes(std::shared_ptr<cuda::cudaxml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
        << TABS << "bool _read_list(std::multimap<std::string, __internal__cudason"
        << output_engine<T1, T2>::m_additional_string << "::" << class_map_it.first << "> &str, T &data);\n";
      }
    }

    /**
     * Creates the forward declaration of the parse method of the classes
     * */
    void create_forward_declaration_parse_classes(std::shared_ptr<cuda::cudaxml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
        << TABS << "bool " << class_map_it.first << "__input_parse(" << class_map_it.first <<
        " &inout, T &rData);\n";
      }
    }


    /**
     * Creates the different array readers
     * */
    void create_array_readers(std::shared_ptr<cuda::cudaxml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Json type array readers implementation\n";
      create_array_readers_basic_types();
      create_array_readers_class_types(ff);
      output_engine<T1, T2>::m_cpp_streamer << "\n";
    }

    /**
     * Creates the different map readers
     * */
    void create_map_readers(std::shared_ptr<cuda::cudaxml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Json type map readers implementation\n";
      create_map_readers_basic_types();
      create_map_readers_class_types(ff);
      output_engine<T1, T2>::m_cpp_streamer << "\n";
    }

#define __READ_LIST(STREAMER, SPACE, TYPE, RAPID_JSON_TYPE)\
        STREAMER << SPACE << "template<typename T>\n"\
                 << SPACE << "bool _read_list(std::vector<"#TYPE"> &str, T &data)\n"\
                 << SPACE << "{\n"\
                 << SPACE << SPACE << "if (!data.IsArray()) {\n"\
                 << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error data is not an array\\n\";\n"\
                 << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << "}\n\n"\
                 << SPACE << SPACE << "for (rapidjson::SizeType i = 0; i < data.Size(); ++i) {\n"\
                 << SPACE << SPACE << SPACE << "if (data[i].Is"#RAPID_JSON_TYPE"()) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "str.push_back(data[i].Get"#RAPID_JSON_TYPE"());\n"\
                 << SPACE << SPACE << SPACE << "} else {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error data is not an "#RAPID_JSON_TYPE"\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "return true;\n"\
                 << SPACE << "}\n\n"

    /**
     * Create the readers for basic types
     * */
    void create_array_readers_basic_types() {
      __READ_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS, int, Int);
      __READ_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS, long
      long
          int, Int64);
      __READ_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS, double, Double);
      __READ_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS, bool, Bool);
      __READ_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS, std::string, String);
    }

#define __READ_MAP(STREAMER, SPACE, TYPE, RAPID_JSON_TYPE)\
    STREAMER     << SPACE << "template<typename T>\n"\
                 << SPACE << "bool _read_map(std::multimap<std::string, "#TYPE"> &str, T &data)\n"\
                 << SPACE << "{\n"\
                 << SPACE << SPACE << "for (rapidjson::Document::MemberIterator i = data.MemberBegin();\n"\
                 << SPACE << SPACE << SPACE << "i != data.MemberEnd();\n"\
                 << SPACE << SPACE << SPACE << "++i) {\n"\
                 << SPACE << SPACE << SPACE << "if (!i->name.IsString()) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error map name is not a string\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << SPACE << "if (!i->value.Is"#RAPID_JSON_TYPE"()) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error map value is not a "#RAPID_JSON_TYPE"\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << SPACE << "str.insert(std::make_pair(std::string(i->name.GetString()), ("#TYPE")(i->value.Get"#RAPID_JSON_TYPE"())));\n"\
                 << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "return true;\n"\
                 << SPACE << "}\n\n"

    /**
     * Create the readers for basic types
     * */
    void create_map_readers_basic_types() {
      __READ_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, int, Int);
      __READ_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, long
      long
          int, Int64);
      __READ_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, double, Double);
      __READ_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, bool, Bool);
      __READ_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, std::string, String);
    }

#define __READ_LIST_CLASS(STREAMER, SPACE, ADDITIONAL_STR, TYPE)\
        STREAMER << SPACE << "template<typename T>\n"\
                 << SPACE << "bool _read_list(std::vector<__internal__cudason" << ADDITIONAL_STR << "::" << TYPE << "> &str, T &data)\n"\
                 << SPACE << "{\n"\
                 << SPACE << SPACE << "if (!data.IsArray()) {\n"\
                 << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an array\\n\";\n"\
                 << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << "}\n\n"\
                 << SPACE << SPACE << "for (rapidjson::SizeType i = 0; i < data.Size(); ++i) {\n"\
                 << SPACE << SPACE << SPACE << "__internal__cudason" << ADDITIONAL_STR << "::" << TYPE << " local_" << TYPE << ";\n"\
                 << SPACE << SPACE << SPACE << "if (" << TYPE << "__input_parse(local_" << TYPE << ", data[i])) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "str.push_back(local_" << TYPE << ");\n"\
                 << SPACE << SPACE << SPACE << "} else {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << TYPE << "\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "return true;\n"\
                 << SPACE << "}\n\n"

    /**
     * Create the readers for the different class types
     * */
    void create_array_readers_class_types(std::shared_ptr<cuda::cudaxml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it : class_map) {
        __READ_LIST_CLASS((output_engine<T1, T2>::m_cpp_streamer), TABS,
                          (output_engine<T1, T2>::m_additional_string),
                          class_map_it.first);
      }
    }

#define __READ_MAP_CLASS(STREAMER, SPACE, ADDITIONAL_STR, TYPE)\
     STREAMER    << SPACE << "template<typename T>\n"\
                 << SPACE << "bool _read_map(std::multimap<std::string, __internal__cudason" << ADDITIONAL_STR << "::" << TYPE << "> &str, T &data)\n"\
                 << SPACE << "{\n"\
                 << SPACE << SPACE << "for (rapidjson::Document::MemberIterator i = data.MemberBegin();\n"\
                 << SPACE << SPACE << SPACE << "i != data.MemberEnd();\n"\
                 << SPACE << SPACE << SPACE << "++i) {\n"\
                 << SPACE << SPACE << SPACE << "if (!i->name.IsString()) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map name is not a string\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "return false;\n"\
                 << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << SPACE << "if (i->value.IsObject()) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << "__internal__cudason" << ADDITIONAL_STR << "::" << TYPE << " local_" << TYPE << ";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "if (" << TYPE << "__input_parse(local_" << TYPE << ", i->value)) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << "str.insert(std::make_pair(std::string(i->name.GetString()), local_" << TYPE << "));\n"\
                 << SPACE << SPACE << SPACE << SPACE << "} else {\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << TYPE << "\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << SPACE << "} else if (i->value.IsArray()){\n"\
                 << SPACE << SPACE << SPACE << SPACE << "for (rapidjson::SizeType j = 0; j < i->value.Size(); ++j){\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << "__internal__cudason" << ADDITIONAL_STR << "::" << TYPE << " local_" << TYPE << ";\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << "if (" << TYPE << "__input_parse(local_" << TYPE << ", i->value[j])) {\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << SPACE << "str.insert(std::make_pair(std::string(i->name.GetString()), local_" << TYPE << "));\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << "} else {\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << SPACE << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << TYPE << "\\n\";\n"\
                 << SPACE << SPACE << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "}\n"\
                 << SPACE << SPACE << "return true;\n"\
                 << SPACE << "}\n\n"
    /**
     * Create the readers for the different maps types on classes
     * */
    void create_map_readers_class_types(std::shared_ptr<cuda::cudaxml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it : class_map) {
        __READ_MAP_CLASS((output_engine<T1, T2>::m_cpp_streamer), TABS,
                         (output_engine<T1, T2>::m_additional_string),
                         class_map_it.first);
      }
    }

#define _DATA_READER_SINGLE(STREAMER, SPACE, ADDITIONAL_SPACE, TYPE_NAME, CLASS_NAME, RAPID_JSON_TYPE, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!"#RDATA"[\"" << TYPE_NAME << "\"].Is"#RAPID_JSON_TYPE"()) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "\
        << CLASS_NAME << " is wrong type " << TYPE_NAME << "\\n\";\n"\
        << SPACE << SPACE << SPACE <<  ((ADDITIONAL_SPACE == true)?SPACE:"") << "return false;\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") <<  "}\n"

#define _DATA_READER_SINGLE_MAP(STREAMER, SPACE, ADDITIONAL_SPACE, TYPE_NAME, CLASS_NAME, RAPID_JSON_TYPE, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if ("#RDATA".MemberCount() == 0) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Error mandatory map with 0 elements\\n\";\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "for (rapidjson::Document::MemberIterator i = "#RDATA".MemberBegin();\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "i != "#RDATA".MemberEnd();\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "++i) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!i->name.IsString()) {\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map key is not string\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"

#define _DATA_READER_SINGLE_ASSIGN(STREAMER, SPACE, ADDITIONAL_SPACE, TYPE_NAME, RAPID_JSON_TYPE, INOUT, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << ""#INOUT"" << TYPE_NAME\
        << " = "#RDATA"[\"" << TYPE_NAME << "\"].Get"#RAPID_JSON_TYPE"();\n"

#define _DATA_READER_LIST_ASSIGN(STREAMER, SPACE, ADDITIONAL_SPACE, ADDITIONAL_STRING, TYPE_NAME, INOUT, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!__internal__cudason" << ADDITIONAL_STRING << "::_read_list("\
        << ""#INOUT"" << TYPE_NAME << ", "#RDATA"[\"" << TYPE_NAME\
        << "\"])) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error reading list\\n\";\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") \
        << "}\n\n"

#define _DATA_READER_REFCLASS_ASSIGN(STREAMER, SPACE, ADDITIONAL_SPACE, REFCLASS, TYPE_NAME, INOUT, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!" << REFCLASS << "__input_parse("#INOUT""\
        << TYPE_NAME << ", "#RDATA"[\"" << TYPE_NAME\
        << "\"])) {\n"\
        <<  SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error reading refclass\\n\";\n"\
        <<  SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n\n"

#define _DATA_READER_MAP_ASSIGN(STREAMER, SPACE, ADDITIONAL_SPACE, ADDITIONAL_STRING, TYPE_NAME, INOUT, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!__internal__cudason" << ADDITIONAL_STRING << "::_read_map("#INOUT"" << TYPE_NAME << ", "#RDATA")) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error reading map\\n\";\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n\n"

#define _DATA_READER_SECOND_IF_OPTIONAL(STREAMER, SPACE, ADDITIONAL_SPACE, TYPE_NAME, RAPID_JSON_TYPE, RDATA)\
        STREAMER << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!"#RDATA"[\"" << TYPE_NAME << "\"].Is"#RAPID_JSON_TYPE"()) {\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << \"Error wrong type\" << __LINE__ << \":\" << __FILE__ << \"\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "}\n"

#define _DATA_READER_SECOND_IF_OPTIONAL_MEMBER(STREAMER, SPACE, ADDITIONAL_SPACE, TYPE_NAME, RAPID_JSON_TYPE, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if ("#RDATA".MemberCount() > 0) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "for (rapidjson::Document::MemberIterator i = "#RDATA".MemberBegin();\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "i != "#RDATA".MemberEnd();\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "++i) {\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!i->name.IsString()) {\n"\
        << SPACE << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map key is not string\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!i->value.Is" << RAPID_JSON_TYPE << "()) {\n"\
        << SPACE << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map value is not " << RAPID_JSON_TYPE << " Actual type - \" << i->value.GetType() << \"\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"\
        << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"

#define _DATA_READER_SECOND_IF_OPTIONAL_LIST(STREAMER, SPACE, ADDITIONAL_SPACE, ADDITIONAL_STRING, TYPE_NAME, RDATA)\
        STREAMER << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!__internal__cudason" << ADDITIONAL_STRING << "::_read_list(inout."\
        << TYPE_NAME << ", "#RDATA"[\"" << TYPE_NAME << "\"])) {\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "std::cerr << \"Error reading array\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "return false;\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "}\n"

#define _DATA_READER_SECOND_IF_OPTIONAL_REFCLASS(STREAMER, SPACE, ADDITIONAL_SPACE, REFCLAS, TYPE_NAME, INOUT, RDATA)\
        STREAMER << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!" << REFCLAS << "__input_parse("#INOUT""\
        << TYPE_NAME << ", "#RDATA"[\"" << TYPE_NAME << "\"])) {\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "std::cerr << \"Error reading refclass\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "return false;\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"") << "}\n"

#define _DATA_READER_SECOND_IF_OPTIONAL_MAP(STREAMER, SPACE, ADDITIONAL_SPACE, ADDITIONAL_STRING, TYPE_NAME, RDATA)\
        STREAMER << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if ("#RDATA".MemberCount() > 0) {\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "if (!__internal__cudason" << ADDITIONAL_STRING << "::_read_map(inout." << TYPE_NAME << ", "#RDATA")) {\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "std::cerr << \"Error reading map\\n\";\n"\
        << SPACE << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "return false;\n"\
        << SPACE << SPACE << SPACE << ((ADDITIONAL_SPACE == true)?SPACE:"")\
        << "}\n"

    /**
     * Create the readers for the different data structures
     * */
    void create_data_readers(std::shared_ptr<cuda::cudaxml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Data parsers\n";
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it : class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n" << TABS
        << "bool " << class_map_it.first << "__input_parse(" << class_map_it.first << " &inout, T &rData)\n"
        << TABS << "{\n";
        const std::vector<std::shared_ptr<cudaxmltype>> &elements_class = class_map_it.second->getChildren();
        for (auto &&single_element: elements_class) {
          if (single_element->condition().empty()) {
            // if !condition
            if (!single_element->optional()) {
              // First if checking the data contains that element but
              // Don't check the member if we have a map
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (!rData.HasMember(\"" << single_element->name() << "\")) {\n"
                << TABS << TABS << TABS
                << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
                << class_map_it.first << " is missing mandatory entry " << single_element->name() << "\\n\";\n"
                << TABS << TABS << TABS << "return false;\n"
                << TABS << TABS << "}\n";
              }
              // Second if Check the type is the valid value
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, Bool, rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, Int64, rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, Int, rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, Double, rData);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, String, rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, Object, rData);
              } else if (single_element->isList()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it.first, Array, rData);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SINGLE_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                        single_element->name(), class_map_it.first,
                                        internal_to_rapidjson(single_element->refclass()), rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Bool, inout., rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int64, inout., rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int, inout., rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Double, inout., rData);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), String, inout., rData);
              } else if (single_element->isList()) {
                _DATA_READER_LIST_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), inout., rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_REFCLASS_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                             single_element->refclass(), single_element->name(), inout.,
                                             rData);
              } else if (single_element->isMap()) {
                _DATA_READER_MAP_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), inout., rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
            } else { // optional
              // First if
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (rData.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Bool, rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Int64, rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Int, rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Double, rData);
              } else if (single_element->isString()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), String, rData);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Array, rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Object, rData);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SECOND_IF_OPTIONAL_MEMBER((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Bool, inout., rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int64, inout., rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int, inout., rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Double, inout., rData);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), String, inout., rData);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     false,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL_REFCLASS((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         false,
                                                         single_element->refclass(),
                                                         single_element->name(), inout., rData);
              } else if (single_element->isMap()) {
                _DATA_READER_SECOND_IF_OPTIONAL_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    false,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "}\n"; // Close first if
              }
            }
          } else {
            // else condition not empty
            output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
            << "if (" << single_element->condition() <<  ") {\n";
            // Evaluate contidion
            // Proceed as normal with one more indentation
            if (!single_element->optional()) {
              // First if checking the data contains that element
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (!rData.HasMember(\"" << single_element->name() << "\")) {\n"
                << TABS << TABS << TABS << TABS
                << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
                << class_map_it.first << " is missing mandatory entry " << single_element->name() << "\\n\";\n"
                << TABS << TABS << TABS << TABS << "return false;\n"
                << TABS << TABS << TABS << "}\n";
              }
              // Second if Check the type is the valid value
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, Bool, rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, Int64, rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, Int, rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, Double, rData);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, String, rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, Object, rData);
              } else if (single_element->isList()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it.first, Array, rData);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SINGLE_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                        single_element->name(), class_map_it.first,
                                        internal_to_rapidjson(single_element->refclass()), rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Bool, inout., rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int64, inout., rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int, inout., rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Double, inout., rData);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), String, inout., rData);
              } else if (single_element->isList()) {
                _DATA_READER_LIST_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), inout., rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_REFCLASS_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                             single_element->refclass(), single_element->name(), inout.,
                                             rData);
              } else if (single_element->isMap()) {
                _DATA_READER_MAP_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), inout., rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
            } else {
              // The data is optional
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (rData.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Bool, rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Int64, rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Int, rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Double, rData);
              } else if (single_element->isString()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), String, rData);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Array, rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Object, rData);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SECOND_IF_OPTIONAL_MEMBER((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Bool, inout., rData);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int64, inout., rData);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int, inout., rData);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Double, inout., rData);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), String, inout., rData);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     true,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), rData);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL_REFCLASS((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         true,
                                                         single_element->refclass(),
                                                         single_element->name(), inout., rData);
              } else if (single_element->isMap()) {
                _DATA_READER_SECOND_IF_OPTIONAL_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    true,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), rData);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS << "}\n"; // Close first if
              }
            }
            output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "}\n"; // Condition
          }
        }
        output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "return true;\n";
        output_engine<T1, T2>::m_cpp_streamer << TABS << "}\n\n"; // End of the function
      }
    }

    /**
     * Create the full json readers for each of the json elements
     * in the input template
     * */
    void create_json_readers(std::shared_ptr<cuda::cudaxml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Json parsers\n";
      auto json_array = ff->getJsonArray();
      for (auto &&class_map_it : json_array) {
        output_engine<T1, T2>::m_cpp_streamer
        << TABS << "bool cudason::" << class_map_it->name() << "::read_data(const std::string &input_text)\n"
        << TABS << "{\n"
        << TABS << TABS << "rapidjson::Document _document;\n"
        << TABS << TABS << "if (_document.Parse(input_text.c_str()).HasParseError()) {\n"
        << TABS << TABS << TABS <<
        "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error parsing input text. Error: \" << _document.GetParseError() << \"\\n\";\n"
        << TABS << TABS << TABS << "return false;\n"
        << TABS << TABS << "}\n";
        const std::vector<std::shared_ptr<cudaxmltype>> &elements_class = class_map_it->getChildren();
        for (auto &&single_element: elements_class) {
          if (single_element->condition().empty()) {
            // if !condition
            if (!single_element->optional()) {
              // First if checking the data contains that element
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (!_document.HasMember(\"" << single_element->name() << "\")) {\n"
                << TABS << TABS << TABS
                << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
                << class_map_it->name() << " is missing mandatory entry " << single_element->name() <<
                "\\n\";\n"
                << TABS << TABS << TABS << "return false;\n"
                << TABS << TABS << "}\n";
              }
              // Second if Check the type is the valid value
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), Bool, _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), Int64, _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), Int, _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), Double, _document);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), String, _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), Object, _document);
              } else if (single_element->isList()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                    single_element->name(), class_map_it->name(), Array, _document);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SINGLE_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                        single_element->name(), class_map_it.first,
                                        internal_to_rapidjson(single_element->refclass()), _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Bool, , _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int64, , _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int, , _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Double, , _document);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), String, , _document);
              } else if (single_element->isList()) {
                _DATA_READER_LIST_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), , _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_REFCLASS_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                             single_element->refclass(), single_element->name(), ,
                                             _document);
              } else if (single_element->isMap()) {
                _DATA_READER_MAP_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), , _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << "\n";
            } else { // optional
              // First if
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (_document.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Bool, _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Int64, _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Int, _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Double, _document);
              } else if (single_element->isString()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), String, _document);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Array, _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                single_element->name(), Object, _document);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SECOND_IF_OPTIONAL_MEMBER((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Bool, , _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int64, , _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Int, , _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), Double, , _document);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, false,
                                           single_element->name(), String, , _document);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     false,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), _document);
              } else if (single_element->isMap()) {
                _DATA_READER_SECOND_IF_OPTIONAL_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    false,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "}\n"; // Close first if
            }
          } else {
            // else condition not empty
            output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "if (" <<
            single_element->condition() <<
            ") {\n";
            // Evaluate contidion
            // Proceed as normal with one more indentation
            if (!single_element->optional()) {
              // First if checking the data contains that element
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (!_document.HasMember(\"" << single_element->name() << "\")) {\n"
                << TABS << TABS << TABS << TABS
                << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
                << class_map_it->name() << " is missing mandatory entry " << single_element->name() <<
                "\\n\";\n"
                << TABS << TABS << TABS << TABS << "return false;\n"
                << TABS << TABS << TABS << "}\n";
              }
              // Second if Check the type is the valid value
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), Bool, _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), Int64, _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), Int, _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), Double, _document);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), String, _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), Object, _document);
              } else if (single_element->isList()) {
                _DATA_READER_SINGLE((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                    single_element->name(), class_map_it->name(), Array, _document);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SINGLE_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                        single_element->name(), class_map_it.first,
                                        internal_to_rapidjson(single_element->refclass()), _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Bool, , _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int64, , _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int, , _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Double, , _document);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), String, , _document);
              } else if (single_element->isList()) {
                _DATA_READER_LIST_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), , _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_REFCLASS_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                             single_element->refclass(), single_element->name(), ,
                                             _document);
              } else if (single_element->isMap()) {
                _DATA_READER_MAP_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), , _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << "\n";
            } else {
              // The data is optional
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (_document.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Bool, _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Int64, _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Int, _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Double, _document);
              } else if (single_element->isString()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), String, _document);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Array, _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                single_element->name(), Object, _document);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _DATA_READER_SECOND_IF_OPTIONAL_MEMBER((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Bool, , _document);
              } else if (single_element->isInteger()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int64, , _document);
              } else if (single_element->isInteger32()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Int, , _document);
              } else if (single_element->isFloat()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), Double, , _document);
              } else if (single_element->isString()) {
                _DATA_READER_SINGLE_ASSIGN((output_engine<T1, T2>::m_cpp_streamer), TABS, true,
                                           single_element->name(), String, , _document);
              } else if (single_element->isList()) {
                _DATA_READER_SECOND_IF_OPTIONAL_LIST((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     true,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), _document);
              } else if (single_element->isRefClass()) {
                _DATA_READER_SECOND_IF_OPTIONAL_REFCLASS((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         true,
                                                         single_element->refclass(),
                                                         single_element->name(), , _document);
              } else if (single_element->isMap()) {
                _DATA_READER_SECOND_IF_OPTIONAL_MAP((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    true,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), _document);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS << "}\n"; // Close first if
              }
            }
            output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "}\n"; // Condition
          }
        }
        output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "return true;\n";
        output_engine<T1, T2>::m_cpp_streamer << TABS << "}\n\n"; // End of the function
      }
    }
  };
}

#endif //CUDASON_CUDASONOUTENGINERAPIDJSON_H_H