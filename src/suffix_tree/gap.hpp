#include "./suffix_tree_node.hpp"

struct gap {
public:
    int m_start;
    int m_end;
    gap(const suffix_tree_node* ptr);
    ~gap();
};