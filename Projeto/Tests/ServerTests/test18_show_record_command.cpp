#include <string>
#include <cassert>
#include <filesystem>
#include <unistd.h>
#include "../../Server.h"
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #----------------------------------------------------------------#
    // |                 Teste 18 - Show Record Command                 |
    // #----------------------------------------------------------------#

    int uID = 103327;
    string uID_string = to_string(uID);
    string fdata = "dados";
    string asset_name = "banger.txt";

    string reply = process_request("SRC\n");
    assert(reply == "RRC ERR\n");

    reply = process_request("SRC 10\n");
    assert(reply == "RRC ERR\n");

    reply = process_request("SRC 001 password\n");
    assert(reply == "RRC ERR\n");

    reply = process_request("SRC 1000\n");
    assert(reply == "RRC ERR\n");

    reply = process_request("SRC 001\n");
    assert(reply == "RRC NOK\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("LIN " + to_string(uID + 1) + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("SRC 001\n");
    assert(reply == "RRC NOK\n");

    // open auction
    assert(open_auction(uID + 1, "boas", 100, 1, asset_name, sizeof(fdata), fdata) == 1);

    reply = process_request("SRC 001\n");
    assert(reply.find("RRC OK " + to_string(uID + 1) + " boas " + asset_name + " 100 ") < reply.length());
    assert(reply.find(" B ") >= reply.length());
    assert(reply.find(" E ") >= reply.length());

    // bid on auction
    assert(bid(uID, 1, 120) == 0);

    reply = process_request("SRC 001\n");
    assert(reply.find("RRC OK " + to_string(uID + 1) + " boas " + asset_name + " 100 ") < reply.length());
    assert(reply.find(" B " + uID_string + " 120 ") < reply.length());
    assert(reply.find(" E ") >= reply.length());

    // bid on auction
    assert(bid(uID, 1, 150) == 0);

    reply = process_request("SRC 001\n");
    assert(reply.find("RRC OK " + to_string(uID + 1) + " boas " + asset_name + " 100 ") < reply.length());
    assert(reply.find(" B " + uID_string + " 120 ") < reply.length());
    assert(reply.find(" B " + uID_string + " 150 ") < reply.length());
    assert(reply.find(" E ") >= reply.length());

    sleep(2); // waits auction finish

    reply = process_request("SRC 001\n");
    assert(reply.find("RRC OK " + to_string(uID + 1) + " boas " + asset_name + " 100 ") < reply.length());
    assert(reply.find(" B " + uID_string + " 120 ") < reply.length());
    assert(reply.find(" B " + uID_string + " 150 ") < reply.length());
    assert(reply.find(" E ") < reply.length());

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);
    filesystem::remove_all("ASDIR/USERS/" + to_string(uID + 1));
    filesystem::remove_all("ASDIR/AUCTIONS/001");

    return 0;
}