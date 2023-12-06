#include <string>
#include <iostream>
#include <ostream>
#include <cassert>
#include <filesystem>
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #-------------------------------------------------------------------#
    // |                  Teste 1 - Funções de ficheiros                   |
    // #-------------------------------------------------------------------#

    string file_name = "example/example.txt";
    
    // Check if file exists
    assert(file_exists(file_name) == 0);
    
    // Create a file
    assert(create_file(file_name) == 0);
    assert(file_exists(file_name) == 1);
    
    // Write to file
    const string data_to_write = "Hello, World!\n";
    assert(write_on_file(file_name, data_to_write, true) == 0);
    
    // Read from file
    string buffer;
    assert(read_from_file(file_name, buffer) == 0);

    assert(data_to_write == buffer);

    assert(delete_file(file_name) == 0);

    assert(file_exists(file_name) == 0);

    // Limpar o lixo produzido pelo teste
    filesystem::remove("example");
}