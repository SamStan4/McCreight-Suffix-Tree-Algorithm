#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cassert>

// TEMP
#include <cstring>

#include "./suffix_tree_node.hpp"

#define USE_NAIVE_ALG 0
#define SORT_TREE 1

class suffix_tree {
public:
  suffix_tree(const std::vector<std::pair<std::string, std::string>>&, const std::string&);
  ~suffix_tree();
  void print_tree(std::ostream&);
  std::pair<size_t, std::vector<size_t>> get_lrs(size_t);
  std::string get_bwt(const size_t);
  size_t get_num_internal_nodes(void) const;
  size_t get_num_leaf_nodes(void) const;
  size_t get_num_nodes(void) const;
  double get_avg_internal_node_depth(void) const;
  size_t get_deepest_internal_node_depth(void) const;
private:
  std::vector<std::pair<std::string, std::string>> m_strs;
  std::vector<const char*> m_c_strs;
  std::string m_alphabet;
  suffix_tree_node* m_root_ptr;
  int32_t m_id_pool;
  void sort_tree(void);
  void add_string(const size_t string_idx);
  void resolve_missing_suffix_link(suffix_tree_node*, const size_t);
  suffix_tree_node* find_path_and_insert(suffix_tree_node*, const char*, const size_t);
  suffix_tree_node* split_edge(suffix_tree_node*, size_t, const size_t);
  size_t get_string_size(const size_t) const;
};

#endif