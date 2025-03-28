#include "./gap.hpp"

gap::gap(const suffix_tree_node* ptr)
    : m_start(ptr->m_start), m_end(ptr->m_end) {}

gap::~gap() {}