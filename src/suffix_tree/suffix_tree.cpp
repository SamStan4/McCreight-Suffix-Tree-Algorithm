#include "./suffix_tree.hpp"

suffix_tree::suffix_tree(const std::string& _alphabet, const std::string& _str)
    : m_root_ptr(new suffix_tree_node()), m_alphabet(_alphabet), m_str(_str) {
    this->m_root_ptr->m_parent_ptr = this->m_root_ptr;
    this->m_root_ptr->m_suffix_ptr = this->m_root_ptr;
    this->m_str.push_back('$');
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

void suffix_tree::print(std::ostream& os) {
    std::string str1;
    std::string str2;
    this->print_helper(os, this->m_root_ptr, str1, str2);
}

void suffix_tree::print_helper(std::ostream& os, suffix_tree_node* cur_ptr, std::string& prefix, std::string& suffix) {
    if (!cur_ptr) {
        return;
    }

    os << prefix;
    os << (!cur_ptr->m_sibling_ptr ? "└── " : "├── ");
    os << (cur_ptr->is_leaf() ? "[Leaf]" : "[Internal]") 
       << " (" << cur_ptr->m_start_idx << ", " << cur_ptr->m_size << ")";

    if (cur_ptr->is_leaf()) {
        os << " \"" << suffix << "\"";
    }

    os << std::endl;

    const size_t prefix_cur_size = prefix.size();
    const size_t suffix_cur_size = suffix.size();

    prefix += (!cur_ptr->m_sibling_ptr ? "    " : "│   ");

    suffix_tree_node* cur_child = cur_ptr->m_child_ptr;

    while (cur_child) {
        suffix += this->m_str.substr(cur_child->m_start_idx, cur_child->m_size);
        this->print_helper(os, cur_child, prefix, suffix);
        suffix.resize(suffix_cur_size);
        cur_child = cur_child->m_sibling_ptr;
    }

    prefix.resize(prefix_cur_size);
}

void suffix_tree::build_tree_mccreight() {
    const int32_t size = (int32_t)this->m_str.size();
    for (int32_t i = 0; i < size; ++i) {
        int32_t pos = 0;
        suffix_tree_node* internal_node_ptr = this->find_path(this->m_root_ptr, i, pos);
        suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
        new_leaf_ptr->m_parent_ptr = internal_node_ptr;
        new_leaf_ptr->m_sibling_ptr = internal_node_ptr->m_child_ptr;
        internal_node_ptr->m_child_ptr = new_leaf_ptr;
        new_leaf_ptr->m_start_idx = (internal_node_ptr == this->m_root_ptr ? i : pos);
        new_leaf_ptr->m_size = size - new_leaf_ptr->m_start_idx;
    }
}

suffix_tree_node* suffix_tree::find_path(suffix_tree_node* cur_ptr, int32_t idx, int32_t& pos) {
    int i = cur_ptr->m_start_idx;
    int j = 0;
    int k = idx;
    for (; j < cur_ptr->m_size && this->m_str[i] == this->m_str[k]; ++i, ++j, ++k);
    if (j == cur_ptr->m_size) {
        suffix_tree_node* next_ptr = cur_ptr->get_child_ptr(this->m_str, k);
        if (next_ptr == nullptr) {
            pos = k;
            return cur_ptr;
        } else {
            return this->find_path(next_ptr, k, pos);
        }
    } else {
        pos = k;

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
                    cur_child_ptr->m_sibling_ptr = new_internal_ptr;
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

int32_t suffix_tree::get_number_leaf_nodes(void) {

}

int32_t suffix_tree::get_number_internal_nodes(void) {

}