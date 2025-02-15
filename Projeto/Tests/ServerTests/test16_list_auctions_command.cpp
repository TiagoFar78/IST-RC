#include <string>
#include <cassert>
#include <filesystem>
#include <unistd.h>
#include "../../Server.h"
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #----------------------------------------------------------------#
    // |                Teste 16 - List Auctions Command                |
    // #----------------------------------------------------------------#

    string reply = process_request("LST a\n");
    assert(reply == "RLS ERR\n");

    int uID = 103327;
    string uID_string = to_string(uID);
    string fdata = "dados";
    string asset_name = "banger.txt";

    reply = process_request("LST\n");
    assert(reply == "RLS NOK\n");

    reply = process_request("LIN " + uID_string + " password\n");
    assert(reply == "RLI REG\n");

    reply = process_request("LST\n");
    assert(reply == "RLS NOK\n");

    // open auction
    assert(open_auction(uID, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == 1);

    reply = process_request("LST\n");
    assert(reply == "RLS OK 001 1\n");

    // open second auction
    assert(open_auction(uID, "segundo", 15, 1, asset_name, sizeof(fdata), fdata) == 2);

    reply = process_request("LST\n");
    assert(reply == "RLS OK 001 1 002 1\n");

    sleep(2); // waits second auction finish

    reply = process_request("LST\n");
    assert(reply == "RLS OK 001 1 002 0\n");

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + uID_string);
    filesystem::remove_all("ASDIR/AUCTIONS/001");
    filesystem::remove_all("ASDIR/AUCTIONS/002");

    return 0;
}