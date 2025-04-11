#include "./suffix_tree.hpp"

suffix_tree::suffix_tree(const std::vector<std::pair<std::string, std::string>>& input_strs, const std::string& alphabet) :
  m_alphabet(alphabet), m_root_ptr(new suffix_tree_node()), m_id_pool(0) {

  this->m_alphabet.insert(this->m_alphabet.begin(), '$');
  
  this->m_strs.reserve(input_strs.size());
  this->m_c_strs.reserve(input_strs.size());

  for (const auto& [raw_str, label] : input_strs) {
    std::string terminated = raw_str + "$";
    this->m_strs.emplace_back(terminated, label);
    this->m_c_strs.push_back(this->m_strs.back().first.c_str());
  }

  this->m_root_ptr->m_parent_ptr = this->m_root_ptr;
  this->m_root_ptr->m_suffix_link_ptr = this->m_root_ptr;

  for (size_t i = 0; i < this->m_c_strs.size(); ++i) {
    this->add_string(i);
  }
}
suffix_tree::~suffix_tree() {
  const std::function<void(suffix_tree_node*)> release = [&release](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    release(cur_ptr->m_child_ptr);
    release(cur_ptr->m_next_sib_ptr);
    delete(cur_ptr);
  };
  release(this->m_root_ptr);
}

void suffix_tree::print_tree(std::ostream& os) {
  std::string prefix;
  std::string suffix;
  const std::function<void(suffix_tree_node*)> print_helper = [&print_helper, &os, &prefix, &suffix](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    os
      << prefix
      << (!cur_ptr->m_next_sib_ptr ? "└── " : "├── ")
      << (!cur_ptr->m_child_ptr ? "[Leaf]" : "[Internal]" )
      << " (" << cur_ptr->m_size << ", " << cur_ptr->m_depth << ")";
    if (!cur_ptr->m_child_ptr) {
      os << " \"" << suffix << "\"";
    }
    os << std::endl;
    const size_t prefix_size = prefix.size();
    const size_t suffix_size = suffix.size();
    prefix += (cur_ptr->m_next_sib_ptr ? "│   " : "    ");
    cur_ptr = cur_ptr->m_child_ptr;
    while (cur_ptr) {
      suffix += cur_ptr->get_string();
      print_helper(cur_ptr);
      suffix.resize(suffix_size);
      cur_ptr = cur_ptr->m_next_sib_ptr;
    }
    prefix.resize(prefix_size);
  };
  print_helper(this->m_root_ptr);
}

std::pair<size_t, std::vector<size_t>> suffix_tree::get_lrs(size_t str_id) {
  suffix_tree_node* deepest_internal = this->m_root_ptr;
  const std::function<void(suffix_tree_node*)> find_deepest_internal = [&find_deepest_internal, &deepest_internal, &str_id](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    if (!cur_ptr->is_visited_by(str_id)) return;
    find_deepest_internal(cur_ptr->m_child_ptr);
    find_deepest_internal(cur_ptr->m_next_sib_ptr);
    if (cur_ptr->m_child_ptr && cur_ptr->m_depth > deepest_internal->m_depth) {
      deepest_internal = cur_ptr;
    }
  };
  find_deepest_internal(this->m_root_ptr);
  std::vector<size_t> occurrences;
  suffix_tree_node* cur_ptr = deepest_internal->m_child_ptr;
  while (cur_ptr) {
    if (cur_ptr->is_visited_by(str_id)) {
      size_t lrs_start = cur_ptr->m_start - this->m_c_strs[str_id] - deepest_internal->m_depth;
      occurrences.push_back(lrs_start);
    }
    cur_ptr = cur_ptr->m_next_sib_ptr;
  }
  return {deepest_internal->m_depth, occurrences};
}

std::string suffix_tree::get_bwt(const size_t str_id) {
  this->sort_tree();
  std::string bwt_str;
  const std::function<void(suffix_tree_node*)> bwt_dfs = [&bwt_dfs, &bwt_str, &str_id, this](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) {
      return;
    } else if (!cur_ptr->m_child_ptr) {
      if (cur_ptr->m_id == 0) {
        bwt_str.push_back('$');
      } else {
        bwt_str.push_back(this->m_strs[str_id].first[cur_ptr->m_id - 1]);
      }
      return;
    }
    suffix_tree_node* cur_child = cur_ptr->m_child_ptr;
    while (cur_child) {
      bwt_dfs(cur_child);
      cur_child = cur_child->m_next_sib_ptr;
    }
  };
  bwt_dfs(this->m_root_ptr);
  return bwt_str;
}

size_t suffix_tree::get_num_internal_nodes(void) const {
  size_t total_internal_nodes = 0;
  const std::function<void(suffix_tree_node*)> counter_helper = [&counter_helper, &total_internal_nodes](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    if (cur_ptr->m_child_ptr) {
      ++total_internal_nodes;
    }
    counter_helper(cur_ptr->m_child_ptr);
    counter_helper(cur_ptr->m_next_sib_ptr);
  };
  counter_helper(this->m_root_ptr);
  return total_internal_nodes;
}

size_t suffix_tree::get_num_leaf_nodes(void) const {
  size_t total_leaf_nodes = 0;
  const std::function<void(suffix_tree_node*)> counter_helper = [&counter_helper, &total_leaf_nodes](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    if (!cur_ptr->m_child_ptr) {
      ++total_leaf_nodes;
    }
    counter_helper(cur_ptr->m_child_ptr);
    counter_helper(cur_ptr->m_next_sib_ptr);
  };
  counter_helper(this->m_root_ptr);
  return total_leaf_nodes;
}

size_t suffix_tree::get_num_nodes(void) const {
  size_t total_nodes = 0;
  const std::function<void(suffix_tree_node*)> counter_helper = [&counter_helper, &total_nodes](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    ++total_nodes;
    counter_helper(cur_ptr->m_child_ptr);
    counter_helper(cur_ptr->m_next_sib_ptr);
  };
  counter_helper(this->m_root_ptr);
  return total_nodes;
}

double suffix_tree::get_avg_internal_node_depth(void) const {
  size_t total_node_depth = 0;
  const std::function<void(suffix_tree_node*)> counter_helper = [&counter_helper, &total_node_depth](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    if (cur_ptr->m_child_ptr) {
      total_node_depth += cur_ptr->m_depth;
    }
    counter_helper(cur_ptr->m_child_ptr);
    counter_helper(cur_ptr->m_next_sib_ptr);
  };
  counter_helper(this->m_root_ptr);
  double avg_depth = static_cast<double>(total_node_depth) / static_cast<double>(this->get_num_internal_nodes());
  return avg_depth;
}

size_t suffix_tree::get_deepest_internal_node_depth(void) const {
  size_t deepest_depth = 0;
  const std::function<void(suffix_tree_node*)> get_deep_helper = [&get_deep_helper, &deepest_depth](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    if (cur_ptr->m_child_ptr && cur_ptr->m_depth > deepest_depth) {
      deepest_depth = cur_ptr->m_depth;
    }
    get_deep_helper(cur_ptr->m_child_ptr);
    get_deep_helper(cur_ptr->m_next_sib_ptr);
  };
  get_deep_helper(this->m_root_ptr);
  return deepest_depth;
}

size_t suffix_tree::get_string_size(const size_t id) const {
  return this->m_strs[id].first.size();
}

void suffix_tree::sort_tree(void) {
  const std::function<void(suffix_tree_node*)> sort_helper = [&sort_helper](suffix_tree_node* cur_ptr) -> void {
    if (!cur_ptr) return;
    cur_ptr->sort_children();
    sort_helper(cur_ptr->m_child_ptr);
    sort_helper(cur_ptr->m_next_sib_ptr);
  };
  sort_helper(this->m_root_ptr);
}

void suffix_tree::add_string(const size_t string_idx) {
  #if !USE_NAIVE_ALG
    suffix_tree_node* internal_ptr = this->m_root_ptr;
  #endif
  for (const char* cur_char_ptr = this->m_c_strs[string_idx]; *cur_char_ptr != '\0'; ++cur_char_ptr) {
    #if USE_NAIVE_ALG
      this->find_path_and_insert(this->m_root_ptr, cur_char_ptr, string_idx);
    #else
      if (!internal_ptr->m_suffix_link_ptr)
        this->resolve_missing_suffix_link(internal_ptr, string_idx);
      const char* next_pos = cur_char_ptr + internal_ptr->m_suffix_link_ptr->m_parent_ptr->m_depth;
      internal_ptr = this->find_path_and_insert(internal_ptr->m_suffix_link_ptr, next_pos, string_idx);
    #endif
  }
}

void suffix_tree::resolve_missing_suffix_link(suffix_tree_node* start_ptr, const size_t string_id) {
  suffix_tree_node* parent_ptr = start_ptr->m_parent_ptr;
  if (!parent_ptr->m_suffix_link_ptr) {
    this->resolve_missing_suffix_link(parent_ptr, string_id);
  }
  const char* i = start_ptr->m_start;
  size_t j = start_ptr->m_size;
  size_t k = 0;
  if (parent_ptr == this->m_root_ptr) {
    ++i;
    --j;
  }
  suffix_tree_node* cur_ptr = parent_ptr->m_suffix_link_ptr;
  while (k < j) {
    cur_ptr = cur_ptr->find_child(i);
    i += cur_ptr->m_size;
    k += cur_ptr->m_size;
  }
  if (k == j) {
    start_ptr->m_suffix_link_ptr = cur_ptr;
  } else {
    start_ptr->m_suffix_link_ptr = this->split_edge(cur_ptr, cur_ptr->m_size - k + j, string_id);
  }
}

suffix_tree_node* suffix_tree::find_path_and_insert(suffix_tree_node* cur_ptr, const char* cur_char_ptr, const size_t string_id) {
  assert(cur_ptr);
  assert(cur_char_ptr);
  cur_ptr->m_visitors.insert(string_id);
  const char* i = cur_ptr->m_start;
  const char* j = cur_char_ptr;
  size_t k = 0;
  while (k < cur_ptr->m_size && (*i) == (*j)) {
    if ((*i) == '\0') {
      return cur_ptr->m_parent_ptr;
    }
    ++i;
    ++j;
    ++k;
  }
  if (k == cur_ptr->m_size) {
    suffix_tree_node* next_ptr = cur_ptr->find_child(j);
    if (next_ptr) {
      return this->find_path_and_insert(next_ptr, j, string_id);
    } else {

      suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
      new_leaf_ptr->m_visitors.insert(string_id);
      new_leaf_ptr->m_id = this->m_id_pool++;
      new_leaf_ptr->m_start = j;
      new_leaf_ptr->m_size = this->get_string_size(string_id) - (j - this->m_c_strs[string_id]);
      new_leaf_ptr->m_parent_ptr = cur_ptr;
      if (cur_ptr->m_child_ptr)
        cur_ptr->m_child_ptr->m_prev_sib_ptr = new_leaf_ptr;
      new_leaf_ptr->m_next_sib_ptr = cur_ptr->m_child_ptr;
      cur_ptr->m_child_ptr = new_leaf_ptr;
      new_leaf_ptr->set_depth();
      return cur_ptr;
    }
  } else {
    suffix_tree_node* new_internal_ptr = this->split_edge(cur_ptr, k, string_id);
    suffix_tree_node* new_leaf_ptr = new suffix_tree_node();
    new_leaf_ptr->m_visitors.insert(string_id);
    new_leaf_ptr->m_id = this->m_id_pool++;
    new_leaf_ptr->m_start = j;
    new_leaf_ptr->m_size = this->get_string_size(string_id) - (j - this->m_c_strs[string_id]);
    cur_ptr->m_next_sib_ptr = new_leaf_ptr;
    new_leaf_ptr->m_prev_sib_ptr = cur_ptr;
    new_leaf_ptr->m_parent_ptr = new_internal_ptr;
    new_leaf_ptr->set_depth();
    return new_internal_ptr;
  }
}

suffix_tree_node* suffix_tree::split_edge(suffix_tree_node* cur_ptr, size_t size, const size_t string_id) {
  suffix_tree_node* new_internal_ptr = new suffix_tree_node();
  new_internal_ptr->m_visitors = cur_ptr->m_visitors;
  new_internal_ptr->m_visitors.insert(string_id);
  new_internal_ptr->m_start = cur_ptr->m_start;
  new_internal_ptr->m_size = size;
  cur_ptr->m_start += size;
  cur_ptr->m_size -= size;
  new_internal_ptr->m_child_ptr = cur_ptr;
  new_internal_ptr->m_next_sib_ptr = cur_ptr->m_next_sib_ptr;
  new_internal_ptr->m_prev_sib_ptr = cur_ptr->m_prev_sib_ptr;
  new_internal_ptr->m_parent_ptr = cur_ptr->m_parent_ptr;
  if (cur_ptr->m_parent_ptr->m_child_ptr == cur_ptr) {
    cur_ptr->m_parent_ptr->m_child_ptr = new_internal_ptr;
  }
  if (cur_ptr->m_next_sib_ptr) {
    cur_ptr->m_next_sib_ptr->m_prev_sib_ptr = new_internal_ptr;
  }
  if (cur_ptr->m_prev_sib_ptr) {
    cur_ptr->m_prev_sib_ptr->m_next_sib_ptr = new_internal_ptr;
  }
  cur_ptr->m_parent_ptr = new_internal_ptr;
  cur_ptr->m_next_sib_ptr = nullptr;
  cur_ptr->m_prev_sib_ptr = nullptr;
  new_internal_ptr->set_depth();
  return new_internal_ptr;
}