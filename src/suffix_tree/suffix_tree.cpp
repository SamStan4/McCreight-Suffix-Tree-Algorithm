#include "./suffix_tree.hpp"

suffix_tree_node* suffix_tree::node_hops(suffix_tree_node* node_ptr, std::pair<int32_t, int32_t> b) {
    suffix_tree_node* cur_ptr = node_ptr;
    int32_t i = 0;
    while (b.first <= b.second) {
        suffix_tree_node* cur_child_ptr = cur_ptr->m_child_ptr;
        while (cur_child_ptr && this->m_string[cur_child_ptr->m_start] != this->m_string[b.first]) {
            cur_child_ptr = cur_child_ptr->m_sibling_ptr;
        }
        if (!cur_child_ptr) {
            throw std::runtime_error("Error finding child pointer");
        }
        cur_ptr = cur_child_ptr;
        b.first += (cur_ptr->m_end - cur_ptr->m_start) + 1;
    }
    if (b.first == b.second + 1) {
        // No need for new internal node
        return cur_ptr;
    } else {
        // Needs new internal node, there is a split
        // pointer hell
        suffix_tree_node* new_internal_node_ptr = new suffix_tree_node();
        new_internal_node_ptr->m_start = cur_ptr->m_start;
        new_internal_node_ptr->m_end = cur_ptr->m_end - (b.first - b.second) + 1;
        new_internal_node_ptr->m_parent_ptr = cur_ptr->m_parent_ptr;
        new_internal_node_ptr->m_sibling_ptr = cur_ptr->m_sibling_ptr;
        new_internal_node_ptr->m_child_ptr = cur_ptr;
        if (cur_ptr->m_parent_ptr->m_child_ptr == cur_ptr) {
            cur_ptr->m_parent_ptr->m_child_ptr = new_internal_node_ptr;
        } else {
            suffix_tree_node* cur_child_ptr = cur_ptr->m_parent_ptr->m_child_ptr;
            while (cur_child_ptr) {
                if (cur_child_ptr->m_sibling_ptr == cur_ptr) {
                    cur_child_ptr->m_sibling_ptr = new_internal_node_ptr;
                    break;
                }
                cur_child_ptr = cur_child_ptr->m_sibling_ptr;
            }
            if (!cur_child_ptr) {
                throw std::runtime_error("Error finding sibiling in chain");
            }
        }
        cur_ptr->m_parent_ptr = new_internal_node_ptr;
        cur_ptr->m_start = new_internal_node_ptr->m_start + 1;
        return new_internal_node_ptr;
    }
}

void suffix_tree::build_suffix_tree_mccreight() {
}

suffix_tree::suffix_tree(const std::string& string, const std::string& alphabet) 
    : m_string(string), m_alphabet(alphabet), m_root_ptr(nullptr) {
    this->build_suffix_tree_mccreight();
}

suffix_tree::~suffix_tree() {
}