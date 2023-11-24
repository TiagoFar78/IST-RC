#include <string>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <vector>
#include "../AuctionManager.h"

using namespace std;

int main() {
    // #------------------------------------------------------------------#
    // |                  Teste 9 - List Auctions Target                  |
    // #------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";
    int total_open_auctions = 3; // LEMBRAR QUE ISTO NAO PODE PASSAR DE 9

    string fdata = "dados";
    string asset_name = "banger.txt";

    // Basic tests
    vector<AuctionState> list = list_auctions_target(uID);

    assert(list.size() == 0);

    assert(login(uID, password) == 1);

    assert(open_auction(uID, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == 1);

    list = list_auctions_target(uID);
    assert(list.size() == 1);
    assert(list[0].aID_string == "001" && list[0].state == 1);

    assert(close(uID, 1) == 0);

    list = list_auctions_target(uID);
    assert(list.size() == 1);
    assert(list[0].aID_string == "001" && list[0].state == 0);

    assert(login(uID + 1, password));

    assert(open_auction(uID + 1, "segunda", 100, 30, asset_name, sizeof(fdata), fdata) == 2);

    list = list_auctions_target(uID);
    assert(list.size() == 1);
    assert(list[0].aID_string == "001" && list[0].state == 0);

    assert(open_auction(uID, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == 3);

    list = list_auctions_target(uID);
    assert(list.size() == 2);
    assert(list[0].aID_string == "001" && list[0].state == 0);
    assert(list[1].aID_string == "003" && list[1].state == 1);

    // Limpar o lixo produzido pelo teste
    for (int i = 1; i <= total_open_auctions; i++) {
        string start_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/START_" + to_string(0) + to_string(0) + to_string(i) + ".txt";
        string asset_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/" + asset_name;
        string end_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/END_" + to_string(0) + to_string(0) + to_string(i) + ".txt";
        string host_file_name = "ASDIR/USERS/" + to_string(uID + ((i - 1) % 2)) + "/HOSTED/" + to_string(0) + to_string(0) + to_string(i) + ".txt";

        delete_file(start_file_name);
        delete_file(asset_file_name);
        delete_file(end_file_name);
        delete_file(host_file_name);
        filesystem::remove("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i));
    }
        
    filesystem::remove("ASDIR/USERS/" + to_string(uID) + "/HOSTED");
    filesystem::remove("ASDIR/USERS/" + to_string(uID + 1) + "/HOSTED");

    unregister(uID);
    filesystem::remove("ASDIR/USERS/" + to_string(uID));
    unregister(uID + 1);
    filesystem::remove("ASDIR/USERS/" + to_string(uID + 1));

    return 0;
}