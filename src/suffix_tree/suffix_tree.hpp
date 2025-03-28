#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include "./suffix_tree_node.hpp"

class suffix_tree {
public:
    suffix_tree(const std::string&, const std::string&);
    ~suffix_tree();
private:
    suffix_tree_node* m_root_ptr;
    std::string m_alphabet;
    std::string m_str;

    void destructor_helper(suffix_tree_node* cur_ptr);

    void build_tree_mccreight();

    suffix_tree_node* find_path(suffix_tree_node* cur_ptr, int32_t idx);
};

#endif