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
#include "umixml.h"
#include "umixmltags.h"
#include "umixmltypestring.h"
#include <boost/algorithm/string.hpp>
#include <exception>
#include <iostream>
#include <pugixml.hpp>

namespace umi {
  /**
   * Class used to help on the reading process of the xml
   * to avoid an ugly public interface or high compile times
   * */
  class umixml_helper {
  public:
    /**
     * Checks if the type is an internal type
     * */
    bool is_internal(const std::string &val) {
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
    /**
     * Transform one boolean value string to a valid bool c++ value
     * throws an exception in case not conversion is possible
     * */
    bool boolean_to_bool(const char *value) {
      if (boost::algorithm::iequals(value, "true")) {
        return true;
      } else if (boost::algorithm::iequals(value, "false")) {
        return false;
      }
      throw std::runtime_error("Wrong value");
    }
    /**
     * Reads one node from rapid xml and fills the map stored in it
     * */
    bool read_map_node(std::shared_ptr<umixmltypemap> &map_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting map node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            map_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              map_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            map_node->condition(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_VALUE, node_attribute->name(), strlen(UMI_XMLATTR_VALUE)) ==
                     0) {
            map_node->refclass(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node refclass: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads one node from rapid xml and fills the referenced class stored in it
     * */
    bool read_refclass_node(std::shared_ptr<umixmltyperefclass> &refclass_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting refclass node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            refclass_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              refclass_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            refclass_node->condition(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_REFCLASS, node_attribute->name(), strlen(UMI_XMLATTR_REFCLASS)) ==
                     0) {
            refclass_node->refclass(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node refclass: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads one node from rapid xml and fills the list stored in it
     * */
    bool read_list_node(std::shared_ptr<umixmltypelist> &list_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting list node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            list_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              list_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            list_node->condition(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_REFCLASS, node_attribute->name(), strlen(UMI_XMLATTR_REFCLASS)) ==
                     0) {
            list_node->refclass(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node list: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads on node from rapid xml and fills the bool stored in it
     * */
    bool read_boolean_node(std::shared_ptr<umixmltypeboolean> &boolean_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting boolean node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            boolean_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              boolean_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            boolean_node->condition(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node boolean: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads on node from rapid xml and fills the float stored in it
     * */
    bool read_float_node(std::shared_ptr<umixmltypefloat> &float_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting float node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            float_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              float_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            float_node->condition(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node float: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads on node from rapid xml and fills the string stored in it
     * */
    bool read_string_node(std::shared_ptr<umixmltypestring> &string_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting string node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            string_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              string_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            string_node->condition(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node string: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads on node from rapid xml and fills the integer stored in it
     * */
    bool read_integer_node(std::shared_ptr<umixmltypeinteger> &integer_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting integer node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            integer_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              integer_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            integer_node->condition(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node integer: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads on node from rapid xml and fills the integer32 stored in it
     * */
    bool read_integer32_node(std::shared_ptr<umixmltypeinteger32> &integer_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node. Expecting integer32 node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            integer_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              integer_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            integer_node->condition(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node integer32: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        retval = true;
      }
      return retval;
    }
    /**
     * Reads one node from rapid xml and fills the data
     * */
    bool read_class_node(std::shared_ptr<umixmltypeclass> &class_node, pugi::xml_node &node) {
      bool retval = false;
      if (node == nullptr) {
        std::cerr << "Trying to read a nullptr node.\n";
      } else {
        // Get attributes and fill them
        pugi::xml_attribute_iterator node_attribute = node.attributes_begin();
        while (node_attribute != node.attributes_end()) {
          if (strncmp(UMI_XMLATTR_NAME, node_attribute->name(), strlen(UMI_XMLATTR_NAME)) == 0) {
            class_node->name(node_attribute->value());
          } else if (strncmp(UMI_XMLATTR_OPTIONAL, node_attribute->name(), strlen(UMI_XMLATTR_OPTIONAL)) ==
                     0) {
            try {
              class_node->optional(boolean_to_bool(node_attribute->value()));
            } catch (std::exception &ex) {
              std::cerr << "Wrong boolean value: " << node_attribute->value()
              << ". Setting optional to false.\n";
            }
          } else if (
              strncmp(UMI_XMLATTR_CONDITION, node_attribute->name(), strlen(UMI_XMLATTR_CONDITION)) ==
              0) {
            class_node->condition(node_attribute->value());
          } else {
            std::cerr << "Unsupported attribute on node class: << " << node_attribute->name()
            << " ignoring it.\n";
          }
          node_attribute++;
        }
        // Go over each of the children filling them
        pugi::xml_node child_node(node.first_child());
        while (child_node) {
          std::shared_ptr<umi::umixmltype> ptr_type;
          if (strncmp(UMI_XMLTAGS_INTEGER, child_node.name(), strlen(UMI_XMLTAGS_INTEGER)) == 0) {
            auto ptr_integer = std::make_shared<umi::umixmltypeinteger>();
            read_integer_node(ptr_integer, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_integer));
          } else if (strncmp(UMI_XMLTAGS_INTEGER32, child_node.name(), strlen(UMI_XMLTAGS_INTEGER32)) ==
                     0) {
            auto ptr_integer = std::make_shared<umi::umixmltypeinteger32>();
            read_integer32_node(ptr_integer, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_integer));
          } else if (strncmp(UMI_XMLTAGS_STRING, child_node.name(), strlen(UMI_XMLTAGS_STRING)) == 0) {
            auto ptr_string = std::make_shared<umi::umixmltypestring>();
            read_string_node(ptr_string, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_string));
          } else if (strncmp(UMI_XMLTAGS_FLOAT, child_node.name(), strlen(UMI_XMLTAGS_FLOAT)) == 0) {
            auto ptr_float = std::make_shared<umi::umixmltypefloat>();
            read_float_node(ptr_float, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_float));
          } else if (strncmp(UMI_XMLTAGS_BOOLEAN, child_node.name(), strlen(UMI_XMLTAGS_BOOLEAN)) == 0) {
            auto ptr_bool = std::make_shared<umi::umixmltypeboolean>();
            read_boolean_node(ptr_bool, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_bool));
          } else if (strncmp(UMI_XMLTAGS_LIST, child_node.name(), strlen(UMI_XMLTAGS_LIST)) == 0) {
            auto ptr_list = std::make_shared<umi::umixmltypelist>();
            read_list_node(ptr_list, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_list));
          } else if (strncmp(UMI_XMLTAGS_REFCLASS, child_node.name(), strlen(UMI_XMLTAGS_REFCLASS)) == 0) {
            auto ptr_refclass = std::make_shared<umi::umixmltyperefclass>();
            read_refclass_node(ptr_refclass, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_refclass));
          } else if (strncmp(UMI_XMLTAGS_MAP, child_node.name(), strlen(UMI_XMLTAGS_MAP)) == 0) {
            auto ptr_refclass = std::make_shared<umi::umixmltypemap>();
            read_map_node(ptr_refclass, child_node);
            class_node->addChildren(std::dynamic_pointer_cast<umi::umixmltype>(ptr_refclass));
          } else {
            std::cerr << "Unsupported node on node class: << " << child_node.name() << " ignoring it.\n";
          }
          child_node = child_node.next_sibling();
        }
        retval = true;
      }
      return retval;
    }
  };
}

umi::umixml::umixml(const std::string &content) {
  if (content.empty()) {
    return;
  }
  try {
    umi::umixml_helper umi_helper;
    std::vector<pugi::xml_node> json_nodes;
    std::vector<pugi::xml_node> class_nodes;

    pugi::xml_document local_document;
    pugi::xml_parse_result parse_result = local_document.load_string(content.c_str());
    if (!parse_result) {
      std::cerr << "Error parsing the input string.\n" << parse_result.description() << "\n"
      << "Offset: " << parse_result.offset << "\n";
      throw std::runtime_error("Error parsing the input string.");
    }
    std::cerr << "XML parsing complete.\n";
    pugi::xml_node umison_node(local_document.first_child());
    if (!umison_node && umison_node.name() != UMI_XMLTAGS_UMISON) {
      std::cerr << "Missing parent umison node on the input string, aborting.\n"
      << umison_node.name() << "\n";
      throw std::runtime_error("Missing parent umison node on input string");
    }

    // Umison must have a minimum of one "json" node and possibly "class" nodes
    pugi::xml_node node_iterator(umison_node.first_child());
    while (node_iterator) {
      if (strncmp(UMI_XMLTAGS_JSON, node_iterator.name(), strlen(UMI_XMLTAGS_JSON)) == 0) {
        json_nodes.push_back(node_iterator);
      } else if (strncmp(UMI_XMLTAGS_CLASS, node_iterator.name(), strlen(UMI_XMLTAGS_CLASS)) == 0) {
        class_nodes.push_back(node_iterator);
      } else {
        std::cerr << "Unknown node on umison level: " << node_iterator.name() << " Ignoring it.\n";
      }
      node_iterator = node_iterator.next_sibling();
    }
    if (json_nodes.empty()) {
      std::cerr << "Missing at least one json node on the input string, aborting.\n";
      throw std::runtime_error("Missing at least one json node on the input string.");
    }

    std::cerr << "XML checking minimum requirements.\n";
    // Build class list
    for (auto &class_it : class_nodes) {
      auto class_node = std::make_shared<umi::umixmltypeclass>();
      if (umi_helper.read_class_node(class_node, class_it)) {
        m_classMap.push_back(std::make_pair(std::string(class_node->name()), class_node));
      }

    }
    // Build json list
    for (auto &json_it : json_nodes) {
      auto json_node = std::make_shared<umi::umixmltypeclass>();
      if (umi_helper.read_class_node(json_node, json_it)) {
        m_jsonArray.push_back(json_node);
      }
    }
    std::cerr << "Done.\nChecking references on Classes and Jsons.\n";

    // Check any possible missing reference on the classes or jsons
    for (auto &&class_it : m_classMap) {
      // Check children refclass & list
      const std::vector<std::shared_ptr<umixmltype>> &children = class_it.second->getChildren();
      for (auto &&child : children) {
        if (child->isRefClass() || child->isList()) {
          std::array<std::string, 1> arr{child->refclass()};
          auto classFound = std::search(m_classMap.begin(), m_classMap.end(),
                                        arr.begin(), arr.end(),
                                        [](const std::pair<std::string, std::shared_ptr<umi::umixmltypeclass>> &elem1,
                                           const std::string &elem2) {
                                          return elem1.first == elem2;
                                        });
          //m_classMap.find(child->refclass());
          if (classFound == m_classMap.end() && !umi_helper.is_internal(child->refclass())) {
            // Check if it is an internal type
            std::cerr << "Error wrong class referenced on class: " << class_it.first
            << " Refvalue: " << child->refclass() << " Aborting execution.\n";
            throw std::runtime_error("Error wrong class referenced on class.");
          }
        }
      }
    }
    for (auto &&class_it : m_jsonArray) {
      // Check children refclass & list
      const std::vector<std::shared_ptr<umixmltype>> &children = class_it->getChildren();
      for (auto &&child : children) {
        if (child->isRefClass() || child->isList()) {
          std::array<std::string, 1> arr{child->refclass()};
          auto classFound = std::search(m_classMap.begin(), m_classMap.end(),
                                        arr.begin(), arr.end(),
                                        [](const std::pair<std::string, std::shared_ptr<umi::umixmltypeclass>> &elem1,
                                           const std::string &elem2) {
                                          return elem1.first == elem2;
                                        });
          //auto classFound = m_classMap.find(child->refclass());
          if (classFound == m_classMap.end() && !umi_helper.is_internal(child->refclass())) {
            std::cerr << "Error wrong class referenced on json: " << class_it->name()
            << " Refvalue: " << child->refclass() << " Aborting execution.\n";
            throw std::runtime_error("Error wron class referenced within the json.");
          }
        }
      }
    }
    std::cerr << "Parsing complete.\nStarting to build output.\n";
  } catch (std::exception &ex) {
    std::cerr << "Error during the parsing: " << ex.what() << "\n";
    throw;
  }
}
umi::umixml::~umixml() {
}
