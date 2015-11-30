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
#ifndef UMISON_UMISONOUTENGINERAPIDJSON_H_H
#define UMISON_UMISONOUTENGINERAPIDJSON_H_H

#include "umisonoutputengine.h"

namespace umi {
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
    virtual bool internal_write(std::shared_ptr<umi::umixml> &ff) {
      create_disclaimer();
      create_includes();
      // Create the internal namespace
      output_engine<T1, T2>::m_cpp_streamer << "namespace __internal__umison"
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
    void create_forward_declarations(std::shared_ptr<umi::umixml> &ff) {
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
    void create_forward_declaration_classes(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
        << TABS << "bool _read_list(std::vector<__internal__umison"
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
    void create_forward_declaration_map_classes(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n"
        << TABS << "bool _read_list(std::multimap<std::string, __internal__umison"
        << output_engine<T1, T2>::m_additional_string << "::" << class_map_it.first << "> &str, T &data);\n";
      }
    }

    /**
     * Creates the forward declaration of the parse method of the classes
     * */
    void create_forward_declaration_parse_classes(std::shared_ptr<umi::umixml> &ff) {
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
    void create_array_readers(std::shared_ptr<umi::umixml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Json type array readers implementation\n";
      create_array_readers_basic_types();
      create_array_readers_class_types(ff);
      output_engine<T1, T2>::m_cpp_streamer << "\n";
    }

    /**
     * Creates the different map readers
     * */
    void create_map_readers(std::shared_ptr<umi::umixml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Json type map readers implementation\n";
      create_map_readers_basic_types();
      create_map_readers_class_types(ff);
      output_engine<T1, T2>::m_cpp_streamer << "\n";
    }

    std::string build_indentation(const std::string &space, int level) {
      std::string retval;
      for (int i = 0; i < level; ++i) {
        retval += space;
      }
      return retval;
    }
    void _read_list(T2 &streamer, const std::string &space, const std::string &type,
                    const std::string &rapid_json_type, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      streamer << def_indentation << "template<typename T>\n"
      << def_indentation << "bool _read_list(std::vector<" << type << "> &str, T &data)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "if (!data.IsArray()) {\n"
      << def_2p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error data is not an array\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n\n"
      << def_1p_indentation << "for (rapidjson::SizeType i = 0; i < data.Size(); ++i) {\n"
      << def_2p_indentation << "if (data[i].Is" << rapid_json_type << "()) {\n"
      << def_3p_indentation << "str.push_back(data[i].Get" << rapid_json_type << "());\n"
      << def_2p_indentation << "} else {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error data is not an " <<
      rapid_json_type << "\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_1p_indentation << "}\n"
      << def_1p_indentation << "return true;\n"
      << def_indentation << "}\n\n";
    }

    void _read_map(T2 &streamer, const std::string &space, const std::string &type,
                   const std::string &rapid_json_type, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      streamer << def_indentation << "template<typename T>\n"
      << def_indentation << "bool _read_map(std::multimap<std::string, " << type << "> &str, T &data)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "for (rapidjson::Document::MemberIterator i = data.MemberBegin();\n"
      << def_2p_indentation << "i != data.MemberEnd();\n"
      << def_2p_indentation << "++i) {\n"
      << def_2p_indentation << "if (!i->name.IsString()) {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error map name is not a string\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "if (!i->value.Is" << rapid_json_type << "()) {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \"Error map value is not a " <<
      rapid_json_type << "\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "str.insert(std::make_pair(std::string(i->name.GetString()), (" << type <<
      ")(i->value.Get" << rapid_json_type << "())));\n"
      << def_1p_indentation << "}\n"
      << def_1p_indentation << "return true;\n"
      << def_indentation << "}\n\n";
    }

    void _read_list_class(T2 &streamer, const std::string &space, const std::string &additional_str,
                          const std::string &type, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      streamer << def_indentation << "template<typename T>\n"
      << def_indentation << "bool _read_list(std::vector<__internal__umison" << additional_str << "::" << type <<
      "> &str, T &data)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "if (!data.IsArray()) {\n"
      << def_2p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an array\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n\n"
      << def_1p_indentation << "for (rapidjson::SizeType i = 0; i < data.Size(); ++i) {\n"
      << def_2p_indentation << "__internal__umison" << additional_str << "::" << type << " local_" << type << ";\n"
      << def_2p_indentation << "if (" << type << "__input_parse(local_" << type << ", data[i])) {\n"
      << def_3p_indentation << "str.push_back(local_" << type << ");\n"
      << def_2p_indentation << "} else {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << type <<
      "\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_1p_indentation << "}\n"
      << def_1p_indentation << "return true;\n"
      << def_indentation << "}\n\n";
    }

    void _read_map_class(T2 &streamer, const std::string &space, const std::string &additional_str,
                         const std::string &type, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      std::string def_4p_indentation(def_3p_indentation + space);
      std::string def_5p_indentation(def_4p_indentation + space);
      streamer << def_indentation << "template<typename T>\n"
      << def_indentation << "bool _read_map(std::multimap<std::string, __internal__umison" << additional_str << "::" <<
      type << "> &str, T &data)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "for (rapidjson::Document::MemberIterator i = data.MemberBegin();\n"
      << def_2p_indentation << "i != data.MemberEnd();\n"
      << def_2p_indentation << "++i) {\n"
      << def_2p_indentation << "if (!i->name.IsString()) {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map name is not a string\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "if (i->value.IsObject()) {\n"
      << def_3p_indentation << "__internal__umison" << additional_str << "::" << type << " local_" << type << ";\n"
      << def_3p_indentation << "if (" << type << "__input_parse(local_" << type << ", i->value)) {\n"
      << def_4p_indentation << "str.insert(std::make_pair(std::string(i->name.GetString()), local_" << type << "));\n"
      << def_3p_indentation << "} else {\n"
      << def_4p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << type <<
      "\\n\";\n"
      << def_3p_indentation << "}\n"
      << def_2p_indentation << "} else if (i->value.IsArray()){\n"
      << def_3p_indentation << "for (rapidjson::SizeType j = 0; j < i->value.Size(); ++j){\n"
      << def_4p_indentation << "__internal__umison" << additional_str << "::" << type << " local_" << type << ";\n"
      << def_4p_indentation << "if (" << type << "__input_parse(local_" << type << ", i->value[j])) {\n"
      << def_5p_indentation << "str.insert(std::make_pair(std::string(i->name.GetString()), local_" << type << "));\n"
      << def_4p_indentation << "} else {\n"
      << def_5p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << type <<
      "\\n\";\n"
      << def_4p_indentation << "}\n"
      << def_3p_indentation << "}\n"
      << def_2p_indentation << "}\n"
      << def_1p_indentation << "}\n"
      << def_1p_indentation << "return true;\n"
      << def_indentation << "}\n\n";
    }

    /**
     * Create the readers for basic types
     * */
    void create_array_readers_basic_types() {
      _read_list((output_engine<T1, T2>::m_cpp_streamer), TABS, "int", "Int", 1);
      _read_list((output_engine<T1, T2>::m_cpp_streamer), TABS, "long long int", "Int64", 1);
      _read_list((output_engine<T1, T2>::m_cpp_streamer), TABS, "double", "Double", 1);
      _read_list((output_engine<T1, T2>::m_cpp_streamer), TABS, "bool", "Bool", 1);
      _read_list((output_engine<T1, T2>::m_cpp_streamer), TABS, "std::string", "String", 1);
    }

    /**
     * Create the readers for basic types
     * */
    void create_map_readers_basic_types() {
      _read_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "int", "Int", 1);
      _read_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "long long int", "Int64", 1);
      _read_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "double", "Double", 1);
      _read_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "bool", "Bool", 1);
      _read_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "std::string", "String", 1);
    }

    /**
     * Create the readers for the different class types
     * */
    void create_array_readers_class_types(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it : class_map) {
        _read_list_class((output_engine<T1, T2>::m_cpp_streamer), TABS,
                         (output_engine<T1, T2>::m_additional_string),
                         class_map_it.first, 1);
      }
    }

    /**
     * Create the readers for the different maps types on classes
     * */
    void create_map_readers_class_types(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it : class_map) {
        _read_map_class((output_engine<T1, T2>::m_cpp_streamer), TABS,
                        (output_engine<T1, T2>::m_additional_string),
                        class_map_it.first, 1);
      }
    }


    void _data_reader_single(T2 &streamer, const std::string &space, const std::string &type,
                             const std::string &class_name, const std::string rapid_json_type,
                             const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << rdata << "[\"" << type << "\"].Is" << rapid_json_type << "()) {\n"
      << def_1p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
      << class_name << " is wrong type " << type << "\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }

    void _data_reader_single_map(T2 &streamer, const std::string &space, const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      streamer << def_indentation << "if (" << rdata << ".MemberCount() == 0) {\n"
      << def_1p_indentation <<
      "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Error mandatory map with 0 elements\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n"
      << def_indentation << "for (rapidjson::Document::MemberIterator i = " << rdata << ".MemberBegin();\n"
      << def_1p_indentation << "i != " << rdata << ".MemberEnd();\n"
      << def_1p_indentation << "++i) {\n"
      << def_1p_indentation << "if (!i->name.IsString()) {\n"
      << def_2p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map key is not string\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n"
      << def_indentation << "}\n";
    }

    void _data_reader_single_assign(T2 &streamer, const std::string &space, const std::string &type_name,
                                    const std::string &rapid_json_type, const std::string &inout,
                                    const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      streamer << def_indentation << inout << type_name << " = " << rdata << "[\"" << type_name << "\"].Get"
      << rapid_json_type << "();\n";
    }

    void _data_reader_list_assign(T2 &streamer, const std::string &space, const std::string &additional_string,
                                  const std::string &type_name, const std::string &inout,
                                  const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation
      << "if (!__internal__umison" << additional_string << "::_read_list(" << inout << type_name << ", " << rdata <<
      "[\"" << type_name << "\"])) {\n"
      << def_1p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error reading list\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n\n";
    }

    void _data_reader_refclass_assign(T2 &streamer, const std::string &space, const std::string &refclass,
                                      const std::string &type_name, const std::string &inout,
                                      const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << refclass << "__input_parse(" << inout << type_name << ", " << rdata <<
      "[\"" << type_name << "\"])) {\n"
      << def_1p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error reading refclass\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n\n";
    }

    void _data_reader_map_assign(T2 &streamer, const std::string &space, const std::string &additional_string,
                                 const std::string &type_name, const std::string &inout,
                                 const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!__internal__umison" << additional_string << "::_read_map(" << inout <<
      type_name << ", " << rdata << ")) {\n"
      << def_1p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error reading map\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n\n";
    }


    void _data_reader_second_if_optional(T2 &streamer, const std::string &space,
                                         const std::string &type_name, const std::string &rapid_json_type,
                                         const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << rdata << "[\"" << type_name << "\"].Is" << rapid_json_type << "()) {\n"
      << def_1p_indentation << "std::cerr << \"Error wrong type\" << __LINE__ << \":\" << __FILE__ << \"\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }

    void _data_reader_second_if_optional_member(T2 &streamer, const std::string &space,
                                                const std::string &type_name, const std::string &rapid_json_type,
                                                const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      streamer << def_indentation << "if (" << rdata << ".MemberCount() > 0) {\n"
      << def_1p_indentation << "for (rapidjson::Document::MemberIterator i = " << rdata << ".MemberBegin();\n"
      << def_2p_indentation << "i != " << rdata << ".MemberEnd();\n"
      << def_2p_indentation << "++i) {\n"
      << def_2p_indentation << "if (!i->name.IsString()) {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map key is not string\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "if (!i->value.Is" << rapid_json_type << "()) {\n"
      << def_3p_indentation << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error map value is not " <<
      rapid_json_type << " Actual type - \" << i->value.GetType() << \"\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_1p_indentation << "}\n"
      << def_indentation << "}\n";
    }

    void _data_reader_second_if_optional_list(T2 &streamer, const std::string &space,
                                              const std::string &additional_string,
                                              const std::string &type_name, const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!__internal__umison" << additional_string << "::_read_list(inout."
      << type_name << ", " << rdata << "[\"" << type_name << "\"])) {\n"
      << def_1p_indentation << "std::cerr << \"Error reading array\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }

    void _data_reader_second_if_optional_refclass(T2 &streamer, const std::string &space, const std::string &refclass,
                                                  const std::string &type_name, const std::string &inout,
                                                  const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << refclass << "__input_parse(" << inout << type_name << ", " << rdata <<
      "[\"" << type_name << "\"])) {\n"
      << def_1p_indentation << "std::cerr << \"Error reading refclass\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }


    void _data_reader_second_if_optional_map(T2 &streamer, const std::string &space,
                                             const std::string &additional_string,
                                             const std::string &type_name, const std::string &inout,
                                             const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      streamer << def_indentation << "if (" << rdata << ".MemberCount() > 0) {\n"
      << def_1p_indentation << "if (!__internal__umison" << additional_string << "::_read_map(" << inout << type_name <<
      ", " << rdata << ")) {\n"
      << def_2p_indentation << "std::cerr << \"Error reading map\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n"
      << def_indentation << "}\n";
    }

    /**
     * Create the readers for the different data structures
     * */
    void create_data_readers(std::shared_ptr<umi::umixml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Data parsers\n";
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it : class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T>\n" << TABS
        << "bool " << class_map_it.first << "__input_parse(" << class_map_it.first << " &inout, T &rData)\n"
        << TABS << "{\n";
        const std::vector<std::shared_ptr<umixmltype>> &elements_class = class_map_it.second->getChildren();
        bool printedIsObject = false;
        for (auto &&single_element: elements_class) {
          if (single_element->condition().empty()) {
            // if !condition
            if (!single_element->optional()) {
              // First if checking the data contains that element but
              // Don't check the member if we have a map
              int actual_level = 1;
              if (!single_element->isMap()) {
                actual_level ++;
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!rData.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                // If it is not an object we will fail the parsing
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
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Bool", "rData", 2);
              } else if (single_element->isInteger()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Int64", "rData", 2);
              } else if (single_element->isInteger32()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Int", "rData", 2);
              } else if (single_element->isFloat()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Double", "rData", 2);
              } else if (single_element->isString()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "String", "rData", 2);
              } else if (single_element->isRefClass()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Object", "rData", 2);
              } else if (single_element->isList()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Array", "rData", 2);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_single_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "rData", 2);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "inout.", "rData", 2);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "inout.", "rData", 2);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "inout.", "rData", 2);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "inout.", "rData", 2);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "inout.", "rData", 2);
              } else if (single_element->isList()) {
                _data_reader_list_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), "inout.", "rData", 2);
              } else if (single_element->isRefClass()) {
                _data_reader_refclass_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                             single_element->refclass(), single_element->name(), "inout.",
                                             "rData", 2);
              } else if (single_element->isMap()) {
                _data_reader_map_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), "inout.", "rData", 2);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
            } else { // optional
              // First if
              int actual_level = 2;
              if (!single_element->isMap()) {
                actual_level++;
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!rData.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (rData.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Bool", "rData", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int64", "rData", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int", "rData", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Double", "rData", actual_level);
              } else if (single_element->isString()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "String", "rData", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Array", "rData", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Object", "rData", actual_level);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_second_if_optional_member((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), "rData", actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "inout.", "rData", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "inout.", "rData", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "inout.", "rData", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "inout.", "rData", actual_level);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "inout.", "rData", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional_list((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), "rData", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional_refclass((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         single_element->refclass(),
                                                         single_element->name(), "inout.", "rData", actual_level);
              } else if (single_element->isMap()) {
                _data_reader_second_if_optional_map((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), "inout.", "rData", actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << build_indentation(TABS, actual_level)
              << "inout." << single_element->optional_name() << " = true;\n";
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "}\n"; // Close first if
              }
            }
          } else {
            // else condition not empty
            output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
            << "if (" << single_element->condition() << ") {\n";
            // Evaluate contidion
            // Proceed as normal with one more indentation
            if (!single_element->optional()) {
              // First if checking the data contains that element
              if (!single_element->isMap()) {
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!rData.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
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
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Bool", "rData", 3);
              } else if (single_element->isInteger()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Int64", "rData", 3);
              } else if (single_element->isInteger32()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Int", "rData", 3);
              } else if (single_element->isFloat()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Double", "rData", 3);
              } else if (single_element->isString()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "String", "rData", 3);
              } else if (single_element->isRefClass()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Object", "rData", 3);
              } else if (single_element->isList()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it.first, "Array", "rData", 3);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_single_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "rData", 3);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "inout.", "rData", 3);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "inout.", "rData", 3);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "inout.", "rData", 3);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "inout.", "rData", 3);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "inout.", "rData", 3);
              } else if (single_element->isList()) {
                _data_reader_list_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), "inout.", "rData", 3);
              } else if (single_element->isRefClass()) {
                _data_reader_refclass_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                             single_element->refclass(), single_element->name(), "inout.",
                                             "rData", 3);
              } else if (single_element->isMap()) {
                _data_reader_map_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), "inout.", "rData", 3);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
            } else {
              // The data is optional
              int actual_level = 3;
              if (!single_element->isMap()) {
                actual_level++;
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!rData.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (rData.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Bool", "rData", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int64", "rData", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int", "rData", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Double", "rData", actual_level);
              } else if (single_element->isString()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "String", "rData", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Array", "rData", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Object", "rData", actual_level);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_second_if_optional_member((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), "rData", actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "inout.", "rData", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "inout.", "rData", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "inout.", "rData", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "inout.", "rData", actual_level);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "inout.", "rData", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional_list((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), "rData", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional_refclass((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         single_element->refclass(),
                                                         single_element->name(), "inout.", "rData", actual_level);
              } else if (single_element->isMap()) {
                _data_reader_second_if_optional_map((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), "inout.", "rData", actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer <<build_indentation(TABS, actual_level)
              << "inout." << single_element->optional_name() << " = true;\n";
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
    void create_json_readers(std::shared_ptr<umi::umixml> &ff) {
      output_engine<T1, T2>::m_cpp_streamer << TABS << "// Json parsers\n";
      auto json_array = ff->getJsonArray();
      for (auto &&class_map_it : json_array) {
        output_engine<T1, T2>::m_cpp_streamer
        << TABS << "bool umison::" << class_map_it->name() << "::read_data(const std::string &input_text)\n"
        << TABS << "{\n"
        << TABS << TABS << "rapidjson::Document _document;\n"
        << TABS << TABS << "if (_document.Parse(input_text.c_str()).HasParseError()) {\n"
        << TABS << TABS << TABS <<
        "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error parsing input text. Error: \" << _document.GetParseError() << \"\\n\";\n"
        << TABS << TABS << TABS << "return false;\n"
        << TABS << TABS << "}\n";
        bool printedIsObject = false;
        const std::vector<std::shared_ptr<umixmltype>> &elements_class = class_map_it->getChildren();
        for (auto &&single_element: elements_class) {
          if (single_element->condition().empty()) {
            // if !condition
            if (!single_element->optional()) {
              // First if checking the data contains that element
              if (!single_element->isMap()) {
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!_document.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (!_document.HasMember(\"" << single_element->name() << "\")) {\n"
                << TABS << TABS << TABS
                << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
                << class_map_it->name() << " is missing mandatory entry or entity is not an object" <<
                single_element->name() <<
                "\\n\";\n"
                << TABS << TABS << TABS << "return false;\n"
                << TABS << TABS << "}\n";
              }
              // Second if Check the type is the valid value
              if (single_element->isBoolean()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Bool", "_document", 2);
              } else if (single_element->isInteger()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Int64", "_document", 2);
              } else if (single_element->isInteger32()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Int", "_document", 2);
              } else if (single_element->isFloat()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Double", "_document", 2);
              } else if (single_element->isString()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "String", "_document", 2);
              } else if (single_element->isRefClass()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Object", "_document", 2);
              } else if (single_element->isList()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Array", "_document", 2);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_single_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "_document", 2);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "", "_document", 2);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "", "_document", 2);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "", "_document", 2);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "", "_document", 2);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "", "_document", 2);
              } else if (single_element->isList()) {
                _data_reader_list_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), "", "_document", 2);
              } else if (single_element->isRefClass()) {
                _data_reader_refclass_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                             single_element->refclass(), single_element->name(), "",
                                             "_document", 2);
              } else if (single_element->isMap()) {
                _data_reader_map_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), "", "_document", 2);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << "\n";
            } else { // optional
              // First if
              int actual_level = 2;
              if (!single_element->isMap()) {
                actual_level++;
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!_document.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                << "if (_document.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Bool", "_document", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int64", "_document", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int", "_document", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Double", "_document", actual_level);
              } else if (single_element->isString()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "String", "_document", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Array", "_document", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Object", "_document", actual_level);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_second_if_optional_member((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), "_document",
                                                       actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "", "_document", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "", "_document", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "", "_document", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "", "_document", actual_level);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "", "_document", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional_list((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), "_document", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional_refclass((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         single_element->refclass(),
                                                         single_element->name(), "inout.", "rData", actual_level);
              } else if (single_element->isMap()) {
                _data_reader_second_if_optional_map((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), "", "_document", actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << build_indentation(TABS, actual_level) << single_element->optional_name() <<
              " = true;\n";
              if (!single_element->isMap()) {
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "}\n"; // Close first if
              }
            }
          } else {
            // else condition not empty
            output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << "if (" <<
            single_element->condition() << ") {\n";
            // Evaluate contidion
            // Proceed as normal with one more indentation
            if (!single_element->optional()) {
              // First if checking the data contains that element
              if (!single_element->isMap()) {
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!_document.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (!_document.HasMember(\"" << single_element->name() << "\")) {\n"
                << TABS << TABS << TABS << TABS
                << "std::cerr << __FILE__ << \":\" << __LINE__ << \" Error entity: "
                << class_map_it->name() << " is missing mandatory entry or entity is not an object" <<
                single_element->name() <<
                "\\n\";\n"
                << TABS << TABS << TABS << TABS << "return false;\n"
                << TABS << TABS << TABS << "}\n";
              }
              // Second if Check the type is the valid value
              if (single_element->isBoolean()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Bool", "_document", 3);
              } else if (single_element->isInteger()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Int64", "_document", 3);
              } else if (single_element->isInteger32()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Int", "_document", 3);
              } else if (single_element->isFloat()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Double", "_document", 3);
              } else if (single_element->isString()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "String", "_document", 3);
              } else if (single_element->isRefClass()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Object", "_document", 3);
              } else if (single_element->isList()) {
                _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                    class_map_it->name(), "Array", "_document", 3);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_single_map((output_engine<T1, T2>::m_cpp_streamer), TABS, "_document", 3);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Third line, read the element depending on the type
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "", "_document", 3);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "", "_document", 3);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "", "_document", 3);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "", "_document", 3);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "", "_document", 3);
              } else if (single_element->isList()) {
                _data_reader_list_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                         (output_engine<T1, T2>::m_additional_string),
                                         single_element->name(), "", "_document", 3);
              } else if (single_element->isRefClass()) {
                _data_reader_refclass_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                             single_element->refclass(), single_element->name(), "",
                                             "_document", 3);
              } else if (single_element->isMap()) {
                _data_reader_map_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                        (output_engine<T1, T2>::m_additional_string),
                                        single_element->name(), "", "_document", 3);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << "\n";
            } else {
              int actual_level = 2;
              // The data is optional
              if (!single_element->isMap()) {
                actual_level++;
                if(!printedIsObject) {
                  output_engine<T1, T2>::m_cpp_streamer << TABS << TABS
                  << "if (!_document.IsObject()) {\n" << TABS << TABS << TABS
                  << "std::cerr << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
                  << TABS << TABS << TABS << "return false;\n"
                  << TABS << TABS << "}\n";
                  printedIsObject = true;
                }
                output_engine<T1, T2>::m_cpp_streamer << TABS << TABS << TABS
                << "if (_document.HasMember(\"" << single_element->name() << "\")) {\n";
              }
              // Second if
              if (single_element->isBoolean()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Bool", "_document", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int64", "_document", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Int", "_document", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Double", "_document", actual_level);
              } else if (single_element->isString()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "String", "_document", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Array", "_document", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                single_element->name(), "Object", "_document", actual_level);
              } else if (single_element->isMap()) {
                if (elements_class.size() > 1) {
                  std::cerr << "Error: More than one element on the same level within a map\n";
                  exit(-1);
                }
                _data_reader_second_if_optional_member((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                       single_element->name(),
                                                       internal_to_rapidjson(single_element->refclass()), "_document",
                                                       actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              // Assign data
              if (single_element->isBoolean()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Bool", "", "_document", actual_level);
              } else if (single_element->isInteger()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int64", "", "_document", actual_level);
              } else if (single_element->isInteger32()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Int", "", "_document", actual_level);
              } else if (single_element->isFloat()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "Double", "", "_document", actual_level);
              } else if (single_element->isString()) {
                _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                           single_element->name(), "String", "", "_document", actual_level);
              } else if (single_element->isList()) {
                _data_reader_second_if_optional_list((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     (output_engine<T1, T2>::m_additional_string),
                                                     single_element->name(), "_document", actual_level);
              } else if (single_element->isRefClass()) {
                _data_reader_second_if_optional_refclass((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                         single_element->refclass(),
                                                         single_element->name(), "", "_document", actual_level);
              } else if (single_element->isMap()) {
                _data_reader_second_if_optional_map((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                    (output_engine<T1, T2>::m_additional_string),
                                                    single_element->name(), "", "_document", actual_level);
              } else {
                std::cerr << "Invalid element\n";
                exit(-1);
              }
              output_engine<T1, T2>::m_cpp_streamer << build_indentation(TABS, actual_level)
              << single_element->optional_name() << " = true;\n";
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

#endif
