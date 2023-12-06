#include <string>
#include <vector>

using namespace std;

#define PASSWORD_LENGTH 8

// #------------------------------------------------------------------#
// |                        Input Verification                        |
// #------------------------------------------------------------------#

bool is_unexpected_uid(string uID_string) {
    if (uID_string.length() != 6)
        return true;

     for (char c : uID_string) {
        if (!isdigit(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_aid(string aID_string) {
    if (aID_string.length() != 3)
        return true;

    for (char c : aID_string) {
        if (!isdigit(c)) {
            return true;
        }
    }

    int aID_number = atoi(aID_string.c_str());
    if (aID_number >= 1) { 
        return true;
    }

    return false;
}

bool is_unexpected_password(string password) {
    if (password.length() != 8)
        return true;

    for (char c : password) {
        if (!isalnum(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_value(string value) {
    if (value.length() != 6)
        return true;

    for (char c : value) {
        if (!isdigit(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_login_input(vector<string> arguments) {
    if (arguments.size() != 2) {
        return true;
    }

    if (is_unexpected_uid(arguments[0])) {
        return true;
    }

    if (is_unexpected_password(arguments[1])) {
        return true;
    }

    return false;
}

bool is_unexpected_logout_input(vector<string> arguments) {
    return is_unexpected_login_input(arguments); // Same parameters
}

bool is_unexpected_unregister_input(vector<string> arguments) {
    return is_unexpected_login_input(arguments); // Same parameters
}

bool is_unexpected_list_auctions_target_input(vector<string> arguments) {
    if (arguments.size() != 1) {
        return true;
    }

    if (is_unexpected_uid(arguments[0])) {
        return true;
    }

    return false;
}

bool is_unexpected_list_auctions_input(vector<string> arguments) {
    return arguments.size() != 0;
}

bool is_unexpected_list_bids_target_input(vector<string> arguments) {
    return is_unexpected_list_auctions_target_input(arguments);
}

bool is_unexpected_show_record_input(vector<string> arguments) {
    if (arguments.size() != 1) {
        return true;
    }

    if (is_unexpected_aid(arguments[0])) {
        return true;
    }

    return false;
}

bool is_unexpected_close_input(vector<string> arguments) {
    if (arguments.size() != 3) {
        return true;
    }

    if (is_unexpected_uid(arguments[0])) {
        return true;
    }

    if (is_unexpected_password(arguments[1])) {
        return true;
    }

    if (is_unexpected_aid(arguments[2])) {
        return true;
    }

    return false;
}

bool is_unexpected_show_asset_input(vector<string> arguments) {
    if (arguments.size() != 1) {
        return true;
    }

    if (is_unexpected_aid(arguments[0])) {
        return true;
    }

    return false;
}

bool is_unexpected_bid_input(vector<string> arguments) {
    if (arguments.size() != 4) {
        return true;
    }

    if (is_unexpected_uid(arguments[0])) {
        return true;
    }

    if (is_unexpected_password(arguments[1])) {
        return true;
    }

    if (is_unexpected_aid(arguments[2])) {
        return true;
    }

    if (is_unexpected_value(arguments[3])) {
        return true;
    }

    return false;
}
