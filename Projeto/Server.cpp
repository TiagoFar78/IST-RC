#include <string>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include "AuctionManager.h"
#include "ArgumentVerification.h"

#include <unistd.h>
#include <cstring>
#include <csignal>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

// #-------------------------------------------------------------------#
// |                             Constants                             |
// #-------------------------------------------------------------------#

#define BUFFER_SIZE 2048
#define COMMAND_BUFFER_SIZE 3
#define UDP_BUFFER_SIZE 6000

string PORT = "58028";
bool is_tcp_socket = false;
bool is_verbose_mode = false;
string current_uid = "";
string current_command = "";
string current_reply = "";
int current_fd;

string LOGIN_COMMAND = "LIN";
string LOGOUT_COMMAND = "LOU";
string UNREGISTER_COMMAND = "UNR";
string LIST_AUCTIONS_TARGET_COMMAND = "LMA";
string LIST_AUCTIONS_COMMAND = "LST";
string LIST_BIDS_TARGET_COMMAND = "LMB";
string SHOW_RECORD_COMMAND = "SRC";
string OPEN_COMMAND = "OPA";
string CLOSE_COMMAND = "CLS";
string SHOW_ASSET_COMMAND = "SAS";
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
string SHOW_ASSET_REPLY = "RSA";
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

string SERVER_SIDE_ERROR = "-> A server file error occured. Try again";

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
string process_show_asset(vector<string> request_arguments);
string process_bid_attempt(vector<string> request_arguments);

void show_process_in_terminal(struct sockaddr_in addr, socklen_t addrlen);
void send_error_message(string message);

// #------------------------------------------------------------------#
// |                         Useful Functions                         |
// #------------------------------------------------------------------#

int setSocketTimeout(int socket, int timeoutSeconds) {
    struct timeval timeout;
    timeout.tv_sec = timeoutSeconds;
    timeout.tv_usec = 0;

    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        cout << "Error setting receive timeout\n";
        return -1;
    }
    return 0;
}

string add_zeros_before_server(int zeros_amount, int number) {
    string s = "";
    string number_string = to_string(number);
    int zeros_to_add = zeros_amount - number_string.length();
    for (int i = 0; i < zeros_to_add; i++) {
        s += to_string(0);
    }

    s += number_string;

    return s;
}

// #-------------------------------------------------------------------#
// |                          Process Request                          |
// #-------------------------------------------------------------------#

string process_request(string request) {
    if (request[request.length() - 1] != '\n') {
        return ERROR_REPLY + "\n";
    }

    request.pop_back(); // Removes last \n
    return process_request(split_string(request, ' '));
}

string process_request(vector<string> request_arguments) {
    if (request_arguments.size() < 1) {
        return ERROR_REPLY + "\n";
    }

    string command = request_arguments[0];
    request_arguments.erase(request_arguments.begin());

    if (command == LOGIN_COMMAND && !is_tcp_socket) {
        current_command = command;
        return LOGIN_REPLY + " " + process_login_attempt(request_arguments) + "\n";
    }
    else if (command == LOGOUT_COMMAND && !is_tcp_socket) {
        current_command = command;
        return LOGOUT_REPLY + " " + process_logout_attempt(request_arguments) + "\n";
    }
    else if (command == UNREGISTER_COMMAND && !is_tcp_socket) {
        current_command = command;
        return UNREGISTER_REPLY + " " + process_unregister_attempt(request_arguments) + "\n";
    }
    else if (command == LIST_AUCTIONS_TARGET_COMMAND && !is_tcp_socket) {
        current_command = command;
        return LIST_AUCTIONS_TARGET_REPLY + " " + process_list_auctions_target(request_arguments) + "\n";
    }
    else if (command == LIST_AUCTIONS_COMMAND && !is_tcp_socket) {
        current_command = command;
        return LIST_AUCTIONS_REPLY + " " + process_list_auctions(request_arguments) + "\n";
    }
    else if (command == LIST_BIDS_TARGET_COMMAND && !is_tcp_socket) {
        current_command = command;
        return LIST_BIDS_TARGET_REPLY + " " + process_list_bids_target(request_arguments) + "\n";
    }
    else if (command == SHOW_RECORD_COMMAND && !is_tcp_socket) {
        current_command = command;
        return SHOW_RECORD_REPLY + " " + process_show_record(request_arguments) + "\n";
    }
    else if (command == OPEN_COMMAND && is_tcp_socket) {
        current_command = command;
        return OPEN_REPLY + " " + process_open_attempt(request_arguments) + "\n";
    }
    else if (command == CLOSE_COMMAND && is_tcp_socket) {
        current_command = command;
        return CLOSE_REPLY + " " + process_close_attempt(request_arguments) + "\n";
    }
    else if (command == SHOW_ASSET_COMMAND && is_tcp_socket) {
        current_command = command;
        return SHOW_ASSET_REPLY + " " + process_show_asset(request_arguments) + "\n";
    }
    else if (command == BID_COMMAND && is_tcp_socket) {
        current_command = command;
        return BID_REPLY + " " + process_bid_attempt(request_arguments) + "\n";
    }

    return ERROR_REPLY + "\n";
}

string process_login_attempt(vector<string> request_arguments) {
    if (is_unexpected_login_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);
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

    return SERVER_SIDE_ERROR;
}

string process_logout_attempt(vector<string> request_arguments) {
    if (is_unexpected_logout_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);
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

    return SERVER_SIDE_ERROR;
}

string process_unregister_attempt(vector<string> request_arguments) {
    if (is_unexpected_unregister_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);
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

    return SERVER_SIDE_ERROR;
}

string process_list_auctions_target(vector<string> request_arguments) {
    if (is_unexpected_list_auctions_target_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);

    if (!is_registered(uID)) {
        return NOT_OK_REPLY;
    }

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
    current_uid = to_string(uID);

    if (!is_registered(uID)) {
        return NOT_OK_REPLY;
    }

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

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);
    string password = request_arguments[1];
    string name = request_arguments[2];
    int value = atoi(request_arguments[3].c_str());
    int time_active = atoi(request_arguments[4].c_str());
    string file_name = request_arguments[5];
    int file_size = atoi(request_arguments[6].c_str());
    string file_data = "";
    for (int i = 7; i < request_arguments.size(); i++) {
        file_data += request_arguments[i] + " ";
    }
    file_data.pop_back(); // Removes last space

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
    else if (return_code >= 0) {
        return OK_REPLY + " " + add_zeros_before_server(3, return_code);
    }

    return SERVER_SIDE_ERROR;
}

string process_close_attempt(vector<string> request_arguments) {
    if (is_unexpected_close_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);
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
    else if (return_code == 0) {
        return OK_REPLY;
    }

    return SERVER_SIDE_ERROR;
}

string process_show_asset(vector<string> request_arguments) {
    if (is_unexpected_show_asset_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int aID = atoi(request_arguments[0].c_str());

    string return_code = show_asset(aID);
    if (return_code.length() == 0) {
        return NOT_OK_REPLY;
    }

    return OK_REPLY + " " + return_code;
}

string process_bid_attempt(vector<string> request_arguments) {
    if (is_unexpected_bid_input(request_arguments)) {
        return ERROR_REPLY;
    }

    int uID = atoi(request_arguments[0].c_str());
    current_uid = to_string(uID);
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
    else if (return_code == 0) {
        return BID_ACCEPTED_REPLY;
    }

    return SERVER_SIDE_ERROR;
}

// #------------------------------------------------------------------#
// |                           Server setup                           |
// #------------------------------------------------------------------#

// >---------------------------{ TCP }---------------------------<

struct addrinfo *tcp_res;

int tcp_setup() {
    struct addrinfo hints;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }

    memset(&hints, 0, sizeof(hints)); 
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, PORT.c_str(), &hints, &tcp_res);
    if (errcode != 0) {
        close(fd);
        return -1;
    }

    ssize_t n = bind(fd, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (n == -1) {
        close(fd);
        freeaddrinfo(tcp_res);
        return -1;
    }

    int socket_timeout = setSocketTimeout(fd, 5);
    if (socket_timeout == -1) {
        close(fd);
        freeaddrinfo(tcp_res);
        return -1;
    }

    if (listen(fd, 5) == -1) {
        close(fd);
        freeaddrinfo(tcp_res);
        return -1;
    }

    return fd;
}

void execute_tcp(int fd) {
    current_fd = fd;

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int newfd = accept(fd, (struct sockaddr*)&addr, &addrlen);
    if (newfd == -1) {
        cout << "There was a problem setting up the new tcp file descriptor\n";
        exit(1); 
    }

    int socket_timeout = setSocketTimeout(newfd, 5);
    if(socket_timeout == -1) {
        cout << "There was a problem setting up the new tcp file descriptor\n";
        exit(1);
    }

    current_fd = newfd;

    char buffer[BUFFER_SIZE + 1];
    memset(buffer, 0, BUFFER_SIZE + 1);
    char command_buffer[COMMAND_BUFFER_SIZE + 1];
    memset(command_buffer, 0, COMMAND_BUFFER_SIZE + 1);

    memset(command_buffer, 0, COMMAND_BUFFER_SIZE);
    ssize_t n = read(newfd, command_buffer, COMMAND_BUFFER_SIZE);
    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            send_error_message("Timeout in receiving TCP message\n");
            return;
        } else {
            send_error_message("Error receiving TCP message\n");
            return;
        }
    }

    string command;
    command += command_buffer;
    
    n = BUFFER_SIZE;
    if (command == OPEN_COMMAND) {
        int file_size_index = 7;
        int total_data = 0;
        int accumulated_read = -1;
        bool found_size = false;
        while (total_data > accumulated_read) {
            memset(buffer, 0, BUFFER_SIZE);
            n = read(newfd, buffer, BUFFER_SIZE);
            if (n == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    send_error_message("Timeout in receiving TCP message\n");
                    return;
                } else {
                    send_error_message("Error receiving TCP message\n");
                    return;
                }
            }

            string substring_buffer(buffer, buffer + n);
            command += substring_buffer;

            if (found_size) {
                accumulated_read += n;
            }
            else {
                found_size = true;
                vector<string> command_arguments = split_string(command, ' ');
                if (command_arguments.size() >= file_size_index + 1) {
                    int file_size = atoi(command_arguments[file_size_index].c_str());
                    if (file_size == 0) {
                        break;
                    }

                    int non_data_arguments_length = 0;
                    for (int i = 0; i < file_size_index + 1; i++) {
                        non_data_arguments_length += command_arguments[i].length() + 1;
                    }

                    accumulated_read = command.length() - non_data_arguments_length;
                    total_data = file_size;
                }
            }
        }
    }
    else {
        while (n == BUFFER_SIZE) {
            memset(buffer, 0, BUFFER_SIZE);
            n = read(newfd, buffer, BUFFER_SIZE);
            if (n == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    send_error_message("Timeout in receiving TCP message\n");
                    return;
                } else {
                    send_error_message("Error receiving TCP message\n");
                    return;
                }
            }

            string substring_buffer(buffer, buffer + n);
            command += substring_buffer;
        }
    }

    string reply = process_request(command);
    current_reply = reply.substr(0, reply.length() <= 7 ? reply.length() : 7);

    n = write(newfd, reply.c_str(), reply.length()); 
    if (n == -1) {
        send_error_message("Error sending TCP message\n");
        return;
    }

    close(newfd);

    if (is_verbose_mode) {
        show_process_in_terminal(addr, addrlen);
    }
}

// >---------------------------{ UDP }---------------------------<

struct addrinfo *udp_res;

int udp_setup() {
    struct addrinfo hints; 

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        return -1;
    }

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags=AI_PASSIVE;

    int errcode = getaddrinfo(NULL, PORT.c_str(), &hints, &udp_res);
    if (errcode != 0) {
        close(fd);
        return -1;
    }

    ssize_t n = bind(fd, udp_res->ai_addr, udp_res->ai_addrlen);
    if (n == -1) {
        close(fd);
        freeaddrinfo(udp_res);
        return -1;
    }

    int socket_timeout = setSocketTimeout(fd, 5);
    if(socket_timeout == -1) {
        close(fd);
        freeaddrinfo(udp_res);
    }

    return fd;
}

void execute_udp(int fd) {
    current_fd = fd;

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    
    char buffer[UDP_BUFFER_SIZE + 1];
    memset(buffer, 0, UDP_BUFFER_SIZE + 1);

    ssize_t n = recvfrom(fd, buffer, UDP_BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addrlen);
    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            send_error_message("Timeout in receiving udp message\n");
            return;
        } else {
            send_error_message("Error receiving UDP message\n");
            return;
        }
    }

    string buffer_string(buffer, buffer + n);
    string reply = process_request(buffer_string);
    current_reply = reply.substr(0, reply.length() <= 7 ? reply.length() : 7);

    n = sendto(fd, reply.c_str(), reply.length(), 0, (struct sockaddr *)&addr, addrlen);
    if (n == -1) {
        send_error_message("Error sending UDP message\n");
        return;
    }

    if (is_verbose_mode) {
        show_process_in_terminal(addr, addrlen);
    }
}

// >-------------------------{ Server }-------------------------<

void send_error_message(string message) {
    if (is_tcp_socket) {
        ssize_t n = write(current_fd, message.c_str(), message.length());
        if (n == -1) {
            cout << "An error occured sending an error message\n";
        }
    }
    else {
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        ssize_t n = sendto(current_fd, message.c_str(), message.length(), 0, (struct sockaddr *)&addr, addrlen);
        if (n == -1) {
            cout << "An error occured sending an error message\n";
        }
    }
}

void show_process_in_terminal(struct sockaddr_in addr, socklen_t addrlen) {
    char host[NI_MAXHOST + 1], service[NI_MAXSERV + 1];
    memset(host, 0, NI_MAXHOST + 1);
    memset(service, 0, NI_MAXSERV + 1);

    int errcode = getnameinfo((struct sockaddr*) &addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0);
    if (errcode != 0) {
        cout << "It was not possible to get client IP and port" << endl;
        return;
    }

    cout << endl;
    cout << "A new request was sent by " << host << ":" << service << endl;
    if (current_command == "") {
        cout << "An invalid command was used" << endl;
        cout << "And the server replied with: ERR" << endl;
        return;
    }

    cout << "The executed command was " << current_command;
    if (current_uid == "") {
        cout << endl;
    }
    else {
        cout << " by the user with id " << current_uid << endl;
    }

    if (current_reply.find('>') == -1) {
        cout << "And the server replied with: " << current_reply << endl;
    }
}

int setup_server_settings(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "-v")) {
            return -1;
        }

        is_verbose_mode = true;
    }
    else if (argc >= 3) {
        if (strcmp(argv[1], "-p")) {
            return 0;
        }

        if (atoi(argv[2]) == 0) {
            return -1;
        }
        PORT = argv[2];

        if (argc == 4) {
            if (strcmp(argv[3], "-v")) {
                return -1;
            }

            is_verbose_mode = true;
        }
    }

    return 0;
}

int udp_fd, tcp_fd;

void signal_handler(int signumber) {
    freeaddrinfo(udp_res);
    close(udp_fd);
    freeaddrinfo(tcp_res);
    close(tcp_fd);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (setup_server_settings(argc, argv) == -1) {
        cout << "Invalid arguments for starting server\n";
        return 0;
    }

    cout << "Abriu o server\n";

    signal(SIGCHLD, SIG_IGN);

    fd_set inputs, testfds;
    int out_fds;
    struct timeval timeout;

    tcp_fd = tcp_setup();
    udp_fd = udp_setup();
    if (tcp_fd == -1 || udp_fd == -1) {
        cout << "There was a problem setting up either tcp or udp file descriptor. Try again.\n";
        return 0;
    }

    FD_ZERO(&inputs); 
    FD_SET(udp_fd, &inputs); 
    FD_SET(tcp_fd, &inputs); 

    while (true) { 
        testfds = inputs;

        signal(SIGINT, signal_handler);

        memset((void *)&timeout, 0, sizeof(timeout));
        timeout.tv_sec=120;

        out_fds = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *) &timeout);

        switch (out_fds) {
            case 0:
                //printf("\n ---------------Timeout event-----------------\n");
                break;
            case -1:
                perror("select error");
                freeaddrinfo(tcp_res);
                close(tcp_fd);
                freeaddrinfo(udp_res);
                close(udp_fd);
                exit(1);
            default:
                if (FD_ISSET(udp_fd, &testfds)) {
                    pid_t pid = fork();
                    if (pid == -1) {
                        perror("fork error");
                    } else if (pid == 0) {
                        is_tcp_socket = false;
                        execute_udp(udp_fd);
                        exit(0);
                    }
                }
                else if (FD_ISSET(tcp_fd, &testfds)) { 
                    pid_t pid = fork();
                    if (pid == -1) {
                        perror("fork error");
                    } else if (pid == 0) {
                        is_tcp_socket = true;
                        execute_tcp(tcp_fd);
                        exit(0);
                    }
                }
                wait(NULL);
        }
    }

    freeaddrinfo(tcp_res);
    close(tcp_fd);
    freeaddrinfo(udp_res);
    close(udp_fd);

    return 0;
}