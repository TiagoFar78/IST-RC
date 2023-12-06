#include <string>
#include <cassert>
#include <iostream>
#include <filesystem>
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #------------------------------------------------------------------#
    // |                         Teste 4 - Logout                         |
    // #------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";

    string pass_file = "ASDIR/USERS/" + to_string(uID);
    pass_file.append("/" + to_string(uID));
    pass_file.append("_pass.txt");

    string login_file = "ASDIR/USERS/" + to_string(uID);
    login_file.append("/" + to_string(uID));
    login_file.append("_login.txt");
    
    assert(file_exists(pass_file) == 0);
    assert(file_exists(login_file) == 0);

    assert(logout(uID) == -2);

    assert(login(uID, password) == 1);

    assert(file_exists(pass_file) == 1);
    assert(file_exists(login_file) == 1);

    assert(logout(uID) == 0);

    assert(file_exists(pass_file) == 1);
    assert(file_exists(login_file) == 0);

    assert(logout(uID) == -1);

    assert(file_exists(pass_file) == 1);
    assert(file_exists(login_file) == 0);

    // Limpar o lixo produzido pelo teste
    delete_file(pass_file);
    filesystem::remove("ASDIR/USERS/" + to_string(uID));
}