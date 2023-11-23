#include <iostream>
#include <fstream>
#include <cmath>
#include <dirent.h>
#include <vector>
#include <sys/stat.h>
#include "AuctionManager.h"

using namespace std;

// #-------------------------------------------------------------------#
// |                        Function Definition                        |
// #-------------------------------------------------------------------#

int close(int aID);
string add_zeros_before(int zeros_amount, int number);
int auction_expired_time(int aID);

// #-------------------------------------------------------------------#
// |                      Function Implementation                      |
// #-------------------------------------------------------------------#

// > --------------------- { Useful Functions } --------------------- <

int getLastIndexOfSlash(const string& s) {
	for(int i = s.length() - 1; i >= 0; i--) {
		if (s[i] == '/') {
			return i;
        }
	}
	
	return -1;	
}

vector<string> split_string(string input, char delimiter) {
    vector<string> result;
    string current;

    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] != delimiter) {
            current += input[i];
        }

        if (input[i] == delimiter || i == input.length() - 1) {
            result.push_back(current);
            current.clear(); 
        }
    }

    return result;
}

string add_zeros_before(int zeros_amount, int number) {
    string s = "";
    string number_string = to_string(number);
    int zeros_to_add = zeros_amount - number_string.length();
    for (int i = 0; i < zeros_to_add; i++) {
        s += to_string(0);
    }

    s += number_string;

    return s;
}

int count_folder_entries(string folder_path) {
    struct dirent** trash; // TODO como mandar isto literalmente para o lixo? Isto serve?
    return scandir(folder_path.c_str(), &trash, 0, alphasort) - 2; // -2 porque conta com o . e ..
}


// > ---------------------- { File Functions } ---------------------- <

int file_exists(const string& file_name) {
    ifstream file(file_name);
    return file.good();
}

int create_file(const string& file_name) {
    int last_index_of_slash = getLastIndexOfSlash(file_name);

    if (last_index_of_slash == -1) {
        ofstream file(file_name);
        file.close();
        return 0;
    }

    string folder_name = file_name.substr(0, last_index_of_slash);

    int status = mkdir(folder_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (status == 0 || errno == EEXIST) { // EEXIST means the folder already exists
        ofstream outfile(file_name);

        if (outfile.is_open()) {
            outfile.close();
            return 0;
        }
    }

    return -1;
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


// > ----------------------- { User Session } ----------------------- <

/*
 * Return:
 * 1 - user was registered after this login
 * 0 - password is correct match
 * -1 - password is an incorrect match
 */
int login(int uID, const string& password) {
    string uID_string = to_string(uID);

    string pass_file_name = "ASDIR/USERS/" + uID_string;
    pass_file_name.append("/" + uID_string);
    pass_file_name.append("_pass.txt");
    
    string login_file_name = "ASDIR/USERS/" + uID_string;
    login_file_name.append("/" + uID_string);
    login_file_name.append("_login.txt");

    if (!file_exists(pass_file_name)) {
        create_file(pass_file_name);
        write_on_file(pass_file_name, password, true);
        create_file(login_file_name);
        return 1;
    }

    string registered_password;
    read_from_file(pass_file_name, registered_password);
    if (password == registered_password) {
        create_file(login_file_name);
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
    string uID_string = to_string(uID);

    string pass_file_name = "ASDIR/USERS/" + uID_string;
    pass_file_name.append("/" + uID_string);
    pass_file_name.append("_pass.txt");

    if (!file_exists(pass_file_name)) {
        return -2;
    }
    
    string login_file_name = "ASDIR/USERS/" + uID_string;
    login_file_name.append("/" + uID_string);
    login_file_name.append("_login.txt");

    if (!file_exists(login_file_name)) {
        return -1;
    }

    delete_file(login_file_name);

    return 0;
}


/*
 * Return:
 * 0 - unregistered successfully
 * -1 - not logged in
 * -2 - not registered
 */
int unregister(int uID) {
    string uID_string = to_string(uID);

    string pass_file_name = "ASDIR/USERS/" + uID_string;
    pass_file_name.append("/" + uID_string);
    pass_file_name.append("_pass.txt");

    if (!file_exists(pass_file_name)) {
        return -2;
    }
    
    string login_file_name = "ASDIR/USERS/" + uID_string;
    login_file_name.append("/" + uID_string);
    login_file_name.append("_login.txt");

    if (!file_exists(login_file_name)) {
        return -1;
    }

    delete_file(pass_file_name);
    delete_file(login_file_name);

    return 0;
}


// > ----------------------- { Lists & Show } ----------------------- <

/*
 * Return:
 * list of auctions owned by uID
 */
vector<AuctionState> list_auctions_target(int uID) {
    string uID_string = to_string(uID);

    vector<AuctionState> auctions_states;

    int i = 1;
    string aID_string = add_zeros_before(3, i);
    while (file_exists("ASDIR/AUCTIONS/" + aID_string + "/START_" + aID_string + ".txt")) {
        if (file_exists("ASDIR/USERS/" + uID_string + "/HOSTED/" + aID_string + ".txt")) {
            int auction_state = 1;

            if (auction_expired_time(i) != -1) {
                close(i);
                auction_state = 0;
            } 
            else if (file_exists("ASDIR/AUCTIONS/" + aID_string + "/END_" + aID_string + ".txt")) {
                auction_state = 0;
            }

            auctions_states.push_back({aID_string, auction_state});
        }

        i++;
        aID_string = add_zeros_before(3, i);
    }

    return auctions_states;
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
 * list of auctions states
 */
vector<AuctionState> list_auctions() {
    vector<AuctionState> auctions_states;

    int i = 1;
    string aID_string = add_zeros_before(3, i);
    while (file_exists("ASDIR/AUCTIONS/" + aID_string + "/START_" + aID_string + ".txt")) {
        int auction_state = 1;

        if (auction_expired_time(i) != -1) {
            close(i);
            auction_state = 0;
        } 
        else if (file_exists("ASDIR/AUCTIONS/" + aID_string + "/END_" + aID_string + ".txt")) {
            auction_state = 0;
        }

        auctions_states.push_back({aID_string, auction_state});

        i++;
        aID_string = add_zeros_before(3, i);
    }

    return auctions_states;
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
 * {0, cena} - successfully
 * {-1, null} - no file (or other problem)
 */
int show_asset(int aID) {
    return 0;
}


// > -------------------- { Auctions Functions } -------------------- <

/*
 * Return:
 * created AID - when created successfully
 * -1 - could not create auction
 * -2 - not logged in
 */
int open_auction(int uID, const string& name, int start_value, int time_active, const string& fname, int file_size, const string& fdata) {
    string uID_string = to_string(uID);
    
    string login_file_name = "ASDIR/USERS/" + uID_string + "/" + uID_string + "_login.txt";

    if (!file_exists(login_file_name)) {
        return -2;
    }

    int auctions_count = count_folder_entries("ASDIR/AUCTIONS");
    if (auctions_count >= 999) {
        return -1;
    }

    if (name.find(' ') < name.length()) {
        return -1;
    }

    if (start_value < 0) {
        return -1;
    }

    if (file_size > 99999999) { // TODO fazer isto como deve de ser 
        return -1;
    }

    // TODO fazer verificações de não ser possível criar auction tipo o name ser só uma palavra

    int aID = auctions_count + 1;
    string aID_string = add_zeros_before(3, aID);
    
    string start_file_name = "ASDIR/AUCTIONS/" + aID_string + "/START_" + aID_string + ".txt";
    string asset_file_name = "ASDIR/AUCTIONS/" + aID_string + "/" + fname;
    string host_file_name = "ASDIR/USERS/" + uID_string + "/HOSTED/" + aID_string + ".txt";

    create_file(start_file_name);
    create_file(asset_file_name);
    create_file(host_file_name);

    time_t full_time;
    time(&full_time);

    struct tm *current_time = gmtime(&full_time);

    string time_string = add_zeros_before(4, current_time->tm_year + 1900) + "-" + 
            add_zeros_before(2, current_time->tm_mon + 1) + "-" + add_zeros_before(2, current_time->tm_mday) + " " +
            add_zeros_before(2, current_time->tm_hour) + ":" + add_zeros_before(2, current_time->tm_min) + ":" +
            add_zeros_before(2, current_time->tm_sec);

    string start_file_content = uID_string + " " + name + " " + fname + " " + to_string(start_value) + 
            " " + to_string(time_active) + " " + time_string + " " + to_string(full_time);
    write_on_file(start_file_name, start_file_content, true);

    write_on_file(asset_file_name, fdata, true);

    return aID;
}

/*
 * Return:
 * time it closed - when already closed
 * -1 - not closed
 */
int auction_expired_time(int aID) {
    string aID_string = add_zeros_before(3, aID);
    string start_file_name = "ASDIR/AUCTIONS/" + aID_string + "/START_" + aID_string + ".txt";
    string start_file_contents;
    read_from_file(start_file_name, start_file_contents);

    vector<string> contents_arguments = split_string(start_file_contents, ' ');

    int start_time = stoi(contents_arguments[7]);
    int active_time = stoi(contents_arguments[4]);

    time_t current_time;
    time(&current_time);

    return current_time >= start_time + active_time ? start_time + active_time : -1;
}

int close(int aID) {
    string aID_string = add_zeros_before(3, aID);
    string end_file_name = "ASDIR/AUCTIONS/" + aID_string + "/END_" + aID_string + ".txt";
    if (file_exists(end_file_name)) {
        return -4;
    }

    create_file(end_file_name);

    int return_code = 0;
    time_t end_time;

    int auction_expire_time = auction_expired_time(aID);
    if (auction_expire_time > 0) {
        return_code = -4;
        end_time = auction_expire_time;
    }
    else {
        time(&end_time);
    }

    struct tm* end_date_time = gmtime(&end_time);
    string end_date_time_string = add_zeros_before(4, end_date_time->tm_year + 1900) + "-" + 
            add_zeros_before(2, end_date_time->tm_mon + 1) + "-" + add_zeros_before(2, end_date_time->tm_mday) + " " +
            add_zeros_before(2, end_date_time->tm_hour) + ":" + add_zeros_before(2, end_date_time->tm_min) + ":" +
            add_zeros_before(2, end_date_time->tm_sec);

    string end_file_contents = end_date_time_string + " " + to_string(end_time);
    write_on_file(end_file_name, end_file_contents, true);

    return return_code;
}

/*
 * Return:
 * 0 - when closed successfully
 * -1 - not logged in
 * -2 - aID does not exist
 * -3 - auction is not owned by user uID
 * -4 - auction aID has already finished
 */
int close(int uID, int aID) {
    string uID_string = to_string(uID);
    string aID_string = add_zeros_before(3, aID);

    string login_file_name = "ASDIR/USERS/" + uID_string + "/" + uID_string + "_login.txt";
    if (!file_exists(login_file_name)) {
        return -1;
    }

    string start_file_name = "ASDIR/AUCTIONS/" + aID_string + "/START_" + aID_string + ".txt";
    if (!file_exists(start_file_name)) {
        return -2;
    }

    string host_file_name = "ASDIR/USERS/" + uID_string + "/HOSTED/" + aID_string + ".txt";
    if (!file_exists(host_file_name)) {
        return -3;
    }

    return close(aID);
}


// > ---------------------- { Bids Functions } ---------------------- <

int get_higher_bid(int aID) {
    struct dirent** filelist;

    string aID_string = add_zeros_before(3, aID);

    string dirname = "ASDIR/AUCTIONS/" + aID_string + "/BIDS/";
    int n_entries = scandir(dirname.c_str(), &filelist, 0, alphasort);
    if (n_entries - 2 <= 0) {
        return 0;
    }

    n_entries--;
    string file_name = filelist[n_entries]->d_name;
    file_name = file_name.substr(0, file_name.length() - 4);

    return stoi(file_name);
}

/*
 * Return:
 * 0 - accepted
 * -1 - auction aID is not active
 * -2 - refused because a larger bid
 * -3 - user tries to make a bid in an auction hosted by himself
 */
int bid(int uID, int aID, int value) {
    string aID_string = add_zeros_before(3, aID);

    string start_file_name = "ASDIR/AUCTIONS/" + aID_string + "/START_" + aID_string + ".txt";
    if (!file_exists(start_file_name)) {
        return -1;
    }

    string end_file_name = "ASDIR/AUCTIONS/" + aID_string + "/END_" + aID_string + ".txt";
    if (file_exists(end_file_name)) {
        return -1;
    }

    int higher_bid = get_higher_bid(aID);
    if (value <= higher_bid) {
        return -2;
    }

    string uID_string = to_string(uID);

    string host_file_name = "ASDIR/USERS/" + uID_string + "/HOSTED/" + aID_string + ".txt";
    if (file_exists(host_file_name)) {
        return -3;
    }

    string bidder_file_name = "ASDIR/USERS/" + uID_string + "/BIDDED/" + aID_string + ".txt";
    create_file(bidder_file_name);

    string bid_file_name = "ASDIR/AUCTIONS/" + aID_string + "/BIDS/" + add_zeros_before(6, value) + ".txt";
    create_file(bid_file_name);
    
    time_t full_time;
    time(&full_time);

    struct tm *current_time = gmtime(&full_time);

    string time_string = add_zeros_before(4, current_time->tm_year + 1900) + "-" + 
            add_zeros_before(2, current_time->tm_mon + 1) + "-" + add_zeros_before(2, current_time->tm_mday) + " " +
            add_zeros_before(2, current_time->tm_hour) + ":" + add_zeros_before(2, current_time->tm_min) + ":" +
            add_zeros_before(2, current_time->tm_sec);
    
    string contents_file_name = uID_string + " " + to_string(value) + " " + time_string + " " + to_string(full_time);

    write_on_file(bid_file_name, contents_file_name, true);

    return 0;
}