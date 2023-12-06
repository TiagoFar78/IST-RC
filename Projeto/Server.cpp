#include <string>
#include <vector>
#include <iostream>
#include "AuctionManager.h"
#include "InputVerification.h"

using namespace std;

// #-------------------------------------------------------------------#
// |                             Constants                             |
// #-------------------------------------------------------------------#

#define PORT "58028"

string LOGIN_COMMAND = "LIN";
string LOGOUT_COMMAND = "LOU";
string UNREGISTER_COMMAND = "UNR";
string LIST_AUCTIONS_TARGET_COMMAND = "LMA";
string LIST_AUCTIONS_COMMAND = "LST";
string LIST_BIDS_TARGET_COMMAND = "LMB";
string SHOW_RECORD_COMMAND = "SRC";
string OPEN_COMMAND = "OPA";
string CLOSE_COMMAND = "CLS";
string BID_COMMAND = "BID";

string LOGIN_REPLY = "RLI";
string LOGOUT_REPLY = "RLO";
string UNREGISTER_REPLY = "RUR";
string LIST_AUCTIONS_TARGET_REPLY = "RMA";
string LIST_AUCTIONS_REPLY = "RLS";
string LIST_BIDS_TARGET_REPLY = "RMB";
string SHOW_RECORD_REPLY = "RRC";
string OPEN_REPLY = "ROA";
string CLOSE_REPLY = "RCL";
string BID_REPLY = "RBD";

string OK_REPLY = "OK";
string NOT_OK_REPLY = "NOK";
string REGISTERED_REPLY = "REG";
string NOT_REGISTERED_REPLY = "UNR";
string NOT_LOGGED_IN_REPLY = "NLG";
string AUCTION_ID_DOES_NOT_EXIST_REPLY = "EAU";
string NOT_AUCTION_OWNER_REPLY = "EOW";
string AUCTION_ENDED_REPLY = "END";
string BID_ACCEPTED_REPLY = "ACC";
string LOW_BID_REPLY = "REF";
string BID_ON_OWN_AUCTION_REPLY = "ILG";

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
string process_show_record(vector<string> request_arguments);
string process_open_attempt(vector<string> request_arguments);
string process_close_attempt(vector<string> request_arguments);
string process_bid_attempt(vector<string> request_arguments);

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
// |                          Process Request                          |
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
    else if (command == SHOW_RECORD_COMMAND) {
        return SHOW_RECORD_REPLY + " " + process_show_record(request_arguments) + "\n";
    }
    else if (command == OPEN_COMMAND) {
        return OPEN_REPLY + " " + process_open_attempt(request_arguments) + "\n";
    }
    else if (command == CLOSE_COMMAND) {
        return CLOSE_REPLY + " " + process_close_attempt(request_arguments) + "\n";
    }
    else if (command == BID_COMMAND) {
        return BID_REPLY + " " + process_bid_attempt(request_arguments) + "\n";
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

string process_show_record(vector<string> request_arguments) {
    if (is_unexpected_show_record_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int aID = atoi(request_arguments[0].c_str());

    vector<string> start_details = get_auction_start_details(aID);
    if (start_details.size() == 0) {
        return NOT_OK_REPLY;
    }

    string record_string = " " + start_details[0] + " " + start_details[1] + " " + start_details[2] + " " + 
        start_details[3] + " " + start_details[5] + " " + start_details[6] + " " + start_details[4];

    vector<vector<string>> bids_details = get_auction_bids_details(aID);

    if (bids_details.size() != 0) {
        for (int i = 0; i < bids_details.size(); i++) {
            record_string += " B " + bids_details[i][0] + " " + bids_details[i][1] + " " + bids_details[i][2] + " " +
                bids_details[i][3] + " " + bids_details[i][4];
        }
    }

    vector<string> end_details = get_auction_end_details(aID);
    if (end_details.size() != 0) {
        record_string += " E " + end_details[0] + " " + end_details[1] + " " + end_details[2];
    }

    return OK_REPLY + record_string;
}

string process_open_attempt(vector<string> request_arguments) {
    if (is_unexpected_open_input(request_arguments)) {
        return ERROR_REPLY;
    }

    // ler esta bomba de alguma forma
    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];
    string name = request_arguments[2];
    int value = atoi(request_arguments[3].c_str());
    int time_active = atoi(request_arguments[4].c_str());
    string file_name = request_arguments[5];
    int file_size = atoi(request_arguments[6].c_str());
    string file_data = request_arguments[7];

    if (!is_password_correct(uID, password)) {
        return NOT_OK_REPLY;
    }

    int return_code = open_auction(uID, name, value, time_active, file_name, file_size, file_data);
    if (return_code == -1) {
        return NOT_OK_REPLY;
    }
    else if (return_code == -2) {
        return NOT_LOGGED_IN_REPLY;
    }

    // deposita o conteudo no ficheiro???

    return OK_REPLY + " " + to_string(return_code);
}

string process_close_attempt(vector<string> request_arguments) {
    if (is_unexpected_close_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];
    int aID = atoi(request_arguments[2].c_str());

    if (!is_password_correct(uID, password)) {
        return NOT_OK_REPLY;
    }

    int return_code = close(uID, aID);
    if (return_code == -1) {
        return NOT_LOGGED_IN_REPLY;
    }
    else if (return_code == -2) {
        return AUCTION_ID_DOES_NOT_EXIST_REPLY;
    }
    else if (return_code == -3) {
        return NOT_AUCTION_OWNER_REPLY;
    }
    else if (return_code == -4) {
        return AUCTION_ENDED_REPLY;
    }

    return OK_REPLY;
}

string process_bid_attempt(vector<string> request_arguments) {
    if (is_unexpected_bid_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];
    int aID = atoi(request_arguments[2].c_str());
    int value = atoi(request_arguments[3].c_str());

    if (!is_password_correct(uID, password)) {
        return NOT_OK_REPLY;
    }

    if (!is_logged_in(uID)) {
        return NOT_LOGGED_IN_REPLY;
    }

    int return_code = bid(uID, aID, value);
    if (return_code == -1) {
        return NOT_OK_REPLY;
    }
    else if (return_code == -2) {
        return LOW_BID_REPLY;
    }
    else if (return_code == -3) {
        return BID_ON_OWN_AUCTION_REPLY;
    }

    return BID_ACCEPTED_REPLY;
}

// #------------------------------------------------------------------#
// |                           Server setup                           |
// #------------------------------------------------------------------#

/*int main() {

    // Ligar o servidor e ficar à escuta de cenas ig

    return 0;
}*/