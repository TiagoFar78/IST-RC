#ifndef AUCTION_MANAGER_H
#define AUCTION_MANAGER_H

int login(int uID, const std::string& password);
int logout(int uID);
int unregister(int uID);
int listAuctionsTarget(int uID);
int listBidsTarget(int uID);
int listAuctions();
int showRecord(int aID);
int openAuction(int uID, const std::string& name, float startValue, int timeActive, const std::string& fileName, int fileSize, const std::string& fileData);
int close(int uID, const std::string& password, int aID);
int showAsset(int aID);
int bid(int uID, const std::string& password, int aID, float value);
int file_exists(const std::string& fileName);
int create_file(const std::string& fileName);
int delete_file(const std::string& fileName);
int write_on_file(const std::string& fileName, const std::string& buffer, bool clearFile);
int read_from_file(const std::string& fileName, std::string& buffer);

#endif
