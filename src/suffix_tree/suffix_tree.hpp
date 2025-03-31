#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include <string>
#include <functional>
#include <cassert>
#include "./suffix_tree_node.hpp"

#define USE_NAIVE_ALG false

class suffix_tree {
public:
  suffix_tree(const std::string& _str, const std::string& _alphabet);
  ~suffix_tree();
  void print_tree(std::ostream&);
private:
  // This is a pointer to the root node of the tree
  suffix_tree_node* m_root_ptr;

  // This is the string that the suffix tree is being made from
  std::string m_str;

  // This is the alphabet that the string is being made from
  std::string m_alphabet;

  // This is the pool of leaf node ID values that we are going to have
  int32_t m_leaf_id_pool;

  void build_tree();

  suffix_tree_node* find_path_and_insert(suffix_tree_node*, size_t);

  void resolve_missing_suffix_link(suffix_tree_node*);

  suffix_tree_node* split_edge(suffix_tree_node*, size_t);
};

#endif