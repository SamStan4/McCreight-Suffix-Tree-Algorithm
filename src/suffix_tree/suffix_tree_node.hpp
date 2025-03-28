#ifndef __SUFFIX_TREE_NODE_HPP__
#define __SUFFIX_TREE_NODE_HPP__

#include <string>

class suffix_tree;

struct gap;

class suffix_tree_node {
private:
    suffix_tree_node** m_children_ptrs;
    suffix_tree_node* m_parent_ptr;
    suffix_tree_node* m_suffix_ptr;

    int m_start;
    int m_end;

    friend class suffix_tree;
    friend struct gap;
public:
    suffix_tree_node(size_t alphabet_size);
    ~suffix_tree_node();
};

#endif