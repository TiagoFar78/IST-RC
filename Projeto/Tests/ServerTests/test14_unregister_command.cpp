#include <string>
#include <cassert>
#include <filesystem>
#include "../../Server.h"

using namespace std;

int main() {
    // #-----------------------------------------------------------------#
    // |                    Teste 13 - Logout Command                    |
    // #-----------------------------------------------------------------#

    string reply = process_request("UNR");
    assert(reply == "RUR ERR\n");

    int uID = 103327;
    string uID_string = to_string(uID);

    reply = process_request("UNR 103327");
    assert(reply == "RUR ERR\n");

    reply = process_request("UNR 10000 passwor");
    assert(reply == "RUR ERR\n");

    reply = process_request("UNR 103327 passwor");
    assert(reply == "RUR ERR\n");

    reply = process_request("UNR 10000 password");
    assert(reply == "RUR ERR\n");

    reply = process_request("UNR 1000000 password");
    assert(reply == "RUR ERR\n");

    reply = process_request("UNR " + uID_string + " password");
    assert(reply == "RUR UNR\n");

    reply = process_request("LIN " + uID_string + " password");
    assert(reply == "RLI REG\n");

    reply = process_request("UNR " + uID_string + " passpass");
    assert(reply == "RUR NOK\n");

    reply = process_request("UNR " + uID_string + " passwoRd");
    assert(reply == "RUR NOK\n");

    reply = process_request("LOU " + uID_string + " password");
    assert(reply == "RLO OK\n");

    reply = process_request("UNR " + uID_string + " password");
    assert(reply == "RUR NOK\n");

    reply = process_request("LIN " + uID_string + " password");
    assert(reply == "RLI OK\n");

    reply = process_request("UNR " + uID_string + " password");
    assert(reply == "RUR OK\n");

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);

    return 0;
}