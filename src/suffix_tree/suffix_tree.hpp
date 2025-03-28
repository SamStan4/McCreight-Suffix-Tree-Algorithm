#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include "./suffix_tree_node.hpp"

class suffix_tree {
private:
    suffix_tree_node* m_root_ptr;

    std::string m_string;
    std::string m_alphabet;

    void build_sufix_tree_mccreight();
public:
    suffix_tree(const std::string&, const std::string&);
    ~suffix_tree();
};

#endif