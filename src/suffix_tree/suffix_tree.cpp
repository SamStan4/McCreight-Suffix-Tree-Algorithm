#include "./suffix_tree.hpp"

suffix_tree::suffix_tree(const std::string& _alphabet, const std::string& _str)
    : m_root_ptr(new suffix_tree_node()), cur_leaf_idx(0), m_alphabet(_alphabet), m_str(_str) {
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
        os << " \"" << this->get_string(cur_ptr) << "\"";
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
    const int32_t str_size = (int32_t)this->m_str.size();
    suffix_tree_node* internal_ptr = this->m_root_ptr;
    for (int32_t i = 0; i < str_size; ++i) {
        // if (!internal_ptr->m_suffix_ptr) {
        //     this->resolve_missing_suffix_link(internal_ptr);
        // }
        internal_ptr = this->find_path_and_insert(this->m_root_ptr, i);
    }
}

suffix_tree_node* suffix_tree::find_path_and_insert(suffix_tree_node* cur_ptr, int32_t idx) {
    int32_t i = cur_ptr->m_start_idx; // this is the position in the tree in which we are comparing with
    int32_t j = idx; // this is the position in the string in which we are comparing with
    int32_t k = 0; // this is the number of characters that were the same
    while (k < cur_ptr->m_size && this->compare_indicies(i, j)) {
        ++i;
        ++j;
        ++k;
    }
    if (k == cur_ptr->m_size) {
        suffix_tree_node* next_ptr = cur_ptr->get_child_ptr(this->m_str, j);
        if (next_ptr) {
            return this->find_path_and_insert(next_ptr, j);
        } else {
            suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
            new_leaf_ptr->id = this->cur_leaf_idx++;
            new_leaf_ptr->m_start_idx = j;
            new_leaf_ptr->m_size = (int32_t)this->m_str.size() - j;
            new_leaf_ptr->m_parent_ptr = cur_ptr;
            new_leaf_ptr->m_sibling_ptr = cur_ptr->m_child_ptr;
            cur_ptr->m_child_ptr = new_leaf_ptr;
            new_leaf_ptr->update_depth();
            return cur_ptr;
        }
    } else {
        suffix_tree_node* new_internal_ptr = new suffix_tree_node();
        suffix_tree_node* new_leaf_ptr = new suffix_tree_node();

        new_leaf_ptr->id = this->cur_leaf_idx++;

        // link internal pointer first

        new_internal_ptr->m_start_idx = cur_ptr->m_start_idx;
        new_internal_ptr->m_size = k;

        cur_ptr->m_start_idx += k;
        cur_ptr->m_size -= k;

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

        // link in leaf pointer second

        new_leaf_ptr->m_start_idx = j;
        new_leaf_ptr->m_size = (int32_t)this->m_str.size() - j;

        cur_ptr->m_sibling_ptr = new_leaf_ptr;
        new_leaf_ptr->m_parent_ptr = new_internal_ptr;

        new_internal_ptr->update_depth();
        new_leaf_ptr->update_depth();

        return new_internal_ptr;
    }
}

// this is the nodehops function, didnt name it that
void suffix_tree::resolve_missing_suffix_link(suffix_tree_node* start_ptr) {
    suffix_tree_node* parent_ptr = start_ptr->m_parent_ptr;
    if (!parent_ptr->m_suffix_ptr) {
        this->resolve_missing_suffix_link(parent_ptr);
    }
    suffix_tree_node* cur_ptr = parent_ptr->m_suffix_ptr;
    suffix_tree_node* cur_ptr_2 = parent_ptr;

    // I cant solve this
    int32_t i = parent_ptr->m_start_idx + 1; // + 1 because we dont care about the first character
    int32_t j = start_ptr->m_start_idx + start_ptr->m_size; // the index we need to stop at
    int32_t k = 0;

    // The actual hopping down, guess this is why its called node hops
    while (i < j) {
        cur_ptr = cur_ptr->get_child_ptr(this->m_str, i);
        i += cur_ptr->m_size;
    }

    if (i == j) {
        // The suffix link already existed, and we just found it :)
        start_ptr->m_suffix_ptr = cur_ptr;
    } else {
        // We need to break the edge to make the suffix link
        suffix_tree_node* new_internal_ptr = new suffix_tree_node();

        // link up the new internal pointer

        new_internal_ptr->m_parent_ptr = cur_ptr->m_parent_ptr;
        new_internal_ptr->m_sibling_ptr = cur_ptr->m_sibling_ptr;
        new_internal_ptr->m_child_ptr = cur_ptr;

        // link up the current pointer

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

        // update numeric values

        const int32_t difference = i - j;

        new_internal_ptr->m_start_idx = cur_ptr->m_start_idx;
        new_internal_ptr->m_size = cur_ptr->m_size - difference;
        cur_ptr->m_size = difference;
        cur_ptr->m_start_idx = j;
        new_internal_ptr->update_depth();

        start_ptr->m_suffix_ptr = new_internal_ptr;
    }
}

int32_t suffix_tree::get_number_leaf_nodes(void) {
    int32_t count = 0;
    this->get_number_leaf_nodes_helper(this->m_root_ptr, count);
    return count;
}

int32_t suffix_tree::get_number_internal_nodes(void) {
    int32_t count = 0;
    this->get_number_internal_nodes_helper(this->m_root_ptr, count);
    return count;
}

void suffix_tree::get_number_leaf_nodes_helper(suffix_tree_node* cur_ptr, int32_t& leaf_node_count) {
    if (!cur_ptr) {
        return;
    }

    if (cur_ptr->is_leaf()) {
        ++leaf_node_count;
    }

    this->get_number_leaf_nodes_helper(cur_ptr->m_sibling_ptr, leaf_node_count);
    this->get_number_leaf_nodes_helper(cur_ptr->m_child_ptr, leaf_node_count);
}

void suffix_tree::get_number_internal_nodes_helper(suffix_tree_node* cur_ptr, int32_t& internal_node_count) {
    if (!cur_ptr) {
        return;
    }

    if (!cur_ptr->is_leaf()) {
        ++internal_node_count;
    }

    this->get_number_internal_nodes_helper(cur_ptr->m_sibling_ptr, internal_node_count);
    this->get_number_internal_nodes_helper(cur_ptr->m_child_ptr, internal_node_count);
}

size_t suffix_tree::get_str_size(void) {
    return this->m_str.size();
}

bool suffix_tree::compare_indicies(int32_t i, int32_t j) const {
    if (i >= (int32_t)this->m_str.size() || j >= (int32_t)this->m_str.size()) {
        throw std::runtime_error("error in compare_indicies");
    }
    return this->m_str[i] == this->m_str[j];
}

void suffix_tree::print_ptrs(std::ostream& os) {
    std::string str1;
    std::string str2;
    this->print_ptrs_helper(os, this->m_root_ptr, str1, str2);

}

void suffix_tree::print_ptrs_helper(std::ostream& os, suffix_tree_node* cur_ptr, std::string& prefix, std::string& suffix) {
    if (!cur_ptr) {
        return;
    }

    os << prefix;
    os << (!cur_ptr->m_sibling_ptr ? "└── " : "├── ");
    os << (cur_ptr->is_leaf() ? "[Leaf]" : "[Internal]") 
       << " <cur(" << cur_ptr << "), parent(" << cur_ptr->m_parent_ptr << "), child("
       << cur_ptr->m_child_ptr << "), sibling(" << cur_ptr->m_sibling_ptr << "), sl("
       << cur_ptr->m_suffix_ptr << ")> (" << cur_ptr->m_start_idx << ", " << cur_ptr->m_size << ", " << cur_ptr->m_depth << ")";

    // if (cur_ptr->is_leaf()) {
        os << " \"" << suffix << "\"";
    // }

    os << std::endl;    

    const size_t prefix_cur_size = prefix.size();
    const size_t suffix_cur_size = suffix.size();

    prefix += (!cur_ptr->m_sibling_ptr ? "    " : "│   ");

    suffix_tree_node* cur_child = cur_ptr->m_child_ptr;

    while (cur_child) {
        suffix += this->m_str.substr(cur_child->m_start_idx, cur_child->m_size);
        this->print_ptrs_helper(os, cur_child, prefix, suffix);
        suffix.resize(suffix_cur_size);
        cur_child = cur_child->m_sibling_ptr;
    }

    prefix.resize(prefix_cur_size);
}

std::string suffix_tree::get_string(suffix_tree_node* ptr) {
    suffix_tree_node* cur_ptr = ptr;
    std::string str;
    while (cur_ptr != this->m_root_ptr) {
        std::string substr = this->m_str.substr(cur_ptr->m_start_idx, cur_ptr->m_size);
        std::reverse(substr.begin(), substr.end());
        str += substr;
        cur_ptr = cur_ptr->m_parent_ptr;
    }
    std::reverse(str.begin(), str.end());
    return str;
}

void suffix_tree::write_BWT(std::ostream& os) {
    std::vector<suffix_tree_node*> leaf_ptrs;
    this->get_leaf_ptrs(this->m_root_ptr, leaf_ptrs);
    for (size_t i = 0; i < leaf_ptrs.size(); ++i) {
        if (leaf_ptrs[i]->id == 0) {
            os << "$\n";
        } else {
            os << this->m_str[leaf_ptrs[i]->id - 1] << std::endl;
        }
    }
}

void suffix_tree::get_leaf_ptrs(suffix_tree_node* cur_ptr, std::vector<suffix_tree_node*>& lst) {
    if (!cur_ptr) {
        return;
    } else if (cur_ptr->is_leaf()) {
        lst.push_back(cur_ptr);
        return;
    }

    std::vector<suffix_tree_node*> ptrs;
    suffix_tree_node* cur_child = cur_ptr->m_child_ptr;
    while (cur_child) {
        ptrs.push_back(cur_child);
        cur_child = cur_child->m_sibling_ptr;
    }

    std::sort(ptrs.begin(), ptrs.end(), [this](suffix_tree_node* p1, suffix_tree_node* p2) {
        if (this->m_str[p1->m_start_idx] == '$') {
            return true;
        } else if (this->m_str[p1->m_start_idx] == '$') {
            return false;
        }
        return this->m_str[p1->m_start_idx] < this->m_str[p2->m_start_idx];
    });

    for (auto ptr : ptrs) {
        this->get_leaf_ptrs(ptr, lst);
    }
}