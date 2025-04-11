#include "./suffix_tree_node.hpp"

suffix_tree_node::suffix_tree_node() :
  m_parent_ptr(nullptr), m_child_ptr(nullptr),
  m_next_sib_ptr(nullptr), m_prev_sib_ptr(nullptr),
  m_suffix_link_ptr(nullptr), m_start(nullptr),
  m_size(0), m_depth(0), m_id(-1) {
}

suffix_tree_node::~suffix_tree_node() {
}

std::string suffix_tree_node::get_string() {
  std::string buffer;
  char const* cur = this->m_start;
  size_t i = 0;
  while (i < this->m_size) {
    buffer.push_back(*cur);
    ++cur;
    ++i;
  }
  return buffer;
}

std::string suffix_tree_node::get_complete_string() {
  std::string buffer;
  suffix_tree_node* cur_ptr = this;
  suffix_tree_node* prev_ptr = nullptr;
  while (cur_ptr && cur_ptr != prev_ptr) {
    std::string temp_buffer = cur_ptr->get_string();
    std::reverse(temp_buffer.begin(), temp_buffer.end());
    buffer += temp_buffer;
    prev_ptr = cur_ptr;
    cur_ptr = cur_ptr->m_parent_ptr;
  }
  std::reverse(buffer.begin(), buffer.end());
  return buffer;
}

suffix_tree_node* suffix_tree_node::find_child(const char* const ptr) {
  suffix_tree_node* cur_ptr = this->m_child_ptr;
  while (cur_ptr) {
    if (*(cur_ptr->m_start) == *ptr) {
      break;
    }
    cur_ptr = cur_ptr->m_next_sib_ptr;
  }
  return cur_ptr;
}

suffix_tree_node* suffix_tree_node::find_child(const char c) {
  suffix_tree_node* cur_ptr = this->m_child_ptr;
  while (cur_ptr) {
    if (*(cur_ptr->m_start) == c) {
      break;
    }
    cur_ptr = cur_ptr->m_next_sib_ptr;
  }
  return cur_ptr;
}

void suffix_tree_node::set_depth() {
  this->m_depth = this->m_parent_ptr->m_depth + this->m_size;
}

void suffix_tree_node::sort_children() {
  const std::function<suffix_tree_node*(suffix_tree_node*)> split = [](suffix_tree_node* head_ptr) -> suffix_tree_node* {
    suffix_tree_node* slow_ptr = head_ptr;
    suffix_tree_node* fast_ptr = head_ptr;
    while (fast_ptr->m_next_sib_ptr && fast_ptr->m_next_sib_ptr->m_next_sib_ptr) {
      slow_ptr = slow_ptr->m_next_sib_ptr;
      fast_ptr = fast_ptr->m_next_sib_ptr->m_next_sib_ptr;
    }
    suffix_tree_node* next_ptr = slow_ptr->m_next_sib_ptr;
    slow_ptr->m_next_sib_ptr = nullptr;
    if (next_ptr) {
      next_ptr->m_prev_sib_ptr = nullptr;
    }
    return next_ptr;
  };
  const std::function<suffix_tree_node*(suffix_tree_node*, suffix_tree_node*)> merge = [&merge](suffix_tree_node* first_ptr, suffix_tree_node* second_ptr) -> suffix_tree_node* {
    if (!first_ptr) return second_ptr;
    if (!second_ptr) return first_ptr;
    if (*(first_ptr->m_start) <= *(second_ptr->m_start)) {
      first_ptr->m_next_sib_ptr = merge(first_ptr->m_next_sib_ptr, second_ptr);
      if (first_ptr->m_next_sib_ptr) {
        first_ptr->m_next_sib_ptr->m_prev_sib_ptr = first_ptr;
      }
      first_ptr->m_prev_sib_ptr = nullptr;
      return first_ptr;
    } else {
      second_ptr->m_next_sib_ptr = merge(first_ptr, second_ptr->m_next_sib_ptr);
      if (second_ptr->m_next_sib_ptr) {
        second_ptr->m_next_sib_ptr->m_prev_sib_ptr = second_ptr;
      }
      second_ptr->m_prev_sib_ptr = nullptr;
      return second_ptr;
    }
  };
  const std::function<suffix_tree_node*(suffix_tree_node*)> merge_sort = [&split, &merge, &merge_sort](suffix_tree_node* head_ptr) -> suffix_tree_node* {
    if (!head_ptr || !head_ptr->m_next_sib_ptr) {
      return head_ptr;
    }
    suffix_tree_node* second_head_ptr = split(head_ptr);
    head_ptr = merge_sort(head_ptr);
    second_head_ptr = merge_sort(second_head_ptr);
    return merge(head_ptr, second_head_ptr);
  };
  this->m_child_ptr = merge_sort(this->m_child_ptr);
}

bool suffix_tree_node::is_visited_by(size_t id) {
  return this->m_visitors.find(id) != this->m_visitors.end();
}