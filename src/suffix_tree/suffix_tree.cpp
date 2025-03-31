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
      os << " \"" << suffix << "\"";
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

void suffix_tree::advanced_print_tree(std::ostream& os) {
  std::string prefix;
  std::string suffix;
  const std::function<void(suffix_tree_node*)> advanced_print_helper = [&advanced_print_helper, &os, &prefix, &suffix, this](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    os
      << prefix
      << (!cur_ptr->m_next_sibling_ptr ? "└── " : "├── ")
      << (!cur_ptr->m_child_ptr ? "[Leaf]" : "[Internal]")
      << " (" << cur_ptr->m_start_idx << ", " << cur_ptr->m_size << ", " << cur_ptr->m_depth
      << ") <self(" << cur_ptr << "), parent(" << cur_ptr->m_parent_ptr << "), child(" << cur_ptr->m_child_ptr
      << "), prev_sib(" << cur_ptr->m_prev_sibling_ptr << "), next_sib(" << cur_ptr->m_next_sibling_ptr << "), suffix_link(" << cur_ptr->m_suffix_link_ptr << ")>"
      << " \"" << suffix << "\"" << std::endl;
    const size_t prefix_size = prefix.size();
    const size_t suffix_size = suffix.size();
    prefix += (cur_ptr->m_next_sibling_ptr ? "│   " : "    ");
    cur_ptr = cur_ptr->m_child_ptr;
    while (cur_ptr) {
      suffix += cur_ptr->get_string(this->m_str);
      advanced_print_helper(cur_ptr);
      suffix.resize(suffix_size);
      cur_ptr = cur_ptr->m_next_sibling_ptr;
    }
    prefix.resize(prefix_size);
  };
  advanced_print_helper(this->m_root_ptr);
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
    // this->advanced_print_tree(std::cout);
    // std::cout << "\n\n";
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
    suffix_tree_node* next_ptr = cur_ptr->find_child(this->m_str, (size_t)j);
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

// Start ptr is the pointer missing it's suffix link
void suffix_tree::resolve_missing_suffix_link(suffix_tree_node* start_ptr) {
  size_t i = start_ptr->m_start_idx + (start_ptr->m_parent_ptr == this->m_root_ptr ? 1 : 0); // idx that we are at
  size_t j = start_ptr->m_size - (start_ptr->m_parent_ptr == this->m_root_ptr ? 1 : 0); // how far we need to go
  size_t k = 0; // How far we have went
  if (!start_ptr->m_parent_ptr->m_suffix_link_ptr) {
    this->resolve_missing_suffix_link(start_ptr->m_parent_ptr);
  }
  // Take the parent's suffix link
  suffix_tree_node* cur_ptr = start_ptr->m_parent_ptr->m_suffix_link_ptr;
  while (k < j) {
    // Maybe this might have to do with the case where the cur pointer is root?
    cur_ptr = cur_ptr->find_child(this->m_str, i);
    i += cur_ptr->m_size;
    k += cur_ptr->m_size;
  }
  if (k == j) {
    start_ptr->m_suffix_link_ptr = cur_ptr;
  } else {
    const size_t l = cur_ptr->m_size - j + k;
    start_ptr->m_suffix_link_ptr = this->split_edge(cur_ptr, l);
  }
}

// void suffix_tree::resolve_missing_suffix_link(suffix_tree_node* start_ptr) {
//   assert(start_ptr && "FATAL ERROR, start_ptr was null inside resolve_missing_suffix_link");
//   suffix_tree_node* parent_ptr = start_ptr->m_parent_ptr;
//   // if parent_ptr->m_suffix_link_ptr is not defined, recusivly call
//   if (!parent_ptr->m_suffix_link_ptr) {
//     std::cout << "impossible case reached" << std::endl;
//     this->resolve_missing_suffix_link(parent_ptr);
//   }
//   // Take the suffix link to node 'v'
//   suffix_tree_node* cur_ptr = parent_ptr->m_suffix_link_ptr;
//   std::string str = parent_ptr->get_string(this->m_str) + start_ptr->get_string(this->m_str);
//   if (parent_ptr == this->m_root_ptr || parent_ptr->m_parent_ptr == this->m_root_ptr) {
//     str = str.substr(1);
//   }
//   if (str.size() == 0) {
//     start_ptr->m_suffix_link_ptr = this->m_root_ptr;
//     return;
//   }
//   size_t i = cur_ptr->m_size;
//   size_t target_depth = start_ptr->m_depth;
//   while (i < str.size()) {
//     cur_ptr = cur_ptr->find_child(this->m_str, str[i], false);
//     i += cur_ptr->m_size;
//   }
//   if (i == str.size()) {
//     // clean break
//     start_ptr->m_suffix_link_ptr = cur_ptr;
//   } else {
//     // went past where we wanted to be
//     suffix_tree_node* new_internal_ptr = this->split_edge(cur_ptr->m_parent_ptr, str.size() - parent_ptr->m_suffix_link_ptr->m_depth);
//     start_ptr->m_suffix_link_ptr = new_internal_ptr;
//   }
// }

// void suffix_tree::resolve_missing_suffix_link(suffix_tree_node* start_ptr) {
//   assert(start_ptr != nullptr && "FATAL ERROR, start_ptr was null in resolve_missing_suffix_link");
//   if (!start_ptr) return;
//   if (start_ptr == this->m_root_ptr) {
//     start_ptr->m_suffix_link_ptr = this->m_root_ptr;
//     return;
//   }  suffix_tree_node* parent = start_ptr->m_parent_ptr;
//   suffix_tree_node* suffix_link_candidate = parent->m_suffix_link_ptr;
//   if (!suffix_link_candidate) {
//     resolve_missing_suffix_link(parent);
//     suffix_link_candidate = parent->m_suffix_link_ptr;
//   }
//   size_t match_length = 0;
//   while (match_length < start_ptr->m_size && match_length < suffix_link_candidate->m_size &&
//          this->m_str[suffix_link_candidate->m_start_idx + match_length] == this->m_str[start_ptr->m_start_idx + match_length]) {
//     ++match_length;
//   }
//   if (match_length == suffix_link_candidate->m_size) {
//     start_ptr->m_suffix_link_ptr = suffix_link_candidate;
//   } else {
//     start_ptr->m_suffix_link_ptr = this->split_edge(suffix_link_candidate, match_length);
//   }
// }

/**
 * Splits the edge, go figure
 * The doubly linked list makes this an O(1) function :)
 * returns a pointer to the newly created internal node
 * 
 * this function looks scary
 * 
 * i is the new size of the new internal pointer
 * 
 * cur ptr will become a child of i
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
  new_internal_ptr->set_depth();
  return new_internal_ptr;
}