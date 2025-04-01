#include "./suffix_tree_node.hpp"

// MARK - Public Methods

/**
 * Constructor for the suffix tree node, nothing special here
 */
suffix_tree_node::suffix_tree_node()
  : m_parent_ptr(nullptr), m_child_ptr(nullptr), m_next_sibling_ptr(nullptr), m_prev_sibling_ptr(nullptr), m_suffix_link_ptr(nullptr),
  m_start_idx(0), m_size(0), m_depth(0), m_id(-1) {
}

/**
 * Destructor for the suffix tree node, nothing happens here
 */
suffix_tree_node::~suffix_tree_node() {
}

/**
 * Returns the substring that this node represents
 * @param str this is the string that the suffix tree is being made from
 */
std::string suffix_tree_node::get_string(const std::string& str) {
  return str.substr(this->m_start_idx, this->m_size);
}

/**
 * Returns the complete string that the node represets
 */
std::string suffix_tree_node::get_complete_string(const std::string& str) {
  std::string result;
  suffix_tree_node* cur_ptr = this;
  suffix_tree_node* prev_ptr = nullptr;
  while (cur_ptr && cur_ptr != prev_ptr) {
    std::string substr = cur_ptr->get_string(str);
    std::reverse(substr.begin(), substr.end());
    result += substr;
    prev_ptr = cur_ptr;
    cur_ptr = cur_ptr->m_parent_ptr;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

/**
 * Finds the child that we want
 */
suffix_tree_node* suffix_tree_node::find_child(const std::string& str, int idx) {
  suffix_tree_node* cur_ptr = this->m_child_ptr;
  while (cur_ptr) {
    if (str[idx] == str[cur_ptr->m_start_idx]) {
      break;
    }
    cur_ptr = cur_ptr->m_next_sibling_ptr;
  }
  return cur_ptr;
}

suffix_tree_node* suffix_tree_node::find_child_chr(const std::string& str, const char child_key) {
  suffix_tree_node* cur_ptr = this->m_child_ptr;
  while (cur_ptr) {
    if (str[cur_ptr->m_start_idx] == child_key) {
      break;
    }
    cur_ptr = cur_ptr->m_next_sibling_ptr;
  }
  return cur_ptr;
}

/**
 * !WARNING parent pointer must be correct and its size must be correct to use this
 */
void suffix_tree_node::set_depth() {
  this->m_depth = this->m_parent_ptr->m_depth + this->m_size;
}

// MARK - Private Methods