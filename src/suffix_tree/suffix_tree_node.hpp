#ifndef __SUFFIX_TREE_NODE_HPP__
#define __SUFFIX_TREE_NODE_HPP__

#include <string>
#include <algorithm>

class suffix_tree;

class suffix_tree_node {
public:
    suffix_tree_node();
    ~suffix_tree_node();

    suffix_tree_node* get_child_ptr(const std::string&, int32_t);

    suffix_tree_node* get_child_ptr(const std::string& s1, const std::string& s2, int32_t idx);

    bool is_leaf(void);

private:
    suffix_tree_node* m_child_ptr;
    suffix_tree_node* m_sibling_ptr;
    suffix_tree_node* m_parent_ptr;
    suffix_tree_node* m_suffix_ptr;

    int32_t m_start_idx;
    int32_t m_size;
    int32_t m_depth;

    int32_t id;

    friend class suffix_tree;

    void update_depth(void);
};

#endif