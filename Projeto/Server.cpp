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

string LOGIN_REPLY = "RLI";
string OK_REPLY = "OK";
string NOT_OK_REPLY = "NOK";
string REGISTERED_REPLY = "REG";

string ERROR_REPLY = "ERR";

string EXCEPTION_LIKE_ERROR = "IT REACHED A SPOT IT NEVER COULD. SERVER CRASHED - Tiago\'s message lol";

// #------------------------------------------------------------------#
// |                       Functions Definition                       |
// #------------------------------------------------------------------#

vector<string> split_string(string input, char delimiter);

string process_request(string request);
string process_request(vector<string> request_arguments);
string process_login_attempt(vector<string> request_arguments);

bool is_unexpected_login_input(vector<string> arguments);

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

    return ERROR_REPLY + "\n";
}

string process_login_attempt(vector<string> request_arguments) {
    if (is_unexpected_login_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    string password = request_arguments[1];
    int login_result = login(uID, password);

    switch (login_result) {
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

// #------------------------------------------------------------------#
// |                           Server setup                           |
// #------------------------------------------------------------------#

/*int main() {

    // Ligar o servidor e ficar à escuta de cenas ig

    return 0;
}*/