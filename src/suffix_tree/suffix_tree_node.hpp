#ifndef __SUFFIX_TREE_NODE_HPP__
#define __SUFFIX_TREE_NODE_HPP__

#include <string>

class suffix_tree;

class suffix_tree_node {
public:
    suffix_tree_node();
    ~suffix_tree_node();

    suffix_tree_node* get_child_ptr(const std::string&, int32_t);
private:
    suffix_tree_node* m_child_ptr;
    suffix_tree_node* m_sibling_ptr;
    suffix_tree_node* m_parent_ptr;
    suffix_tree_node* m_suffix_ptr;

    int32_t m_start_idx;
    int32_t m_size;

    friend class suffix_tree;
};

#endif