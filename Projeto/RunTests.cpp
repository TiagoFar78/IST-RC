#include <iostream>
#include <cstdlib> // Include the header for system
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    std::vector<std::string> test_files;

    // Iterate through files in the "Testes" folder
    for (const auto& entry : fs::directory_iterator("Tests")) {
        std::string entry_name = entry.path().filename().string();
        if (entry.is_regular_file() && entry_name.substr(0, 4) == "test" && 
                entry_name.substr(entry_name.length() - 4, entry_name.length()) == ".cpp") {
            test_files.push_back(entry.path().string());
        }
    }

    // Run each test file
    for (const auto& test_file : test_files) {
        std::cout << "Running test file: " << test_file << " -> ";

        std::string test_file_without_extension = test_file.substr(0, test_file.length() - 4);

        std::string compile_command = "g++ AuctionManager.cpp " + test_file + " -o " + test_file_without_extension;
        std::system(compile_command.c_str());

        std::string run_command = test_file_without_extension;
        int result = system(run_command.c_str());

        if (result == 0) {
            std::cout << "\033[32mTest passed successfully.\033[0m" << std::endl;
        } else {
            std::cout << "\033[31mTest failed.\033[0m" << std::endl;
        }

        std::remove(test_file_without_extension.c_str());
    }

    return 0;
}
