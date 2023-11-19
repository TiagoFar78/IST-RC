#include <string>
#include <cassert>
#include <iostream>
#include "../AuctionManager.h"

using namespace std;

int main() {
    // #-------------------------------------------------------------------#
    // |                          Teste 3 - Login                          |
    // #-------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";

    assert(login(uID, password) == 1);

    string pass_file = "ASDIR/USERS/" + to_string(uID);
    pass_file.append("/" + to_string(uID));
    pass_file.append("_pass.txt");

    string login_file = "ASDIR/USERS/" + to_string(uID);
    login_file.append("/" + to_string(uID));
    login_file.append("_login.txt");

    assert(file_exists(pass_file) == 1);
    assert(file_exists(login_file) == 1);

    assert(login(uID, password) == 0);

    assert(file_exists(pass_file) == 1);
    assert(file_exists(login_file) == 1);

    string password_errada = "SenhaErrada";
    assert(login(uID, password_errada) == -1);

    assert(delete_file(pass_file) == 0);
    assert(delete_file(login_file) == 0);

    assert(file_exists(pass_file) == 0);
    assert(file_exists(login_file) == 0);

}