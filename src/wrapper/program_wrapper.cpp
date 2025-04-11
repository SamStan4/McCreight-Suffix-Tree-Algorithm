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
    if (!io_utils::load_one_gene(arg_vector[2], gene_string)) {
        std::cerr << "error loading gene" << std::endl;
        return;
    }

    std::string out_path = "./output.txt";

    this->run_construction(gene_string, alphabet_string, out_path);
}

void program_wrapper::run_construction(const std::string& gene, const std::string& alphabet, const std::string out) {

    auto start = std::chrono::high_resolution_clock::now();

    suffix_tree tree({{gene, alphabet}}, alphabet);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::ofstream ofs(out);

    ofs
        << "creation time: "
        << duration.count()
        << " ms" << std::endl << std::endl
        << "number of internal nodes: " << tree.get_num_internal_nodes() << std::endl
        << "number of leaf nodes: " << tree.get_num_leaf_nodes() << std::endl
        << "number of nodes: " << tree.get_num_nodes() << std::endl
        << "average string depth of an internal node: " << tree.get_avg_internal_node_depth() << std::endl
        << "deepest internal node depth: " << tree.get_deepest_internal_node_depth() << std::endl << std::endl;
    
    const std::pair<size_t, std::vector<size_t>> lrs = tree.get_lrs(0);

    ofs
        << "***************************************************************" << std::endl << std::endl
        << "longest repeating substring:" << std::endl
        << "\tlength: " << lrs.first << std::endl
        << "\tstart positions: ";

    for (const size_t pos : lrs.second) {
        ofs << pos << " ";
    }

    const std::string bwt = tree.get_bwt(0);

    ofs
        << std::endl << std::endl
        << "***************************************************************" << std::endl << std::endl
        << "string BWT: " << std::endl;

    for (const char c : bwt) {
        ofs << c << std::endl;
    }
}