int file_exists(string file_name) {
    return 1;
}

int create_file(string file_name) {
    return 0;
}

/*
 * Return:
 * 1 - user was registered after this login
 * 0 - password is correct match
 * -1 - password is an incorrect match
 */
int login(int uID, string password) {
    std::string uID_string = std::to_string(uID);
    string file_name = uID_string + ".txt";
    if (!file_exists(file_name)) {
        create_file(file_name);
        return 1;
    }

    string registered_password = // read do file_name
    if (strcmp(password, registered_password) == 0) {
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