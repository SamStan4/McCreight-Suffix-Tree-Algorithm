#include "./suffix_tree.hpp"

suffix_tree::suffix_tree(const std::string& _alphabet, const std::string& _str)
    : m_alphabet(_alphabet), m_str(_str), m_root_ptr(new suffix_tree_node()) {
    this->m_root_ptr->m_parent_ptr = this->m_root_ptr;
    this->m_root_ptr->m_suffix_ptr = this->m_root_ptr;
    std::cout << "here" << std::endl;
    this->build_tree_mccreight();
}

suffix_tree::~suffix_tree() {
    this->destructor_helper(this->m_root_ptr);
    this->m_root_ptr = nullptr;
}

void suffix_tree::destructor_helper(suffix_tree_node* cur_ptr) {
    if (!cur_ptr) {
        return;
    }
    this->destructor_helper(cur_ptr->m_sibling_ptr);
    this->destructor_helper(cur_ptr->m_child_ptr);
    delete cur_ptr;
}

void suffix_tree::build_tree_mccreight() {
    const int32_t size = (int32_t)this->m_str.size();
    for (int32_t i = 0; i < size; ++i) {
        suffix_tree_node* internal_node_ptr = this->find_path(this->m_root_ptr, i);
        suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
        new_leaf_ptr->m_parent_ptr = internal_node_ptr;
        new_leaf_ptr->m_sibling_ptr = internal_node_ptr->m_child_ptr;
        internal_node_ptr->m_child_ptr = new_leaf_ptr;
        new_leaf_ptr->m_start_idx = internal_node_ptr->m_start_idx + internal_node_ptr->m_size;
        new_leaf_ptr->m_size = size - new_leaf_ptr->m_start_idx;
    }
}

suffix_tree_node* suffix_tree::find_path(suffix_tree_node* cur_ptr, int32_t idx) {
    int i = cur_ptr->m_start_idx;
    int j = 0;
    int k = idx;
    for (; j < cur_ptr->m_size && this->m_str[i] == this->m_str[k]; ++i, ++j, ++k);
    if (j == cur_ptr->m_size) {
        suffix_tree_node* next_ptr = cur_ptr->get_child_ptr(this->m_str, k);
        if (next_ptr == nullptr) {
            return cur_ptr;
        } else {
            return this->find_path(next_ptr, k);
        }
    } else {
        suffix_tree_node* new_internal_ptr = new suffix_tree_node();

        new_internal_ptr->m_start_idx = cur_ptr->m_start_idx;
        new_internal_ptr->m_size = j;

        cur_ptr->m_start_idx += j;
        cur_ptr->m_size -= j;

        new_internal_ptr->m_child_ptr = cur_ptr;
        new_internal_ptr->m_sibling_ptr = cur_ptr->m_sibling_ptr;
        new_internal_ptr->m_parent_ptr = cur_ptr->m_parent_ptr;

        if (cur_ptr->m_parent_ptr->m_child_ptr == cur_ptr) {
            cur_ptr->m_parent_ptr->m_child_ptr = new_internal_ptr;
        } else {
            suffix_tree_node* cur_child_ptr = cur_ptr->m_parent_ptr->m_child_ptr;
            while (cur_child_ptr) {
                if (cur_child_ptr->m_sibling_ptr == cur_ptr) {
                    cur_child_ptr->m_child_ptr = new_internal_ptr;
                    break;
                }
                cur_child_ptr = cur_child_ptr->m_sibling_ptr;
            }
        }

        cur_ptr->m_parent_ptr = new_internal_ptr;
        cur_ptr->m_sibling_ptr = nullptr;

        return new_internal_ptr;
    }
}