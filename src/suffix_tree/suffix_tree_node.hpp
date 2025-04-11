#ifndef __SUFFIX_TREE_NODE_HPP__
#define __SUFFIX_TREE_NODE_HPP__

#include <string>
#include <algorithm>
#include <functional>
#include <set>

class suffix_tree;

class suffix_tree_node {
public:
  suffix_tree_node();
  ~suffix_tree_node();
  std::string get_string();
  std::string get_complete_string();
  suffix_tree_node* find_child(const char* const);
  suffix_tree_node* find_child(const char);
  void set_depth();
  bool is_visited_by(size_t);
private:
  suffix_tree_node* m_parent_ptr;
  suffix_tree_node* m_child_ptr;
  suffix_tree_node* m_next_sib_ptr;
  suffix_tree_node* m_prev_sib_ptr;
  suffix_tree_node* m_suffix_link_ptr;
  char const* m_start;
  size_t m_size;
  size_t m_depth;
  int32_t m_id;
  std::set<size_t> m_visitors;
  void sort_children();
  friend class suffix_tree;
};

#endif