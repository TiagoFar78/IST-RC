#ifndef AUCTION_MANAGER_H
#define AUCTION_MANAGER_H

int login(int uID, const std::string& password);
int logout(int uID);
int unregister(int uID);
int list_auctions_target(int uID);
int list_bids_target(int uID);
int list_auctions();
int show_record(int aID);
int open_auction(int uID, const std::string& name, float startValue, int timeActive, const std::string& fileName, int fileSize, const std::string& fileData);
int close(int uID, const std::string& password, int aID);
int show_asset(int aID);
int bid(int uID, const std::string& password, int aID, float value);
int file_exists(const std::string& fileName);
int create_file(const std::string& fileName);
int delete_file(const std::string& fileName);
int write_on_file(const std::string& fileName, const std::string& buffer, bool clearFile);
int read_from_file(const std::string& fileName, std::string& buffer);

#endif
