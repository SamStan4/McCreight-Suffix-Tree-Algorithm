#include "./suffix_tree_node.hpp"

suffix_tree_node::suffix_tree_node()
    : m_child_ptr(nullptr), m_sibling_ptr(nullptr),
      m_parent_ptr(nullptr), m_suffix_ptr(nullptr),
      m_start_idx(0), m_size(0) {}

suffix_tree_node::~suffix_tree_node() {}

suffix_tree_node* suffix_tree_node::get_child_ptr(const std::string& str, int32_t idx) {
    const char target_char = str[idx];
    suffix_tree_node* cur_child = this->m_child_ptr;
    while (cur_child) {
        if (str[cur_child->m_start_idx] == target_char) {
            break;
        }
        cur_child = cur_child->m_sibling_ptr;
    }
    return cur_child;
}