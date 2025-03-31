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

  this->build_tree();
}

/**
 * Destructor, needs recursion
 */
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

void suffix_tree::print_tree(std::ostream& os) {
  std::string prefix;
  std::string suffix;
  const std::function<void(suffix_tree_node*)> print_helper = [&print_helper, &os, &prefix, &suffix, this](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    os
      << prefix
      << (!cur_ptr->m_next_sibling_ptr ? "└── " : "├── ")
      << (!cur_ptr->m_child_ptr ? "[Leaf]" : "[Internal]")
      << " (" << cur_ptr->m_start_idx << ", " << cur_ptr->m_size << ", " << cur_ptr->m_depth << ")";
    if (!cur_ptr->m_child_ptr) {
      // os << " \"" << suffix << "\"";
    }
    os << std::endl;
    const size_t prefix_size = prefix.size();
    const size_t suffix_size = suffix.size();
    prefix += (cur_ptr->m_next_sibling_ptr ? "│   " : "    ");
    cur_ptr = cur_ptr->m_child_ptr;
    while (cur_ptr) {
      suffix += cur_ptr->get_string(this->m_str);
      print_helper(cur_ptr);
      suffix.resize(suffix_size);
      cur_ptr = cur_ptr->m_next_sibling_ptr;
    }
    prefix.resize(prefix_size);
  };
  print_helper(this->m_root_ptr);
}

/**
 * Iterates through all suffixes and inserts them
 */
void suffix_tree::build_tree() {
  const size_t str_size = this->m_str.size();

  #if !USE_NAIVE_ALG
    // This is going to be used for suffix links
    suffix_tree_node* internal_ptr = this->m_root_ptr;
  #endif

  for (size_t i = 0; i < str_size; ++i) {
    #if USE_NAIVE_ALG
      this->find_path_and_insert(this->m_root_ptr, i);
    #else
      if (!internal_ptr->m_suffix_link_ptr)
        this->resolve_missing_suffix_link(internal_ptr);
      internal_ptr = this->find_path_and_insert(internal_ptr->m_suffix_link_ptr, i + internal_ptr->m_suffix_link_ptr->m_depth);
    #endif
  }
}

/**
 * A modified implementation of Anath's find path algorithm as discussed in class
 */
suffix_tree_node* suffix_tree::find_path_and_insert(suffix_tree_node* cur_ptr, size_t idx) {
  assert(cur_ptr != nullptr && "FATAL ERROR, cur_ptr was null in find_path_and_insert");
  if (!cur_ptr) {
    return nullptr;
  }
  size_t i = cur_ptr->m_start_idx;
  size_t j = idx;
  size_t k = 0;
  while (k < cur_ptr->m_size && this->m_str[i] == this->m_str[j]) {
    ++i; ++j; ++k;
  }
  if (k == cur_ptr->m_size) {
    suffix_tree_node* next_ptr = cur_ptr->find_child(this->m_str, j);
    if (next_ptr) {
      return this->find_path_and_insert(next_ptr, j);
    } else {
      suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
      new_leaf_ptr->m_id = this->m_leaf_id_pool++;
      new_leaf_ptr->m_start_idx = j;
      new_leaf_ptr->m_size = this->m_str.size() - j;
      new_leaf_ptr->m_parent_ptr = cur_ptr;
      if (cur_ptr->m_child_ptr)
        cur_ptr->m_child_ptr->m_prev_sibling_ptr = new_leaf_ptr;
      new_leaf_ptr->m_next_sibling_ptr = cur_ptr->m_child_ptr;
      cur_ptr->m_child_ptr = new_leaf_ptr;
      new_leaf_ptr->set_depth();
      return cur_ptr;
    }
  } else {
    suffix_tree_node* new_internal_ptr = this->split_edge(cur_ptr, k);
    suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
    new_leaf_ptr->m_id = this->m_leaf_id_pool++;
    new_leaf_ptr->m_start_idx = j;
    new_leaf_ptr->m_size = this->m_str.size() - j;
    cur_ptr->m_next_sibling_ptr = new_leaf_ptr;
    new_leaf_ptr->m_prev_sibling_ptr = cur_ptr;
    new_leaf_ptr->m_parent_ptr = new_internal_ptr;
    new_leaf_ptr->set_depth();
    return new_internal_ptr;
  }
}

void suffix_tree::resolve_missing_suffix_link(suffix_tree_node* start_ptr) {
  assert(start_ptr != nullptr && "FATAL ERROR, start_ptr was null in resolve_missing_suffix_link");
  if (!start_ptr) {
    return;
  }
}

/**
 * Splits the edge, go figure
 * The doubly linked list makes this an O(1) function :)
 * returns a pointer to the newly created internal node
 * 
 * this function looks scary
 */
suffix_tree_node* suffix_tree::split_edge(suffix_tree_node* cur_ptr, size_t i) {
  suffix_tree_node* new_internal_ptr = new suffix_tree_node();
  new_internal_ptr->m_start_idx = cur_ptr->m_start_idx;
  new_internal_ptr->m_size = i;
  cur_ptr->m_start_idx += i;
  cur_ptr->m_size -= i;
  new_internal_ptr->m_child_ptr = cur_ptr;
  new_internal_ptr->m_next_sibling_ptr = cur_ptr->m_next_sibling_ptr;
  new_internal_ptr->m_prev_sibling_ptr = cur_ptr->m_prev_sibling_ptr;
  new_internal_ptr->m_parent_ptr = cur_ptr->m_parent_ptr;
  if (cur_ptr->m_parent_ptr->m_child_ptr == cur_ptr)
    cur_ptr->m_parent_ptr->m_child_ptr = new_internal_ptr;
  if (cur_ptr->m_next_sibling_ptr)
    cur_ptr->m_next_sibling_ptr->m_prev_sibling_ptr = new_internal_ptr;
  if (cur_ptr->m_prev_sibling_ptr)
    cur_ptr->m_prev_sibling_ptr->m_next_sibling_ptr = new_internal_ptr;
  cur_ptr->m_parent_ptr = new_internal_ptr;
  cur_ptr->m_prev_sibling_ptr = nullptr;
  cur_ptr->m_next_sibling_ptr = nullptr;
  cur_ptr->set_depth();
  return new_internal_ptr;
}