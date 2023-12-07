#include <string>
#include <cassert>
#include <filesystem>
#include <unistd.h>
#include "../../Server.h"
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #-----------------------------------------------------------------#
    // |               Teste 17 - List Bids Target Command               |
    // #-----------------------------------------------------------------#

    string reply = process_request("LMB\n");
    assert(reply == "RMB ERR\n");

    int uID = 103327;
    string uID_string = to_string(uID);
    string fdata = "dados";
    string asset_name = "banger.txt";

    reply = process_request("LMB 10000\n");
    assert(reply == "RMB ERR\n");

    reply = process_request("LMB 103327 passwor\n");
    assert(reply == "RMB ERR\n");

    reply = process_request("LMB 1000000\n");
    assert(reply == "RMB ERR\n");

    reply = process_request("LMB " + uID_string + "\n");
    assert(reply == "RMB NLG\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("LIN " + to_string(uID + 1) + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("LMB " + uID_string + "\n");
    assert(reply == "RMB NOK\n");

    // open auction
    assert(open_auction(uID + 1, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == 1);

    reply = process_request("LMB " + uID_string + "\n");
    assert(reply == "RMB NOK\n");

    // open second auction
    assert(open_auction(uID + 1, "segundo", 15, 1, asset_name, sizeof(fdata), fdata) == 2);

    // bid on second auction
    assert(bid(uID, 2, 100) == 0);

    reply = process_request("LMB " + uID_string + "\n");
    assert(reply == "RMB OK 002 1\n");

    sleep(2); // waits second auction finish

    reply = process_request("LMB " + uID_string + "\n");
    assert(reply == "RMB OK 002 0\n");

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);
    filesystem::remove_all("ASDIR/USERS/" + to_string(uID + 1));
    filesystem::remove_all("ASDIR/AUCTIONS/001");
    filesystem::remove_all("ASDIR/AUCTIONS/002");

    return 0;
}