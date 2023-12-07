#include <string>
#include <cassert>
#include <filesystem>
#include <unistd.h>
#include "../../Server.h"
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #----------------------------------------------------------------#
    // |                    Teste 19 - Close Command                    |
    // #----------------------------------------------------------------#

    int uID = 103327;
    string uID_string = to_string(uID);
    string fdata = "dados";
    string asset_name = "banger.txt";

    string reply = process_request("CLS\n");
    assert(reply == "RCL ERR\n");

    reply = process_request("CLS 10\n");
    assert(reply == "RCL ERR\n");

    reply = process_request("CLS 103327\n");
    assert(reply == "RCL ERR\n");

    reply = process_request("CLS 103327 password\n");
    assert(reply == "RCL ERR\n");

    reply = process_request("CLS 103327 password 10\n");
    assert(reply == "RCL ERR\n");

    reply = process_request("CLS " + uID_string + " password 001\n");
    assert(reply == "RCL NOK\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("CLS " + uID_string + " password 001\n");
    assert(reply == "RCL EAU\n");

    reply = process_request("CLS " + uID_string + " cenacena 001\n");
    assert(reply == "RCL NOK\n");

    // open auction
    assert(open_auction(uID, "boas", 100, 1, asset_name, sizeof(fdata), fdata) == 1);
    
    reply = process_request("LIN " + to_string(uID + 1) + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("CLS " + to_string(uID + 1) + " password 001\n");
    assert(reply == "RCL EOW\n");

    reply = process_request("LOU " + uID_string + " password\n");
    assert(reply == "RLO OK\n");

    reply = process_request("CLS " + uID_string + " password 001\n");
    assert(reply == "RCL NLG\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI OK\n");

    reply = process_request("CLS " + uID_string + " password 001\n");
    assert(reply == "RCL OK\n");

    reply = process_request("CLS " + uID_string + " password 001\n");
    assert(reply == "RCL END\n");

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);
    filesystem::remove_all("ASDIR/USERS/" + to_string(uID + 1));
    filesystem::remove_all("ASDIR/AUCTIONS/001");

    return 0;
}