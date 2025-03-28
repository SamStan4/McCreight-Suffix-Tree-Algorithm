#ifndef __SUFFIX_TREE_NODE_HPP__
#define __SUFFIX_TREE_NODE_HPP__

#include <string>

class suffix_tree;

class suffix_tree_node {
private:
    suffix_tree_node* m_child_ptr;
    suffix_tree_node* m_sibling_ptr;
    suffix_tree_node* m_parent_ptr;
    suffix_tree_node* m_suffix_ptr;

    int32_t m_start;
    int32_t m_end;

    friend class suffix_tree;
public:
    suffix_tree_node(
        suffix_tree_node* _child_ptr = nullptr,
        suffix_tree_node* _sibling_ptr = nullptr,
        suffix_tree_node* _parent_ptr = nullptr,
        suffix_tree_node* _suffix_ptr = nullptr,
        int32_t _start = -1,
        int32_t _end = -1
    );
    ~suffix_tree_node();
};


#endif