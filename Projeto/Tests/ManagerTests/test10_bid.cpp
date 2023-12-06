#include <string>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <vector>
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #------------------------------------------------------------------#
    // |                          Teste 10 - Bid                          |
    // #------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";
    int total_open_auctions = 1; // LEMBRAR QUE ISTO NAO PODE PASSAR DE 9

    string fdata = "dados";
    string asset_name = "banger.txt";

    assert(login(uID, password) == 1);

    assert(bid(uID, 1, 10) == -1);

    assert(open_auction(uID, "boas", 5, 30, asset_name, sizeof(fdata), fdata) == 1);

    assert(bid(uID, 1, 10) == -3);

    assert(login(uID + 1, password) == 1);

    assert(bid(uID + 1, 1, 10) == 0);

    assert(bid(uID + 1, 1, 9) == -2);
    
    assert(bid(uID + 1, 1, 20) == 0);

    assert(bid(uID + 1, 1, 21) == 0);

    assert(bid(uID + 1, 1, 22) == 0);

    assert(close(uID, 1) == 0);

    assert(bid(uID + 1, 1, 20) == -1);

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

        delete_file("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(1) + "/BIDS/" + to_string(0) + to_string(0) + to_string(0) + to_string(0) + to_string(1) + to_string(0) + ".txt");
        delete_file("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(1) + "/BIDS/" + to_string(0) + to_string(0) + to_string(0) + to_string(0) + to_string(2) + to_string(0) + ".txt");
        delete_file("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(1) + "/BIDS/" + to_string(0) + to_string(0) + to_string(0) + to_string(0) + to_string(2) + to_string(1) + ".txt");
        delete_file("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(1) + "/BIDS/" + to_string(0) + to_string(0) + to_string(0) + to_string(0) + to_string(2) + to_string(2) + ".txt");
        
        filesystem::remove("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/BIDS");
        filesystem::remove("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i));
    }
        
    filesystem::remove("ASDIR/USERS/" + to_string(uID) + "/HOSTED");
    filesystem::remove("ASDIR/USERS/" + to_string(uID + 1) + "/HOSTED");

    delete_file("ASDIR/USERS/" + to_string(uID + 1) + "/BIDDED/001.txt");
    filesystem::remove("ASDIR/USERS/" + to_string(uID + 1) + "/BIDDED");

    unregister(uID);
    filesystem::remove("ASDIR/USERS/" + to_string(uID));
    unregister(uID + 1);
    filesystem::remove("ASDIR/USERS/" + to_string(uID + 1));

    return 0;
}