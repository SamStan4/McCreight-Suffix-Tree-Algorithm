#include "./program_wrapper.hpp"

program_wrapper& program_wrapper::get_instance(void) {
    static program_wrapper instance;
    return instance;
}

void program_wrapper::run_program(const int arg_count, const char* const* arg_vector) {
    if (arg_count != 3) {
        std::cerr << "incorrect command line args, exiting" << std::endl;
        return;
    }
    std::string gene_string;
    std::string alphabet_string;
    if (!io_utils::load_alphabet(arg_vector[1], alphabet_string)) {
        std::cerr << "error loading alphabet" << std::endl;
        return;
    }
    // if (!io_utils::load_one_gene(arg_vector[2], gene_string)) {
    //     std::cerr << "error loading gene" << std::endl;
    //     return;
    // }
    if (!io_utils::load_one_gene("inputs/genes/Human-BRCA2-cds.fasta", gene_string)) {
        std::cerr << "error loading gene" << std::endl;
        return;
    }

    suffix_tree tree(alphabet_string, gene_string);
    // tree.print(std::cout);
    // std::cout << tree.get_str_size() << std::endl;
    // std::cout << tree.get_number_leaf_nodes() << std::endl;
    // std::cout << tree.get_number_internal_nodes() << std::endl;
    // tree.write_BWT(std::cout);
    std::ofstream of("./output/bwt/Human-BRCA2-cds.txt", std::ios::out);
    if (!of.is_open()) {
        throw std::runtime_error("");
    }
    tree.write_BWT(of);
}