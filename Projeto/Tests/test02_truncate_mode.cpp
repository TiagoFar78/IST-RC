
#include <string>
#include <cassert>
#include "../AuctionManager.h"

using namespace std;

int main() {
    // #-------------------------------------------------------------------#
    // |                  Teste 2 - Funções de ficheiros                   |
    // #-------------------------------------------------------------------#

    string file_name = "example.txt";
    
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

    // Write less to file
    const string data_to_write2 = "Only hellow\n";
    assert(write_on_file(file_name, data_to_write2, true) == 0);

    // Read from file
    buffer = "";
    assert(read_from_file(file_name, buffer) == 0);

    assert(data_to_write2 == buffer);

    assert(delete_file(file_name) == 0);

    assert(file_exists(file_name) == 0);

    return 0;
}