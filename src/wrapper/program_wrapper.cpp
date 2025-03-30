#include "./program_wrapper.hpp"

#include <chrono>
#include <vector>

using namespace std;

string longestSubstring(string s) {

    int n = s.length();
    vector<int> dp(n+1,0);
    
    string ans = "";
    int ansLen = 0;
    
    // find length of non-overlapping 
    // substrings for all pairs (i,j)
    for (int i=n-1; i>=0; i--) {

        for (int j=i; j<n; j++) {
            
            // if characters match, set value 
            // and compare with ansLen.
            if (s[i]==s[j]) {
                dp[j] = 1 + min(dp[j+1], j-i-1);
                
                if (dp[j]>=ansLen) {
                    ansLen = dp[j];
                    ans = s.substr(i, ansLen);
                }
            }
            else dp[j] = 0;
        }
    }
    
    return ansLen>0?ans:"-1";
}

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


    

    auto start = std::chrono::high_resolution_clock::now();

    suffix_tree tree(alphabet_string, gene_string);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
}