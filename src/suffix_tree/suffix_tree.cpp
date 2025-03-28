#include "./suffix_tree.hpp"

suffix_tree::suffix_tree(const std::string& _string, const std::string& _alphabet)
    : m_string(_string), m_alphabet(_alphabet), m_root_ptr(nullptr) {
}

suffix_tree::~suffix_tree() {

}