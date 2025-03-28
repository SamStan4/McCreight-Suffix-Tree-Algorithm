#include "./suffix_tree_node.hpp"

suffix_tree_node::suffix_tree_node(
    suffix_tree_node* _child_ptr,
    suffix_tree_node* _sibling_ptr,
    suffix_tree_node* _parent_ptr,
    suffix_tree_node* _suffix_ptr,
    int32_t _start,
    int32_t _end
    ) : m_child_ptr(_child_ptr), m_sibling_ptr(_sibling_ptr),
      m_parent_ptr(_parent_ptr), m_suffix_ptr(_suffix_ptr),
      m_start(_start), m_end(_end) {}

suffix_tree_node::~suffix_tree_node() {}