#include <string>
#include <cassert>
#include <iostream>
#include <filesystem>
#include "../AuctionManager.h"

using namespace std;

int main() {
    // #------------------------------------------------------------------#
    // |                      Teste 6 - Open Auction                      |
    // #------------------------------------------------------------------#

    int uID = 103327;
    const string password = "SenhaMegaSecreta";
    int total_open_auctions = 2; // LEMBRAR QUE ISTO NAO PODE PASSAR DE 9

    string fdata = "dados";
    string asset_name = "banger.txt";

    assert(open_auction(uID, "boas", 100, 30, asset_name, sizeof(fdata), fdata) == -2);

    assert(login(uID, password) == 1);

    assert(open_auction(uID, "boas", 100, 30, asset_name + to_string(1), sizeof(fdata), fdata) == 1);
    assert(open_auction(uID, "siuuu", 50.1, 150, asset_name + to_string(2), sizeof(fdata), fdata) == 2);

    assert(open_auction(uID, "duas palavras", 50.1, 150, "pog.txt", sizeof(fdata), fdata) == -1);
    assert(open_auction(uID, "umapalavra", -50.1, 150, "pog.txt", sizeof(fdata), fdata) == -1);

    // Limpar o lixo produzido pelo teste
    for (int i = 1; i <= total_open_auctions; i++) {
        string start_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/START_" + to_string(0) + to_string(0) + to_string(i) + ".txt";
        string asset_file_name = "ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i) + "/" + asset_name + to_string(i);
        string host_file_name = "ASDIR/USERS/" + to_string(uID) + "/HOSTED/" + to_string(0) + to_string(0) + to_string(i) + ".txt";

        delete_file(start_file_name);
        delete_file(asset_file_name);
        delete_file(host_file_name);
        filesystem::remove("ASDIR/AUCTIONS/" + to_string(0) + to_string(0) + to_string(i));
    }
        
    filesystem::remove("ASDIR/USERS/" + to_string(uID) + "/HOSTED");

    unregister(uID);
    filesystem::remove("ASDIR/USERS/" + to_string(uID));

    return 0;
}