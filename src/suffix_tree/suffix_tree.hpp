#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include <string>
#include <functional>
#include "./suffix_tree_node.hpp"

class suffix_tree {
public:
  suffix_tree(const std::string& _str, const std::string& _alphabet);
  ~suffix_tree();
private:
  // This is a pointer to the root node of the tree
  suffix_tree_node* m_root_ptr;

  // This is the string that the suffix tree is being made from
  std::string m_str;

  // This is the alphabet that the string is being made from
  std::string m_alphabet;
};

#endif