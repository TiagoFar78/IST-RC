#include <string>
#include <cassert>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include "../../Server.h"
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #----------------------------------------------------------------#
    // |                     Teste 20 - Bid Command                     |
    // #----------------------------------------------------------------#

    int uID = 103327;
    string uID_string = to_string(uID);
    string fdata = "dados";
    string asset_name = "banger.txt";

    string reply = process_request("BID");
    assert(reply == "RBD ERR\n");

    reply = process_request("BID 10");
    assert(reply == "RBD ERR\n");

    reply = process_request("BID 103327");
    assert(reply == "RBD ERR\n");

    reply = process_request("BID 103327 password");
    assert(reply == "RBD ERR\n");

    reply = process_request("BID 103327 password 10");
    assert(reply == "RBD ERR\n");

    reply = process_request("BID 103327 password 001");
    assert(reply == "RBD ERR\n");

    reply = process_request("BID " + uID_string + " password 001 20");
    assert(reply == "RBD NOK\n");

    reply = process_request("LIN " + uID_string + " password");
    assert(reply == "RLI REG\n");

    reply = process_request("LOU " + uID_string + " password");
    assert(reply == "RLO OK\n");

    reply = process_request("BID " + uID_string + " password 001 20");
    assert(reply == "RBD NLG\n");

    reply = process_request("LIN " + uID_string + " password");
    assert(reply == "RLI OK\n");

    reply = process_request("BID " + uID_string + " cenacena 001 20");
    assert(reply == "RBD NOK\n");

    reply = process_request("LIN " + to_string(uID + 1) + " password");
    assert(reply == "RLI REG\n");

    // open auction
    assert(open_auction(uID + 1, "boas", 100, 1, asset_name, sizeof(fdata), fdata) == 1);

    reply = process_request("BID " + to_string(uID + 1) + " password 001 90");
    assert(reply == "RBD REF\n");

    reply = process_request("BID " + to_string(uID + 1) + " password 001 150");
    assert(reply == "RBD ILG\n");

    reply = process_request("BID " + uID_string + " password 001 90");
    assert(reply == "RBD REF\n");

    reply = process_request("BID " + uID_string + " password 001 150");
    assert(reply == "RBD ACC\n");

    reply = process_request("BID " + uID_string + " password 001 150");
    assert(reply == "RBD REF\n");

    reply = process_request("CLS " + to_string(uID + 1) + " password 001");
    assert(reply == "RCL OK\n");

    reply = process_request("BID " + uID_string + " password 001 200");
    assert(reply == "RBD NOK\n");

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);
    filesystem::remove_all("ASDIR/USERS/" + to_string(uID + 1));
    filesystem::remove_all("ASDIR/AUCTIONS/001");

    return 0;
}