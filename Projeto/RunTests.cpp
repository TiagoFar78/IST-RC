#include <iostream>
#include <cstdlib> // Include the header for system
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

bool compareFunction (string a, string b) {return a<b;} 

int main() {
    vector<string> test_files;

    // Iterate through files in the "Testes/ManagerTests" folder
    for (const auto& entry : fs::directory_iterator("Tests/ManagerTests")) {
        string entry_name = entry.path().filename().string();
        if (entry.is_regular_file() && entry_name.substr(0, 4) == "test" && 
                entry_name.substr(entry_name.length() - 4, entry_name.length()) == ".cpp") {
            test_files.push_back(entry.path().string());
        }
    }

    sort(test_files.begin(), test_files.end());

    if (test_files.size() > 0) {
        cout << endl << ">---------------------------------------------{ Manager Tests }---------------------------------------------<" << endl << endl;
    }

    // Run each test file
    for (const auto& test_file : test_files) {
        cout << "Running test file: " << test_file << " -> ";

        string test_file_without_extension = test_file.substr(0, test_file.length() - 4);

        string compile_command = "g++ -std=c++17 AuctionManager.cpp " + test_file + " -o " + test_file_without_extension;
        system(compile_command.c_str());

        string run_command = test_file_without_extension;
        int result = system(run_command.c_str());

        if (result == 0) {
            cout << "\033[32mTest passed successfully.\033[0m" << endl;
        } else {
            cout << "\033[31mTest failed.\033[0m" << endl;
        }

        remove(test_file_without_extension.c_str());
    }

    test_files.clear();

    // Iterate through files in the "Testes/ServerTests" folder
    for (const auto& entry : fs::directory_iterator("Tests/ServerTests")) {
        string entry_name = entry.path().filename().string();
        if (entry.is_regular_file() && entry_name.substr(0, 4) == "test" && 
                entry_name.substr(entry_name.length() - 4, entry_name.length()) == ".cpp") {
            test_files.push_back(entry.path().string());
        }
    }

    sort(test_files.begin(), test_files.end());

    if (test_files.size() > 0) {
        cout << endl << ">---------------------------------------------{ Server Tests }---------------------------------------------<" << endl << endl;
    }

    // Run each test file
    for (const auto& test_file : test_files) {
        cout << "Running test file: " << test_file << " -> ";

        string test_file_without_extension = test_file.substr(0, test_file.length() - 4);

        string compile_command = "g++ -std=c++17 AuctionManager.cpp Server.cpp ArgumentVerification.cpp " + test_file + " -o " + test_file_without_extension;
        system(compile_command.c_str());

        string run_command = test_file_without_extension;
        int result = system(run_command.c_str());

        if (result == 0) {
            cout << "\033[32mTest passed successfully.\033[0m" << endl;
        } else {
            cout << "\033[31mTest failed.\033[0m" << endl;
        }

        remove(test_file_without_extension.c_str());
    }

    return 0;
}
