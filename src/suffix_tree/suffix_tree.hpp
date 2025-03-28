#ifndef __SUFFIX_TREE_HPP__
#define __SUFFIX_TREE_HPP__

#include <iostream>
#include "./suffix_tree_node.hpp"

class suffix_tree {
public:
    suffix_tree(const std::string&, const std::string&);
    ~suffix_tree();
    void print(std::ostream& os);

    int32_t get_number_leaf_nodes(void);
    int32_t get_number_internal_nodes(void);
    size_t get_str_size(void);
private:
    suffix_tree_node* m_root_ptr;
    std::string m_alphabet;
    std::string m_str;

    void print_helper(std::ostream& os, suffix_tree_node* cur_ptr, std::string& prefix, std::string& suffix);

    void destructor_helper(suffix_tree_node* cur_ptr);

    void build_tree_mccreight();

    suffix_tree_node* find_path_and_insert(suffix_tree_node* cur_ptr, int32_t idx);

    void get_number_leaf_nodes_helper(suffix_tree_node* cur_ptr, int32_t& leaf_node_count);

    void get_number_internal_nodes_helper(suffix_tree_node* cur_ptr, int32_t& internal_node_count);

    bool compare_indicies(int32_t i, int32_t j) const;

    // node hops
    void resolve_missing_suffix_link(suffix_tree_node* start_ptr);
};

#endif