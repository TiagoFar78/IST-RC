#include <string>
#include <vector>
#include <iostream>
#include "AuctionManager.h"

using namespace std;

// #-------------------------------------------------------------------#
// |                             Constants                             |
// #-------------------------------------------------------------------#

#define PORT "58028"

#define PASSWORD_LENGTH 8

string LOGIN_COMMAND = "LIN";
string LOGOUT_COMMAND = "LOU";
string UNREGISTER_COMMAND = "UNR";
string LIST_AUCTIONS_TARGET_COMMAND = "LMA";
string LIST_AUCTIONS_COMMAND = "LST";
string LIST_BIDS_TARGET_COMMAND = "LMB";

string LOGIN_REPLY = "RLI";
string LOGOUT_REPLY = "RLO";
string UNREGISTER_REPLY = "RUR";
string LIST_AUCTIONS_TARGET_REPLY = "RMA";
string LIST_AUCTIONS_REPLY = "RLS";
string LIST_BIDS_TARGET_REPLY = "RMB";

string OK_REPLY = "OK";
string NOT_OK_REPLY = "NOK";
string REGISTERED_REPLY = "REG";
string NOT_REGISTERED_REPLY = "UNR";
string NOT_LOGGED_IN_REPLY = "NLG";

string ERROR_REPLY = "ERR";

string EXCEPTION_LIKE_ERROR = "IT REACHED A SPOT IT NEVER COULD. SERVER CRASHED - Tiago\'s message lol";

// #------------------------------------------------------------------#
// |                       Functions Definition                       |
// #------------------------------------------------------------------#

vector<string> split_string(string input, char delimiter);

string process_request(string request);
string process_request(vector<string> request_arguments);
string process_login_attempt(vector<string> request_arguments);
string process_logout_attempt(vector<string> request_arguments);
string process_unregister_attempt(vector<string> request_arguments);
string process_list_auctions_target(vector<string> request_arguments);
string process_list_auctions(vector<string> request_arguments);
string process_list_bids_target(vector<string> request_arguments);

bool is_unexpected_login_input(vector<string> arguments);
bool is_unexpected_logout_input(vector<string> arguments);
bool is_unexpected_unregister_input(vector<string> arguments);
bool is_unexpected_list_auctions_target_input(vector<string> arguments);
bool is_unexpected_list_auctions_input(vector<string> arguments);
bool is_unexpected_list_bids_target_input(vector<string> arguments);

// #------------------------------------------------------------------#
// |                         Useful Functions                         |
// #------------------------------------------------------------------#

// ISTO PRECISA DE ESTAR COMENTADO PARA CORRER OS TESTES E DESCOMENTADO PARA O RESTO DO FUNCIONAMENTO IG
/*vector<string> split_string(string input, char delimiter) {
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
}*/

// #-------------------------------------------------------------------#
// |                          Request Process                          |
// #-------------------------------------------------------------------#

string process_request(string request) {
    return process_request(split_string(request, ' '));
}

string process_request(vector<string> request_arguments) {
    if (request_arguments.size() < 1) {
        return ERROR_REPLY + "\n";
    }

    string command = request_arguments[0];
    request_arguments.erase(request_arguments.begin());

    if (command == LOGIN_COMMAND) {
        return LOGIN_REPLY + " " + process_login_attempt(request_arguments) + "\n";
    }
    else if (command == LOGOUT_COMMAND) {
        return LOGOUT_REPLY + " " + process_logout_attempt(request_arguments) + "\n";
    }
    else if (command == UNREGISTER_COMMAND) {
        return UNREGISTER_REPLY + " " + process_unregister_attempt(request_arguments) + "\n";
    }
    else if (command == LIST_AUCTIONS_TARGET_COMMAND) {
        return LIST_AUCTIONS_TARGET_REPLY + " " + process_list_auctions_target(request_arguments) + "\n";
    }
    else if (command == LIST_AUCTIONS_COMMAND) {
        return LIST_AUCTIONS_REPLY + " " + process_list_auctions(request_arguments) + "\n";
    }
    else if (command == LIST_BIDS_TARGET_COMMAND) {
        return LIST_BIDS_TARGET_REPLY + " " + process_list_bids_target(request_arguments) + "\n";
    }

    return ERROR_REPLY + "\n";
}

string process_login_attempt(vector<string> request_arguments) {
    if (is_unexpected_login_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];
    int returnCode = login(uID, password);

    switch (returnCode) {
    case 0:
        return OK_REPLY;
    case -1:
        return NOT_OK_REPLY;
    case 1:
        return REGISTERED_REPLY;
    }

    exit(1);
    return EXCEPTION_LIKE_ERROR;
}

string process_logout_attempt(vector<string> request_arguments) {
    if (is_unexpected_logout_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];
    
    if (is_registered(uID)) {
        if (!is_password_correct(uID, password)) {
            return NOT_OK_REPLY;
        }
    }

    int returnCode = logout(uID);
    switch (returnCode) {
    case 0:
        return OK_REPLY;
    case -1:
        return NOT_OK_REPLY;
    case -2:
        return NOT_REGISTERED_REPLY;
    }

    exit(1);
    return EXCEPTION_LIKE_ERROR;
}

string process_unregister_attempt(vector<string> request_arguments) {
    if (is_unexpected_unregister_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];

    if (is_registered(uID)) {
        if (!is_password_correct(uID, password)) {
            return NOT_OK_REPLY;
        }
    }

    int returnCode = unregister(uID);
    switch (returnCode) {
    case 0:
        return OK_REPLY;
    case -1:
        return NOT_OK_REPLY;
    case -2:
        return NOT_REGISTERED_REPLY;
    }

    exit(1);
    //return EXCEPTION_LIKE_ERROR;
}

string process_list_auctions_target(vector<string> request_arguments) {
    if (is_unexpected_list_auctions_target_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());

    if (!is_logged_in(uID)) {
        return NOT_LOGGED_IN_REPLY;
    }

    vector<AuctionState> auctions = list_auctions_target(uID);

    if (auctions.size() == 0) {
        return NOT_OK_REPLY;
    }

    string auctions_string = "";
    for (int i = 0; i < auctions.size(); i++) {
        auctions_string += " " + auctions[i].aID_string + " " + to_string(auctions[i].state);
    }

    return OK_REPLY + auctions_string;
}

string process_list_auctions(vector<string> request_arguments) {
    if (is_unexpected_list_auctions_input(request_arguments)) {
        return ERROR_REPLY;
    }

    vector<AuctionState> auctions = list_auctions();

    if (auctions.size() == 0) {
        return NOT_OK_REPLY;
    }

    string auctions_string = "";
    for (int i = 0; i < auctions.size(); i++) {
        auctions_string += " " + auctions[i].aID_string + " " + to_string(auctions[i].state);
    }

    return OK_REPLY + auctions_string;
}

string process_list_bids_target(vector<string> request_arguments) {
    if (is_unexpected_list_bids_target_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());

    if (!is_logged_in(uID)) {
        return NOT_LOGGED_IN_REPLY;
    }

    vector<AuctionState> auctions = list_bids_target(uID);

    if (auctions.size() == 0) {
        return NOT_OK_REPLY;
    }

    string auctions_string = "";
    for (int i = 0; i < auctions.size(); i++) {
        auctions_string += " " + auctions[i].aID_string + " " + to_string(auctions[i].state);
    }

    return OK_REPLY + auctions_string;
}

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

// #------------------------------------------------------------------#
// |                           Server setup                           |
// #------------------------------------------------------------------#

/*int main() {

    // Ligar o servidor e ficar à escuta de cenas ig

    return 0;
}*/