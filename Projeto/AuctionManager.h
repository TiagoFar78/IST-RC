#ifndef AUCTION_MANAGER_H
#define AUCTION_MANAGER_H

#include <vector>

struct AuctionState {
    std::string aID_string;
    int state;
};

int login(int uID, const std::string& password);
int logout(int uID);
int unregister(int uID);
std::vector<AuctionState> list_auctions_target(int uID);
std::vector<AuctionState> list_bids_target(int uID);
std::vector<AuctionState> list_auctions();
std::vector<std::string> get_auction_start_details(int aID);
std::vector<std::vector<std::string>> get_auction_bids_details(int aID);
std::vector<std::string> get_auction_end_details(int aID);
int open_auction(int uID, const std::string& name, int startValue, int timeActive, const std::string& fileName, int fileSize, const std::string& fileData);
int close(int uID, int aID);
int show_asset(int aID);
int bid(int uID, int aID, int value);
int file_exists(const std::string& fileName);
int create_file(const std::string& fileName);
int delete_file(const std::string& fileName);
int write_on_file(const std::string& fileName, const std::string& buffer, bool clearFile);
int read_from_file(const std::string& fileName, std::string& buffer);

#endif
