#include "./wrapper/program_wrapper.hpp"

int main(int argc, char** argv) {
    program_wrapper& app = program_wrapper::get_instance();
    app.run_program(argc, argv);
    return 0;
}