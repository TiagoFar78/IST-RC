#include <string>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include "../../AuctionManager.h"

using namespace std;

int main() {
    // #-------------------------------------------------------------------#
    // |                      Teste 7 - Close Auction                      |
    // #-------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";
    int total_open_auctions = 2; // LEMBRAR QUE ISTO NAO PODE PASSAR DE 9

    string fdata = "dados";
    string asset_name = "banger";
    string asset_extension = ".txt";

    // Basic tests
    assert(close(uID, 1) == -1);
    
    assert(login(uID, password) == 1);

    assert(close(uID, 1) == -2);

    assert(open_auction(uID, "boas", 100, 30, asset_name + to_string(1) + asset_extension, sizeof(fdata), fdata) == 1);

    assert(login(uID + 1, password) == 1);

    assert(close(uID + 1, 1) == -3);

    assert(close(uID, 1) == 0);

    assert(close(uID, 1) == -4);

    // Closed after expiring test
    assert(open_auction(uID, "segundo", 15, 1, asset_name + to_string(2) + asset_extension, sizeof(fdata), fdata) == 2);

    sleep(2);

    assert(close(uID, 2) == -4);

    // Limpar o lixo produzido pelo teste
    for (int i = 1; i <= total_open_auctions; i++) {
        string start_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/START_" + to_string(0) + to_string(0) + to_string(i) + ".txt";
        string asset_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/" + asset_name + to_string(i) + asset_extension;
        string end_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/END_" + to_string(0) + to_string(0) + to_string(i) + ".txt";
        string host_file_name = "ASDIR/USERS/" + to_string(uID) + "/HOSTED/" + to_string(0) + to_string(0) + to_string(i) + ".txt";

        delete_file(start_file_name);
        delete_file(asset_file_name);
        delete_file(end_file_name);
        delete_file(host_file_name);
        filesystem::remove("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i));
    }
        
    filesystem::remove("ASDIR/USERS/" + to_string(uID) + "/HOSTED");

    unregister(uID);
    filesystem::remove("ASDIR/USERS/" + to_string(uID));
    unregister(uID + 1);
    filesystem::remove("ASDIR/USERS/" + to_string(uID + 1));

    return 0;
}