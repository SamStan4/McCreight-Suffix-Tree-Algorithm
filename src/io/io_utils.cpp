#include "./io_utils.hpp"

namespace io_utils {
    bool load_one_gene(const char* file_path, std::string& out_string, std::string& out_string_name) {
        out_string.clear();
        out_string_name.clear();
        bool first_string_flag = true;
        std::ifstream input_stream(file_path);
        if (!input_stream.is_open()) {
            return false;
        }
        std::string buffer;
        while (std::getline(input_stream, buffer)) {
            if (buffer.empty()) {
                continue;
            } else if (buffer[0] == '>') {
                if (first_string_flag) {
                    first_string_flag = false;
                    out_string_name = buffer.substr(1);
                } else {
                    return true;
                }
                continue;
            }
            out_string += buffer;
        }
        input_stream.close();
        return true;
    }

    bool load_one_gene(const char* file_path, std::string& out_string) {
        out_string.clear();
        bool first_string_flag = true;
        std::ifstream input_stream(file_path);
        if (!input_stream.is_open()) {
            return false;
        }
        std::string buffer;
        while (std::getline(input_stream, buffer)) {
            if (buffer.empty()) {
                continue;
            } else if (buffer[0] == '>') {
                if (first_string_flag) {
                    first_string_flag = false;
                } else {
                    return true;
                }
                continue;
            }
            out_string += buffer;
        }
        input_stream.close();
        return true;
    }

    bool load_alphabet(const char* file_path, std::string& out_string) {
        out_string.clear();
        std::ifstream input_stream(file_path);
        if (!input_stream.is_open()) {
            return false;
        }
        std::string buffer;
        std::unordered_set<char> alphabet_set;
        while (std::getline(input_stream, buffer)) {
            for (const char c : buffer) {
                if (std::isupper(c)) {
                    alphabet_set.insert(c);
                }
            }
        }
        input_stream.close();
        out_string.reserve(alphabet_set.size());
        for (const char c : alphabet_set) {
            out_string.push_back(c);
        }
        std::sort(out_string.begin(), out_string.end());
        return true;
    }
}