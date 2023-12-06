#include <string>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <vector>
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #-------------------------------------------------------------------#
    // |                    Teste 11 - List Bids Target                    |
    // #-------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";
    int total_open_auctions = 3; // LEMBRAR QUE ISTO NAO PODE PASSAR DE 9

    string fdata = "dados";
    string asset_name = "banger.txt";

    // Basic tests
    vector<AuctionState> list = list_bids_target(uID);

    assert(list.size() == 0);

    assert(login(uID, password) == 1);

    assert(login(uID + 1, password) == 1);

    assert(open_auction(uID + 1, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == 1);

    assert(bid(uID, 1, 120) == 0);

    list = list_bids_target(uID);
    assert(list.size() == 1);
    assert(list[0].aID_string == "001" && list[0].state == 1);

    assert(close(uID + 1, 1) == 0);

    list = list_bids_target(uID);
    assert(list.size() == 1);
    assert(list[0].aID_string == "001" && list[0].state == 0);

    assert(open_auction(uID, "segunda", 100, 30, asset_name, sizeof(fdata), fdata) == 2);

    assert(bid(uID + 1, 2, 150) == 0);

    list = list_bids_target(uID);
    assert(list.size() == 1);
    assert(list[0].aID_string == "001" && list[0].state == 0);

    assert(open_auction(uID + 1, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == 3);

    assert(bid(uID, 3, 200) == 0);

    list = list_bids_target(uID);
    assert(list.size() == 2);
    assert(list[0].aID_string == "001" && list[0].state == 0);
    assert(list[1].aID_string == "003" && list[1].state == 1);

    // Limpar o lixo produzido pelo teste
    filesystem::remove_all("ASDIR/USERS/" + to_string(uID));
    filesystem::remove_all("ASDIR/USERS/" + to_string(uID + 1));

    for (int i = 1; i <= total_open_auctions; i++) {       
        filesystem::remove_all("ASDIR/AUCTIONS/00" + to_string(i));
    }

    return 0;
}