#ifndef __SUFFIX_TREE_NODE_HPP__
#define __SUFFIX_TREE_NODE_HPP__

#include <string>

class suffix_tree;

class suffix_tree_node {
public:
  suffix_tree_node();
  ~suffix_tree_node();
  std::string get_string(const std::string& str);
private:
  // A pointer to the suffix tree node's parent node
  suffix_tree_node* m_parent_ptr;

  // A pointer to the first child of the suffix tree node
  suffix_tree_node* m_child_ptr;

  // A pointer to the suffix tree node's next sibling
  // NULL if the node is the last in the level
  suffix_tree_node* m_next_sibling_ptr;

  // A pointer to the suffix tree node's previous sibling
  // NULL if the node is the first node in the level
  suffix_tree_node* m_prev_sibling_ptr;

  // A pointer to the node's suffix link
  suffix_tree_node* m_suffix_link_ptr;

  // The start index in the string that this node represents
  size_t m_start_idx;

  // The size of the substring that the node represents
  size_t m_size;

  // How deep this node is in the tree (size of the substring that it represents all the way to root)
  size_t m_depth;

  // This id is for leaf nodes only
  int32_t m_id;

  // Friend so that we dont need to make a bunch of setters and getters
  friend class suffix_tree;
};

#endif