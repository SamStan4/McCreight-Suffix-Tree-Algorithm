#include "./suffix_tree.hpp"

/**
 * Constructor, nothing special here
 */
suffix_tree::suffix_tree(const std::string& _str, const std::string& _alphabet)
  : m_root_ptr(new suffix_tree_node()), m_str(_str), m_alphabet(_alphabet) {
  // Make root's parent pointer itself
  this->m_root_ptr->m_parent_ptr = this->m_root_ptr;

  // Make root's suffix link itself
  this->m_root_ptr->m_suffix_link_ptr = this->m_root_ptr;

  std::cout << "inside constructor" << std::endl;
}

// Destructor, needs recursion
suffix_tree::~suffix_tree() {
  // Recursive function for deleting all nodes in the suffix tree
  const std::function<void(suffix_tree_node*)> release = [&release](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    release(cur_ptr->m_child_ptr);
    release(cur_ptr->m_next_sibling_ptr);
    delete cur_ptr;
  };
  release(this->m_root_ptr);
  this->m_root_ptr = nullptr;
}