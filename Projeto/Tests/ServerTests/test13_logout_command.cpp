#include <string>
#include <cassert>
#include <filesystem>
#include "../../Server.h"

using namespace std;

int main() {
    // #-----------------------------------------------------------------#
    // |                    Teste 13 - Logout Command                    |
    // #-----------------------------------------------------------------#

    string reply = process_request("LOU\n");
    assert(reply == "RLO ERR\n");

    int uID = 103327;
    string uID_string = to_string(uID);

    reply = process_request("LOU 103327\n");
    assert(reply == "RLO ERR\n");

    reply = process_request("LOU 10000 passwor\n");
    assert(reply == "RLO ERR\n");

    reply = process_request("LOU 103327 passwor\n");
    assert(reply == "RLO ERR\n");

    reply = process_request("LOU 10000 password\n");
    assert(reply == "RLO ERR\n");

    reply = process_request("LOU 1000000 password\n");
    assert(reply == "RLO ERR\n");

    reply = process_request("LOU " + uID_string + " password\n");
    assert(reply == "RLO UNR\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("LOU " + uID_string + " passpass\n");
    assert(reply == "RLO NOK\n");

    reply = process_request("LOU " + uID_string + " passwoRd\n");
    assert(reply == "RLO NOK\n");

    reply = process_request("LOU " + uID_string + " password\n");
    assert(reply == "RLO OK\n");

    reply = process_request("LOU " + uID_string + " password\n");
    assert(reply == "RLO NOK\n");

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);

    return 0;
}