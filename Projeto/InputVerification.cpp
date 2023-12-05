#include <string>
#include <vector>

using namespace std;

#define PASSWORD_LENGTH 8

// #------------------------------------------------------------------#
// |                        Input Verification                        |
// #------------------------------------------------------------------#

bool is_unexpected_login_input(vector<string> arguments) {
    if (arguments.size() != 2) {
        return true;
    }

    int uID = atoi(arguments[0].c_str());
    if (uID == 0 || uID > 999999 || uID < 100000) { // TODO Fazer isto com digit count quando houver paciencia
        return true;
    }

    if (arguments[1].length() != PASSWORD_LENGTH) {
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

    int uID = atoi(arguments[0].c_str());
    if (uID == 0 || uID > 999999 || uID < 100000) { // TODO Fazer isto com digit count quando houver paciencia
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