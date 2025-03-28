#include "./suffix_tree_node.hpp"

suffix_tree_node::suffix_tree_node(size_t alphabet_size)
    : m_parent_ptr(nullptr), m_suffix_ptr(nullptr), m_start(-1), m_end(-1) {
    this->m_children_ptrs = new suffix_tree_node*[alphabet_size];
}

suffix_tree_node::~suffix_tree_node() {
    delete []this->m_children_ptrs;
}