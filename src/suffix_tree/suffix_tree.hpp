#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include "./suffix_tree_node.hpp"

class suffix_tree {
private:
    suffix_tree_node* m_root_ptr;

    std::string m_string;
    std::string m_alphabet;

    void build_suffix_tree_mccreight();
    suffix_tree_node* node_hops(suffix_tree_node* node_ptr, std::pair<int32_t, int32_t> b);
    suffix_tree_node* find_path(suffix_tree_node* node_ptr, std::pair<int32_t, int32_t>& g, suffix_tree_node*& p);
public:
    suffix_tree(const std::string&, const std::string&);
    ~suffix_tree();
};

#endif