#include <string>
#include <cassert>
#include <filesystem>
#include "../../Server.h"

using namespace std;

int main() {
    // #------------------------------------------------------------------#
    // |                     Teste 12 - Login Command                     |
    // #------------------------------------------------------------------#

    string reply = process_request("LIN\n");
    assert(reply == "RLI ERR\n");

    int uID = 103327;
    string uID_string = to_string(uID);

    reply = process_request("LIN 103327\n");
    assert(reply == "RLI ERR\n");

    reply = process_request("LIN 10000 passwor\n");
    assert(reply == "RLI ERR\n");

    reply = process_request("LIN 103327 passwor\n");
    assert(reply == "RLI ERR\n");

    reply = process_request("LIN 10000 password\n");
    assert(reply == "RLI ERR\n");

    reply = process_request("LIN 1000000 password\n");
    assert(reply == "RLI ERR\n");

    reply = process_request("LIN "+ uID_string + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("LIN " + uID_string + " passpass\n");
    assert(reply == "RLI NOK\n");

    reply = process_request("LIN " + uID_string + " passwoRd\n");
    assert(reply == "RLI NOK\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI OK\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI OK\n"); // TODO ver se é mesmo isto que acontece neste caso (tentar logar quando já está logado)

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);

    return 0;
}