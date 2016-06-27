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
  static std::string build_indentation(const std::string &space, int level) {
    std::string retval;
    for (int i = 0; i < level; ++i) {
      retval += space;
    }
    return retval;
  }

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

    /**
     * Gets the list of additional headers to be included
     * */
    virtual std::vector<std::string> additional_headers() const {
      std::vector<std::string> retval;
      if (output_engine<T1, T2>::m_additional_engine_information == "rapidjson") {
        retval.emplace_back("rapidjson/rapidjson.h");
        retval.emplace_back("rapidjson/document.h");
      }
      return retval;
    };

    /**
     * Gets the list of additional methods to be declared
     * */
    virtual std::vector<std::string> additional_methods() const {
      std::vector<std::string> retval;
      if (output_engine<T1, T2>::m_additional_engine_information == "rapidjson") {
        retval.emplace_back("bool read_data(const rapidjson::Document &input_text);");
        retval.emplace_back("bool read_data(const rapidjson::Document &input_text, std::ostream &out_stream);");
      }
      return retval;
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
      output_engine<T1, T2>::m_cpp_streamer << "// Data parsers\n";
      auto &dataR = ff->getClassMap();
      for (auto &class_it: dataR) {
        create_single_reader(class_it.second, class_it.first, true, false);
        output_engine<T1, T2>::m_cpp_streamer << "\n";
      }
      // Close the namespace
      output_engine<T1, T2>::m_cpp_streamer << "}\n\n";
      // Create the final parsers
      output_engine<T1, T2>::m_cpp_streamer << "// Json parsers\n";
      auto &classes = ff->getJsonArray();
      for (auto &class_it: classes) {
        create_default_method_read(class_it);
        create_single_reader(class_it, "", false, false);
        if (output_engine<T1, T2>::m_additional_engine_information == "rapidjson") {
          create_single_reader(class_it, "", false, true);
        }
        output_engine<T1, T2>::m_cpp_streamer << "\n";
        create_single_reader(class_it, "", false, false, true);
        output_engine<T1, T2>::m_cpp_streamer << "\n";
      }
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
      << "#include <cstdlib>\n"
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
      output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_list(std::vector<int> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_list(std::vector<long long int> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_list(std::vector<double> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_list(std::vector<bool> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_list(std::vector<std::string> &str, T &data, Stream &ss);\n";
    }

    /**
     * Create the forward declaration of the array of classes
     * */
    void create_forward_declaration_classes(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T, typename Stream>\n"
        << TABS << "bool _read_list(std::vector<__internal__umison"
        << output_engine<T1, T2>::m_additional_string << "::" << class_map_it.first <<
        "> &str, T &data, Stream &ss);\n";
      }
    }

    /**
     * Create the forward declaration of the maps with basic types
     * */
    void create_forward_declaration_map_basic_types() {
      output_engine<T1, T2>::m_cpp_streamer
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_map(std::multimap<std::string, int> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_map(std::multimap<std::string, long long int> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_map(std::multimap<std::string, double> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_map(std::multimap<std::string, bool> &str, T &data, Stream &ss);\n"
      << TABS << "template<typename T, typename Stream>\n"
      << TABS << "bool _read_map(std::multimap<std::string, std::string> &str, T &data, Stream &ss);\n";
    }

    /**
     * Creates the forward declaration of the parse method of the classes
     * */
    void create_forward_declaration_map_classes(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T, typename Stream>\n"
        << TABS << "bool _read_list(std::multimap<std::string, __internal__umison"
        << output_engine<T1, T2>::m_additional_string << "::" << class_map_it.first <<
        "> &str, T &data, Stream &ss);\n";
      }
    }

    /**
     * Creates the forward declaration of the parse method of the classes
     * */
    void create_forward_declaration_parse_classes(std::shared_ptr<umi::umixml> &ff) {
      auto class_map = ff->getClassMap();
      for (auto &&class_map_it: class_map) {
        output_engine<T1, T2>::m_cpp_streamer << TABS << "template<typename T, typename Stream>\n"
        << TABS << "bool " << class_map_it.first << "__input_parse(" << class_map_it.first <<
        " &inout, T &rData, Stream &ss);\n";
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

    void _read_list(T2 &streamer, const std::string &space, const std::string &type,
                    const std::string &rapid_json_type, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      streamer << def_indentation << "template<typename T, typename Stream>\n"
      << def_indentation << "bool _read_list(std::vector<" << type << "> &str, T &data, Stream &ss)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "if (!data.IsArray()) {\n"
      << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \"Error data is not an array\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n\n"
      << def_1p_indentation << "for (rapidjson::SizeType i = 0; i < data.Size(); ++i) {\n"
      << def_2p_indentation << "if (data[i].Is" << rapid_json_type << "()) {\n"
      << def_3p_indentation << "str.push_back(data[i].Get" << rapid_json_type << "());\n"
      << def_2p_indentation << "} else {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \"Error data is not an " <<
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
      streamer << def_indentation << "template<typename T, typename Stream>\n"
      << def_indentation << "bool _read_map(std::multimap<std::string, " << type << "> &str, T &data, Stream &ss)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "for (rapidjson::Document::ConstMemberIterator i = data.MemberBegin();\n"
      << def_2p_indentation << "i != data.MemberEnd();\n"
      << def_2p_indentation << "++i) {\n"
      << def_2p_indentation << "if (!i->name.IsString()) {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \"Error map name is not a string\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "if (!i->value.Is" << rapid_json_type << "()) {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \"Error map value is not a " <<
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
      streamer << def_indentation << "template<typename T, typename Stream>\n"
      << def_indentation << "bool _read_list(std::vector<__internal__umison" << additional_str << "::" << type <<
      "> &str, T &data, Stream &ss)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "if (!data.IsArray()) {\n"
      << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error data is not an array\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n\n"
      << def_1p_indentation << "for (rapidjson::SizeType i = 0; i < data.Size(); ++i) {\n"
      << def_2p_indentation << "__internal__umison" << additional_str << "::" << type << " local_" << type << ";\n"
      << def_2p_indentation << "if (" << type << "__input_parse(local_" << type << ", data[i], ss)) {\n"
      << def_3p_indentation << "str.push_back(local_" << type << ");\n"
      << def_2p_indentation << "} else {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << type <<
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
      streamer << def_indentation << "template<typename T, typename Stream>\n"
      << def_indentation << "bool _read_map(std::multimap<std::string, __internal__umison" << additional_str << "::" <<
      type << "> &str, T &data, Stream &ss)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "for (rapidjson::Document::ConstMemberIterator i = data.MemberBegin();\n"
      << def_2p_indentation << "i != data.MemberEnd();\n"
      << def_2p_indentation << "++i) {\n"
      << def_2p_indentation << "if (!i->name.IsString()) {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error map name is not a string\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "if (i->value.IsObject()) {\n"
      << def_3p_indentation << "__internal__umison" << additional_str << "::" << type << " local_" << type << ";\n"
      << def_3p_indentation << "if (" << type << "__input_parse(local_" << type << ", i->value, ss)) {\n"
      << def_4p_indentation << "str.insert(std::make_pair(std::string(i->name.GetString()), local_" << type << "));\n"
      << def_3p_indentation << "} else {\n"
      << def_4p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << type <<
      "\\n\";\n"
      << def_3p_indentation << "}\n"
      << def_2p_indentation << "} else if (i->value.IsArray()){\n"
      << def_3p_indentation << "for (rapidjson::SizeType j = 0; j < i->value.Size(); ++j){\n"
      << def_4p_indentation << "__internal__umison" << additional_str << "::" << type << " local_" << type << ";\n"
      << def_4p_indentation << "if (" << type << "__input_parse(local_" << type << ", i->value[j], ss)) {\n"
      << def_5p_indentation << "str.insert(std::make_pair(std::string(i->name.GetString()), local_" << type << "));\n"
      << def_4p_indentation << "} else {\n"
      << def_5p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error data is not an " << type <<
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
                             const std::string &class_name, const std::string &rapid_json_type,
                             const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << rdata << "[\"" << type << "\"].Is" << rapid_json_type << "()) {\n"
      << def_1p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error entity: "
      << class_name << " is wrong type " << type << "\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }

    void _data_reader_single_map(T2 &streamer, const std::string &space, const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      streamer
      << def_indentation << "for (rapidjson::Document::ConstMemberIterator i = " << rdata << ".MemberBegin();\n"
      << def_1p_indentation << "i != " << rdata << ".MemberEnd();\n"
      << def_1p_indentation << "++i) {\n"
      << def_1p_indentation << "if (!i->name.IsString()) {\n"
      << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error map key is not string\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n"
      << def_indentation << "}\n";
    }

    void _data_reader_single_assign(T2 &streamer, const std::string &space, const std::string &type_name,
                                    const std::string &rapid_json_type, const std::string &inout,
                                    const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      streamer << def_indentation << inout << "mutable_" << type_name << "() = " << rdata <<
      "[\"" << type_name << "\"].Get"
      << rapid_json_type << "();\n";
    }

    void _data_reader_list_assign(T2 &streamer, const std::string &space, const std::string &additional_string,
                                  const std::string &type_name, const std::string &inout,
                                  const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation
      << "if (!__internal__umison" << additional_string << "::_read_list(" << inout
      << "mutable_" << type_name << "(), " << rdata
      << "[\"" << type_name << "\"], ss)) {\n"
      << def_1p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error reading list\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n\n";
    }

    void _data_reader_refclass_assign(T2 &streamer, const std::string &space, const std::string &refclass,
                                      const std::string &type_name, const std::string &inout,
                                      const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << refclass << "__input_parse(" << inout
      << "mutable_" << type_name << "(), " << rdata << "[\"" << type_name << "\"], ss)) {\n"
      << def_1p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error reading refclass\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n\n";
    }

    void _data_reader_map_assign(T2 &streamer, const std::string &space, const std::string &additional_string,
                                 const std::string &type_name, const std::string &inout,
                                 const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!__internal__umison" << additional_string << "::_read_map(" << inout
      << "mutable_" << type_name << "(), " << rdata << ", ss)) {\n"
      << def_1p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error reading map\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n\n";
    }


    void _data_reader_second_if_optional(T2 &streamer, const std::string &space,
                                         const std::string &type_name, const std::string &rapid_json_type,
                                         const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << rdata << "[\"" << type_name << "\"].Is" << rapid_json_type << "()) {\n"
      << def_1p_indentation << "ss << \"Error wrong type\" << __LINE__ << \":\" << __FILE__ << \"\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }

    void _data_reader_second_if_optional_member(T2 &streamer, const std::string &space,
                                                const std::string &/*type_name*/, const std::string &rapid_json_type,
                                                const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      std::string def_2p_indentation(def_1p_indentation + space);
      std::string def_3p_indentation(def_2p_indentation + space);
      streamer << def_indentation << "if (" << rdata << ".MemberCount() > 0) {\n"
      << def_1p_indentation << "for (rapidjson::Document::ConstMemberIterator i = " << rdata << ".MemberBegin();\n"
      << def_2p_indentation << "i != " << rdata << ".MemberEnd();\n"
      << def_2p_indentation << "++i) {\n"
      << def_2p_indentation << "if (!i->name.IsString()) {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error map key is not string\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_2p_indentation << "if (!i->value.Is" << rapid_json_type << "()) {\n"
      << def_3p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error map value is not " <<
      rapid_json_type << " Actual type - \" << i->value.GetType() << \"\\n\";\n"
      << def_3p_indentation << "return false;\n"
      << def_2p_indentation << "}\n"
      << def_1p_indentation << "}\n"
      << def_indentation << "}\n";
    }

    void _data_reader_second_if_optional_list(T2 &streamer, const std::string &space,
                                              const std::string &additional_string,
                                              const std::string &type_name, const std::string &inout,
                                              const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!__internal__umison" << additional_string << "::_read_list(" << inout
      << "mutable_" << type_name << "(), " << rdata << "[\"" << type_name << "\"], ss)) {\n"
      << def_1p_indentation << "ss << \"Error reading array\\n\";\n"
      << def_1p_indentation << "return false;\n"
      << def_indentation << "}\n";
    }

    void _data_reader_second_if_optional_refclass(T2 &streamer, const std::string &space, const std::string &refclass,
                                                  const std::string &type_name, const std::string &inout,
                                                  const std::string &rdata, int level) {
      std::string def_indentation(build_indentation(space, level));
      std::string def_1p_indentation(def_indentation + space);
      streamer << def_indentation << "if (!" << refclass << "__input_parse(" << inout
      << "mutable_" << type_name << "(), " << rdata << "[\"" << type_name << "\"], ss)) {\n"
      << def_1p_indentation << "ss << \"Error reading refclass\\n\";\n"
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
      << def_1p_indentation << "if (!__internal__umison" << additional_string << "::_read_map(" << inout
      << "mutable_" << type_name << "(), " << rdata << ", ss)) {\n"
      << def_2p_indentation << "ss << \"Error reading map\\n\";\n"
      << def_2p_indentation << "return false;\n"
      << def_1p_indentation << "}\n"
      << def_indentation << "}\n";
    }

    void create_default_additional_method(const std::shared_ptr<umixmltypeclass> &ff) {
      std::string def_indentation(build_indentation(TABS, 0));
      std::string def_1p_indentation(def_indentation + TABS);
      output_engine<T1, T2>::m_cpp_streamer
      << def_indentation << "bool umison::" << ff->name() << "::read_data(const rapidjson::Document &input_text)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "return this->read_data(input_text, std::cerr);\n"
      << def_indentation << "}\n\n";
    }


    void create_default_method_read(const std::shared_ptr<umixmltypeclass> &ff) {
      std::string def_indentation(build_indentation(TABS, 0));
      std::string def_1p_indentation(def_indentation + TABS);
      output_engine<T1, T2>::m_cpp_streamer
      << def_indentation << "bool umison::" << ff->name() << "::read_data(const std::string &input_text)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "return this->read_data(input_text, std::cerr);\n"
      << def_indentation << "}\n\n"
      << def_indentation << "bool umison::" << ff->name() << "::read_data_from_file(const std::string &input)\n"
      << def_indentation << "{\n"
      << def_1p_indentation << "return this->read_data_from_frile(input, std::cerr);\n"
      << def_indentation << "}\n\n";
    }

    /**
     * Method to create a reader based if we are a json reader or a input_parse reader. Creates one reader per element
     * being an element a class or data_parser
     * */
    void create_single_reader(const std::shared_ptr<umixmltypeclass> &ff, const std::string &name, bool data_reader,
                              bool additional_reader, bool from_file = false) {
      int actual_level = 1;
      std::string inout;
      std::string inout_dot;
      std::string rdata;
      // Set variables depending on each mode
      if (data_reader) {
        inout = "inout";
        inout_dot = "inout.";
        rdata = "rdata";
      } else {
        actual_level--;
        rdata = "_document";
      }
      std::string def_indentation(build_indentation(TABS, actual_level));
      std::string def_1p_indentation(def_indentation + TABS);
      std::string def_2p_indentation(def_1p_indentation + TABS);
      std::string def_3p_indentation(def_2p_indentation + TABS);
      std::string def_4p_indentation(def_3p_indentation + TABS);
      // Proceed with the header of the reader
      if (data_reader) {
        output_engine<T1, T2>::m_cpp_streamer
        << def_indentation << "template<typename T, typename Stream>\n"
        << def_indentation << "bool " << name << "__input_parse(" << name << " &" << inout << ", T &" << rdata <<
        ", Stream &ss)\n"
        << def_indentation << "{\n";
      } else {
        if (!additional_reader) {
          if (!from_file) {
            output_engine<T1, T2>::m_cpp_streamer
            << def_indentation << "bool umison::" << ff->name()
            << "::read_data(const std::string &input_text, std::ostream &ss)\n"
            << def_indentation << "{\n"
            << def_1p_indentation << "rapidjson::Document " << rdata << ";\n"
            << def_1p_indentation << "if (" << rdata << ".Parse(input_text.c_str()).HasParseError()) {\n"
            << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error parsing input text. Error: \" << "
            << rdata << ".GetParseError() << \"\\n\";\n"
            << def_2p_indentation << "return false;\n"
            << def_1p_indentation << "}\n";
          } else {
            output_engine<T1, T2>::m_cpp_streamer
            << def_indentation << "bool umison::" << ff->name()
            << "::read_data_from_file(const std::string &input, std::ostream &ss)\n"
            << def_indentation << "{\n"
            << def_1p_indentation << "if (input.empty()) {\n"
            << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Filename is empty.\\n\";\n"
            << def_2p_indentation << "return false;\n"
            << def_1p_indentation << "}\n"
            << def_1p_indentation << "FILE *fi = fopen(input.c_str(), \"rb\");\n"
            << def_1p_indentation << "if (fi == 0) {\n"
            << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error file can't be opened.\\n\";\n"
            << def_2p_indentation << "return false;\n"
            << def_1p_indentation << "}\n"
            << def_1p_indentation << "fseek(fi, 0, SEEK_END);\n"
            << def_1p_indentation << "long fi_size = ftell(fi);\n"
            << def_1p_indentation << "fseek(fi, 0, SEEK_BEG);\n"
            << def_1p_indentation << "if (fi_size == 0) {\n"
            << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error file is empty.\\n\";\n"
            << def_2p_indentation << "fclose(fi);\n"
            << def_2p_indentation << "return false;\n"
            << def_1p_indentation << "}\n"
            << def_1p_indentation << "long actual_pos = 0;\n"
            << def_1p_indentation << "std::vector<char> fi_content(fi_size, static_cast<char>(0x00));\n"
            << def_1p_indentation << "while (actual_pos < fi_size) {\n"
            << def_2p_indentation << "size_t ret_read = fread(&fi_content[actual_pos], 1, fi_size - actual_pos, fi);\n"
            << def_2p_indentation << "if (ret_read != static_cast<size_t>(fi_size - actual_pos)) {\n"
            << def_3p_indentation << "if (foef(fi)) {\n"
            << def_4p_indentation << "break;\n"
            << def_3p_indentation << "} else if (ferror(fi)) {\n"
            << def_4p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error reading file.\\n\";\n"
            << def_4p_indentation << "fclose(fi);\n"
            << def_4p_indentation << "return false;\n"
            << def_3p_indentation << "} else {\n"
            << def_4p_indentation << "actual_pos += static_cast<long>(ret_read);\n"
            << def_3p_indentation << "}\n"
            << def_2p_indentation << "} else {\n"
            << def_3p_indentation << "break;\n"
            << def_2p_indentation << "}\n"
            << def_1p_indentation << "}\n"
            << def_1p_indentation << "fclose(fi);\n"
            << def_1p_indentation << "std::string input_text(fi_content.begin(), fi_content.end());\n"
            << def_1p_indentation << "rapidjson::Document " << rdata << ";\n"
            << def_1p_indentation << "if (" << rdata << ".Parse(input_text.c_str()).HasParseError()) {\n"
            << def_2p_indentation << "ss << __FILE__ << \":\" << __LINE__ << \" Error parsing input text. Error: \" << "
            << rdata << ".GetParseError() << \"\\n\";\n"
            << def_2p_indentation << "return false;\n"
            << def_1p_indentation << "}\n";
          }
        } else {
          output_engine<T1, T2>::m_cpp_streamer
          << def_indentation << "bool umison::" << ff->name()
          << "::read_data(const rapidjson::Document &" << rdata << ", std::ostream &ss)\n"
          << def_indentation << "{\n";
        }
      }
      bool printedIsObject = false;
#define STATUS_CONDITION_INITIAL 0
#define STATUS_CONDITION_IN_CONDITION 1
      int status_condition = STATUS_CONDITION_INITIAL;
      std::string actualCondition;
      auto &elements = ff->getChildren();
      ++actual_level;
      for (auto &single_element : elements) {
        bool openCondition = true;
        // Check the condition and open conditional if necessary
        if (actualCondition == single_element->condition() && !actualCondition.empty()) {
          // We keep the same condition and it is not empty dont open it
          openCondition = false;
        }
        // if we are in one condition and the option is different
        // close the previous one
        if (status_condition == STATUS_CONDITION_IN_CONDITION && actualCondition != single_element->condition()) {
          --actual_level;
          output_engine<T1, T2>::m_cpp_streamer
          << def_1p_indentation << "}\n";
          status_condition = STATUS_CONDITION_INITIAL;
        }
        if (!single_element->condition().empty() && openCondition && status_condition == STATUS_CONDITION_INITIAL) {
          status_condition = STATUS_CONDITION_IN_CONDITION;
          actualCondition = single_element->condition();
          output_engine<T1, T2>::m_cpp_streamer
          << def_1p_indentation << "if (" << inout_dot << single_element->condition() << ") {\n";
          // Increase indentation
          ++actual_level;
        }
        // Check if the element is optional
        if (single_element->optional()) {
          if (!single_element->isMap()) {
            if (!printedIsObject) {
              output_engine<T1, T2>::m_cpp_streamer
              << build_indentation(TABS, actual_level) << "if (!" << rdata << ".IsObject()) {\n"
              << build_indentation(TABS, actual_level + 1) <<
              "ss << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
              << build_indentation(TABS, actual_level + 1) << "return false;\n"
              << build_indentation(TABS, actual_level) << "}\n";
              printedIsObject = true;
            }
            output_engine<T1, T2>::m_cpp_streamer
            << build_indentation(TABS, actual_level) << "if (" << rdata << ".HasMember(\""
            << single_element->name() << "\")) {\n";
            ++actual_level;
          }
        }
        // Check if the object checking has been already printed
        // So we avoid problems checking the rest of elements
        if (!single_element->isMap()) {
          if (!printedIsObject) {
            output_engine<T1, T2>::m_cpp_streamer
            << build_indentation(TABS, actual_level) << "if (!" << rdata << ".IsObject()) {\n"
            << build_indentation(TABS, actual_level + 1) <<
            "ss << __FILE__ <<  \":\" << __LINE__ << \" Element is not an object\\n\";\n"
            << build_indentation(TABS, actual_level + 1) << "return false;\n"
            << build_indentation(TABS, actual_level) << "}\n";
            printedIsObject = true;
          }
          // Get if the type is valid
          if (!single_element->optional()) {
            output_engine<T1, T2>::m_cpp_streamer
            << build_indentation(TABS, actual_level) << "if (!" << rdata << ".HasMember(\"" << single_element->name()
            << "\")) {\n"
            << build_indentation(TABS, actual_level + 1) <<
            "ss << __FILE__ << \":\" << __LINE__ << \" Error entity: "
            << name << " is missing mandatory entry or entity is not an object" << single_element->name()
            << "\\n\";\n"
            << build_indentation(TABS, actual_level + 1) << "return false;\n"
            << build_indentation(TABS, actual_level) << "}\n";
          }
        }
        // Create the if depending on the optionality of the elements 2º IF
        if (single_element->isMap()) {
          if (elements.size() > 1) {
            std::cerr << "Error: More than one element on the same level within a map\n";
            exit(-1);
          }
          if (!single_element->optional()) {
            _data_reader_single_map((output_engine<T1, T2>::m_cpp_streamer), TABS, rdata, actual_level);
          } else {
            _data_reader_second_if_optional_member((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                   single_element->name(),
                                                   internal_to_rapidjson(single_element->refclass()), rdata,
                                                   actual_level);
          }
        } else {
          std::string base_element;
          if (single_element->isBoolean()) {
            base_element = "Bool";
          } else if (single_element->isInteger32()) {
            base_element = "Int";
          } else if (single_element->isInteger()) {
            base_element = "Int64";
          } else if (single_element->isFloat()) {
            base_element = "Double";
          } else if (single_element->isString()) {
            base_element = "String";
          } else if (single_element->isList()) {
            base_element = "Array";
          } else if (single_element->isRefClass()) {
            base_element = "Object";
          } else {
            std::cerr << "Invalid element: " << single_element->name() << "\n";
            exit(-1);
          }
          if (!single_element->optional()) {
            _data_reader_single((output_engine<T1, T2>::m_cpp_streamer), TABS, single_element->name(),
                                name, base_element, rdata, actual_level);
          } else {
            _data_reader_second_if_optional((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                            single_element->name(), base_element, rdata, actual_level);
          }
        }

        // Last part is assignement of elements, 3º IF
        if (single_element->isMap()) {
          if (!single_element->optional()) {
            _data_reader_map_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                    (output_engine<T1, T2>::m_additional_string),
                                    single_element->name(), inout_dot, rdata, actual_level);
          } else {
            _data_reader_second_if_optional_map((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                (output_engine<T1, T2>::m_additional_string),
                                                single_element->name(), inout_dot, rdata, actual_level);
          }
        } else if (single_element->isRefClass()) {
          if (!single_element->optional()) {
            _data_reader_refclass_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                         single_element->refclass(), single_element->name(), inout_dot, rdata,
                                         actual_level);
          } else {
            _data_reader_second_if_optional_refclass((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                     single_element->refclass(),
                                                     single_element->name(), inout_dot, rdata, actual_level);
          }
        } else if (single_element->isList()) {
          if (!single_element->optional()) {
            _data_reader_list_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                     (output_engine<T1, T2>::m_additional_string),
                                     single_element->name(), inout_dot, rdata, actual_level);
          } else {
            _data_reader_second_if_optional_list((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                                 (output_engine<T1, T2>::m_additional_string),
                                                 single_element->name(), inout_dot, rdata, actual_level);
          }
        } else {
          std::string base_element;
          if (single_element->isBoolean()) {
            base_element = "Bool";
          } else if (single_element->isInteger()) {
            base_element = "Int64";
          } else if (single_element->isInteger32()) {
            base_element = "Int";
          } else if (single_element->isFloat()) {
            base_element = "Double";
          } else if (single_element->isString()) {
            base_element = "String";
          } else {
            std::cerr << "Invalid element: " << single_element->name() << "\n";
            exit(-1);
          }
          _data_reader_single_assign((output_engine<T1, T2>::m_cpp_streamer), TABS,
                                     single_element->name(), base_element, inout_dot, rdata, actual_level);
        }
        // New Line
        output_engine<T1, T2>::m_cpp_streamer << "\n";
        // Close the if conditional
        if (single_element->optional()) {
          output_engine<T1, T2>::m_cpp_streamer
          << build_indentation(TABS, actual_level) << inout_dot
          << "mutable_" << single_element->optional_name() << "() = true;\n";
          if (!single_element->isMap()) {
            --actual_level;
            output_engine<T1, T2>::m_cpp_streamer
            << build_indentation(TABS, actual_level) << "}\n";
          }
        }
      }
      if (status_condition == STATUS_CONDITION_IN_CONDITION) {
        --actual_level;
        output_engine<T1, T2>::m_cpp_streamer
        << def_1p_indentation << "}\n";
      }
      output_engine<T1, T2>::m_cpp_streamer << def_1p_indentation << "return true;\n";
      // Close the method
      output_engine<T1, T2>::m_cpp_streamer
      << def_indentation << "}\n";
    }
  };
}

#endif
