#include <iostream>
#include <fstream>

using namespace std;

int file_exists(const string& file_name) {
    ifstream file(file_name);
    return file.good();
}

int create_file(const string& file_name) {
    ofstream file(file_name);
    return file.is_open() ? 0 : -1;
}

int delete_file(const string& file_name) {
    if (remove(file_name.c_str()) != 0) {
        return -1;
    }

    return 0;
}

int write_on_file(const string& file_name, const string& buffer, bool clear_file) {
    ofstream file;

    if (clear_file) {
        file.open(file_name, ios::out | ios::trunc); // Open the file in truncation mode to clear contents
    } else {
        file.open(file_name, ios::app); // Open the file in append mode
    }

    if (file.is_open()) {
        file << buffer;
        return 0;
    }

    return -1;
}

int read_from_file(const string& file_name, string& buffer) {
    ifstream file(file_name);
    if (file.is_open()) {
        file.seekg(0, ios::end);
        streamsize size = file.tellg();
        file.seekg(0, ios::beg);

        buffer.resize(size);
        file.read(&buffer[0], size);

        return 0;
    }

    return -1;
}


/*
 * Return:
 * 1 - user was registered after this login
 * 0 - password is correct match
 * -1 - password is an incorrect match
 */
int login(int uID, const string& password) {
    string uID_string = to_string(uID);
    string file_name = "ASDIR/USERS/" + uID_string + "/" + uID_string + "_pass.txt";
    if (!file_exists(file_name)) {
        create_file(file_name);
        write_on_file(file_name, password, true);
        return 1;
    }

    string registered_password;
    read_from_file(file_name, registered_password);
    if (password == registered_password) {
        string login_file_name = "ASDIR/USERS/" + uID_string + "/" + uID_string + "_login.txt";
        return 0;
    }

    return -1;
}


/*
 * Return:
 * 0 - logged out successfully
 * -1 - not logged in
 * -2 - not registered
 */
int logout(int uID) {
    // Basta remover o UID do zezoca da lista/hashtable dos gajos logged in
    return 0;
}


/*
 * Return:
 * 0 - unregistered successfully
 * -1 - not logged in
 * -2 - not registered
 */
int unregister(int uID) {
    return 0;
}


/*
 * Return:
 * {0, list} successfully
 * {-1, null} - no ongoing auctions
 * {-2, null} - not logged in
 */
int list_auctions_target(int uID) {
    return 0;
}


/*
 * Return:
 * {0, list} successfully
 * {-1, null} - no ongoing bids
 * {-2, null} - not logged in
 */
int list_bids_target(int uID) {
    return 0;
}


/*
 * Return:
 * {0, list} successfully
 */
int list_auctions() {
    return 0;
}


/*
 * Return:
 * {0, list} successfully
 * {-1, null} - does not exist auction
 */
int show_record(int aID) {
    return 0;
}


/*
 * Return:
 * created AID - when created successfully
 * -1 - could not create auction
 * -2 - not logged in
 */
int open_auction(int uID, string name, float start_value, int timeactive, string fname, int fsize, string fdata) {
    return 0;
}


/*
 * Return:
 * 0 - when closed successfully
 * -1 - not logged in
 * -2 - aID does not exist
 * -3 - auction is not owned by user uID
 * -4 - auction aID owned by user uID has already finished
 */
int close(int uID, string password, int aID) {
    return 0;
}


/*
 * Return:
 * {0, cena} - successfully
 * {-1, null} - no file (or other problem)
 */
int show_asset(int aID) {
    return 0;
}


/*
 * Return:
 * 0 - accepted
 * -1 - auction aID is not active
 * -2 - user was not logged in
 * -3 - refused because a larger bid
 * -4 - user tries to make a bid in an auction hosted by himself
 */
int bid(int uID, string password, int aID, float value) {
    return 0;
}