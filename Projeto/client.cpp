#include <unistd.h>
#include <csignal>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>
#include "ArgumentVerification.h"

using namespace std;

string PORT = "58028";
string ADDRESS = "127.0.0.1";

int udp_errcode, udp_socket;
socklen_t udp_addrlen;
struct addrinfo udp_hints,*udp_res;
struct sockaddr_in udp_addr;

int tcp_errcode, tcp_socket;
struct addrinfo tcp_hints,*tcp_res;

ssize_t n;
string password;
string temp_pass;
string uid;
bool logged_in = false;

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

int read_from_file(const string& file_name, string& buffer) {
    ifstream file(file_name, ios::binary);
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

int getLastIndexOfSlash(const string& s) {
	for(int i = s.length() - 1; i >= 0; i--) {
		if (s[i] == '/') {
			return i;
        }
	}

	return -1;
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

size_t getFileSize(const string& file_name) {
    struct stat file_stat;

    if (stat(file_name.c_str(), &file_stat) == 0) {
        return static_cast<size_t>(file_stat.st_size);
    } else {
        return 0;
    }
}

vector<string> splitString(string input, char delimiter) {
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

// #-------------------------------------------------------------------#
// |                Input and Output translation                       |
// #-------------------------------------------------------------------#

string translateInput(string command, vector<string> input) {

    input.erase(input.begin());

    string translated_message, prefix, arguments;

    if (command == "login") {
        if (is_unexpected_login_input(input, true)) {
            return "Invalid login format\n";
        }
        prefix = "LIN ";
        arguments = input[0] + " " + input[1];

    } else if (command == "logout") {
        if (is_unexpected_logout_input(input, true)) {
            return "Invalid logout format\n";
        }
        prefix = "LOU ";
        arguments = uid + " " + password;

    } else if (command == "unregister") {
        if (is_unexpected_unregister_input(input, true)) {
            return "Invalid unregister format\n";
        }
        prefix = "UNR ";
        arguments = uid + " " + password;

    } else if (command == "open") {
        if (is_unexpected_open_input(input, true)) {
            return "Invalid open format\n";
        }
        prefix = "OPA ";
        string fileContents;

        int read_validation = read_from_file(input[1], fileContents);
        if(read_validation == -1) {
            return "Error: A file error occured. Try again\n";
        }
        
        string fileSize = to_string(getFileSize(input[1]));

        arguments = uid + " " + password + " " + input[0] + " " + input[2] + " "
                    + input[3] + " " + input[1] + " " + fileSize + " " + fileContents;

    } else if (command == "close") {
        if (is_unexpected_close_input(input, true)) {
            return "Invalid close format\n";
        }
        prefix = "CLS ";
        arguments = uid + " " + password + " " + input[0];

    } else if ((command == "myauctions") || (command == "ma")) {
        if (is_unexpected_list_auctions_target_input(input, true)) {
            return "Invalid myauctions format\n";
        }
        prefix = "LMA ";
        arguments = uid;

    } else if ((command == "mybids") || (command == "mb")) {
        if (is_unexpected_list_bids_target_input(input, true)) {
            return "Invalid mybids format\n";
        }
        prefix = "LMB ";
        arguments = uid;

    } else if ((command == "list") || (command == "l")) {
        if (is_unexpected_list_auctions_input(input, true)) {
            return "Invalid list format\n";
        }
        prefix = "LST";

    }  else if ((command == "show_asset") || (command == "sa")) {
        if (is_unexpected_show_asset_input(input, true)) {
            return "Invalid show_asset format\n";
        }
        prefix = "SAS ";
        arguments = input[0];

    } else if ((command == "bid") || (command == "b")) {
        if (is_unexpected_bid_input(input, true)) {
            return "Invalid bid format\n";
        }
        prefix = "BID ";
        arguments = uid + " " + password + " " + input[0] + " " + input[1];

    }  else if ((command == "show_record") || (command == "sr")) {
        if (is_unexpected_show_record_input(input, true)) {
            return "Invalid show_record format\n";
        }
        prefix = "SRC ";
        arguments = input[0];
    }

    translated_message = prefix + arguments + "\n";
    return translated_message;
}

string translateOutput(string message) {
    vector<string> output = splitString(message, ' ');

    if (message == "ERR\n") {
        return "Invalid message command\n";
    }

    if(output.size() < 2) {
        return "Received invalid server reply\n";
    }

    string command = output[0];
    string status = output[1];
    output.erase(output.begin());
    output.erase(output.begin());

    if (command == "RLI") {
        if(is_unexpected_login_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK\n") {
            logged_in = true;
            password = temp_pass;
            return "Login successful\n";

        } else if (status == "NOK\n") {
            return "Incorrect login attempt\n";

        } else if (status == "REG\n") {
            logged_in = true;
            password = temp_pass;
            return "New user registered - Login successful\n";
        
        } else if (status == "ERR\n") {
            return "Invalid login format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RLO") {
        if(is_unexpected_logout_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK\n") {
            logged_in = false;
            return "Logout user " + uid + " successful\n";

        } else if (status == "NOK\n") {
            return "User not logged in\n";

        } else if (status == "UNR\n") {
            return "Unknown user\n";

        } else if (status == "ERR\n") {
            return "Invalid logout format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RUR") {
        if(is_unexpected_unregister_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK\n") {
            logged_in = false;
            return "Unregister (and logout) user " + uid + " successful\n";

        } else if (status == "NOK\n") {
            return "User not logged in\n";

        } else if (status == "UNR\n") {
            return "Unknown user\n";

        } else if (status == "ERR\n") {
            return "Invalid unregister format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "ROA") {
        if(is_unexpected_open_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK") {
            return "Open successful. Auction ID = " + output[0]; //done

        } else if (status == "NOK\n") {
            return "Auction could not be started\n";

        } else if (status == "NLG\n") {
            return "User not logged in\n";

        } else if (status == "ERR\n") {
            return "Invalid open format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RCL") {
        if(is_unexpected_close_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK\n") {
            return "Successfully closed\n";

        } else if (status == "NLG\n") {
            return "User not logged in\n";

        } else if (status == "EAU\n") {
            return "Auction does not exist\n";

        } else if (status == "EOW\n") {
            return "Auction is not owned by user " + uid + "\n";

        } else if (status == "END\n") {
            return "Auction has already finished\n";

        } else if (status == "ERR\n") {
            return "Invalid close format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RMA") {
        if(is_unexpected_myauctions_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK") {
            string message, status;

            for(int i = 0; i < output.size() - 1; i = i + 2) {
                status = "Active\n";
                if(output[i + 1] == "0" || output[i + 1] == "0\n") {
                    status = "Inactive\n";
                }
                message += output[i] + " - " + status;
            }
            return message;

        } else if (status == "NOK\n") {
            return "No ongoing auctions\n";

        } else if (status == "NLG\n") {
            return "User not logged in\n";

        } else if (status == "ERR\n") {
            return "Invalid myauctions format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RMB") {
        if(is_unexpected_mybids_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK") {
            string message, status;
            for(int i = 0; i < output.size() - 1; i = i + 2) {
                status = "Active";
                 if(output[i + 1] == "0" || output[i + 1] == "0\n") {
                    status = "Inactive";
                }
                message += output[i] + " - " + status + "\n";
            }
            return message;

        } else if (status == "NOK\n") {
            return "No ongoing bids\n";

        } else if (status == "NLG\n") {
            return "User not logged in\n";

        } else if (status == "ERR\n") {
            return "Invalid mybids format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RLS") {
        if (status == "OK") {
            if(is_unexpected_list_output(output, status))
                return "Received invalid server reply\n";

            string message, status;
            for(int i = 0; i < output.size() - 1; i = i + 2) {
                status = "Active";
                if(output[i + 1] == "0" || output[i + 1] == "0\n") {
                    status = "Inactive";
                }
                message += output[i] + " - " + status + "\n";
            }
            return message;

        } else if (status == "NOK\n") {
            return "No auction was yet started\n";

        } else if (status == "ERR\n") {
            return "Invalid list format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RBD") {
        if(is_unexpected_bid_output(output, status))
            return "Received invalid server reply\n";

        if (status == "NOK\n") {
            return "Auction is not active\n";

        } else if (status == "NLG\n") {
            return "User not logged in\n";

        } else if (status == "ACC\n") {
            return "Bid was accepted\n";

        } else if (status == "REF\n") {
            return "Bid was refused because a larger bid has already been placed previously\n";

        } else if (status == "ILG\n") {
            return "Auction host not allowed to bid\n";

        } else if (status == "ERR\n") {
            return "Invalid bid format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RRC") {
        if(is_unexpected_show_record_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK") {
            string print;
            int increase = 0;

            output[output.size() - 1].pop_back();

            print = "Auction name: " + output[1] + ", hosted by " + output[0]
                    + "\nAsset file name: " + output[2] +"\nStarted: " + output[4]
                    + ", " + output[5] + " with start value: " + output[3] + "\nTo stay open for "
                    + output[6] + " seconds\n";

            for(int i = 7; i < output.size(); i = i + increase) {
                if (output[i] == "E") {
                    print += "\nEnded at " + output[i + 1] + ", " + output[i + 2] + " (" + output[i + 3]
                    + " after the start of the bid)\n";
                    increase = 4;
                } else if (output[i] == "B") {
                    print += "\nBid by user " + output[i + 1] + " with value " + output[i + 2]
                    + " was placed at " + output[i +3] + ", " + output[i + 4] + " (" + output[i + 5]
                    + " after the start of the bid)\n";

                    increase = 6;
                }
            }

            return print;

        } else if (status == "NOK\n") {
            return "Auction does not exist\n";

        } else if (status == "ERR\n") {
            return "Invalid show_record format\n";

        } else {
            return "Received invalid server reply\n";
        }

    } else if (command == "RSA") {
        if(is_unexpected_show_asset_output(output, status))
            return "Received invalid server reply\n";

        if (status == "OK") {
            string content;
            for(int i = 2; i < output.size(); i++) {
                content += output[i];
                if(i != output.size() - 1) {
                    content += " ";
                }
            }

            if (!content.empty() && content.back() == '\n') {
                content.pop_back();

            } else {
                return "Received invalid server reply\n";
            }

            int file_validation = create_file(output[0]);
            if(file_validation == -1) {
                return "Error: A file error occured. Try again\n";
            }

            int write_validation = write_on_file(output[0], content, true);
            if(write_validation == -1) {
                return "Error: A file error occured. Try again\n";
            }

            size_t filesize = stoi(output[1]);
            if(getFileSize(output[0]) != filesize) {
                return "Received invalid server reply\n";
            }

            return "Asset file " + output[0] + " is stored in the current directory.\n"; //done

        } else if (status == "NOK\n") {
            return "There is no file to be sent, or some other problem\n";

        } else if (status == "ERR\n") {
            return "Invalid show_asset format\n";

        } else {
            return "Received invalid server reply\n";
        }

    }

    return "Received invalid server reply: " +  message + "\n";
}

// #-------------------------------------------------------------------#
// |                UDP and TCP methods                                |
// #-------------------------------------------------------------------#

int createUDPSocket() {
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        cout << "Error in creating a socket. Try again\n";
        return -1;
    }

    memset(&udp_hints, 0, sizeof udp_hints);
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;

    udp_errcode = getaddrinfo(ADDRESS.c_str(), PORT.c_str(), &udp_hints, &udp_res);
    if (udp_errcode != 0) {
        cout << "Error in creating a socket. Try again\n";
        close(udp_socket);
        return -1;
    }

    int socket_timeout = setSocketTimeout(udp_socket, 5);
    if(socket_timeout == -1) {
        cout << "Error in creating a socket. Try again\n";
        freeaddrinfo(udp_res);
        close(udp_socket);
        return -1;
    }

    return 0;
}

int createTCPSocket() {
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1) {
        cout << "Error in creating a socket. Try again\n";
        return -1;
    }

    memset(&tcp_hints, 0, sizeof tcp_hints);
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM;

    tcp_errcode = getaddrinfo(ADDRESS.c_str(), PORT.c_str(), &tcp_hints, &tcp_res);
    if (tcp_errcode != 0) {
        cout << "Error in creating a socket. Try again\n";
        close(tcp_socket);
        return -1;
    }

    int socket_timeout = setSocketTimeout(tcp_socket, 5);
    if(socket_timeout == -1) {
        cout << "Error in creating a socket. Try again\n";
        freeaddrinfo(tcp_res);
        close(udp_socket);
        return -1;
    }

    n = connect(tcp_socket, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (n == -1) {
        cout << "Error in creating a socket. Try again\n";
        freeaddrinfo(tcp_res);
        close(tcp_socket);
        return -1;
    }

    return 0;
}

void sendUDP(string message) {
    char buffer[6000];
    memset(buffer, 0, 6000);
    string translated_message;

    n = sendto(udp_socket, message.c_str(), message.length(), 0, udp_res->ai_addr, udp_res->ai_addrlen);
    if (n == -1) {
        cout << "Error in sending message. Try again\n";
        return;
    }

    udp_addrlen = sizeof(udp_addr);

    //como estou a endereçar mudar char buffer
    n = recvfrom(udp_socket, buffer , 6000, 0, (struct sockaddr*)&udp_addr, &udp_addrlen);
    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            cout << "Timeout in receiving udp message" << "\n";
            return;
        } else {
            cout << "Error in receiving message. Try again\n";
            return;
        }

    }

    string substring_buffer(buffer, buffer + n);
    translated_message = translateOutput(substring_buffer);
    cout << translated_message;
}

void sendTCP(string message) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    string translated_message, full_output;

    size_t totalBytesSent = 0;

    while (totalBytesSent < message.length()) {
        ssize_t n = write(tcp_socket, message.c_str() + totalBytesSent, message.length() - totalBytesSent);
        if (n == -1) {
            cout << "Error in sending message. Try again\n";
            return;
        }

        totalBytesSent += n;
    }

    n = read(tcp_socket, buffer, 1024);
    if (n == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            cout << "Timeout in receiving tcp message" << "\n";
            return;
        } else {
            cout << "Error in receiving message. Try again\n";
            return;
        }
    }

    string substring_buffer(buffer, buffer + n);
    full_output += substring_buffer;
    memset(buffer, 0, 1024);
    while(n != 0) {
        n = read(tcp_socket, buffer, 1024);
        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                cout << "Timeout in receiving tcp message" << "\n";
                return;
            } else {
                cout << "Error in receiving message. Try again\n";
                return;
            }
        }
        string substring_buffer(buffer, buffer + n);
        full_output += substring_buffer;
        memset(buffer, 0, 1024);
    }

    translated_message = translateOutput(full_output);
    cout << translated_message;
}


// >-------------------------{ Client }-------------------------< 

int setup_client_settings(int argc, char *argv[]) {
    if (argc == 2 || argc == 4) {
        return -1;
    }

    if (argc >= 3) {
        if (!strcmp(argv[1], "-n")) {
            ADDRESS = argv[2];
        }
        else if (!strcmp(argv[1], "-p")) {
            PORT = argv[2];
        }
        else {
            return -1;
        }
    }

    if (argc == 5) {
        if (!strcmp(argv[3], "-n")) {
            ADDRESS = argv[4];
        }
        else if (!strcmp(argv[3], "-p")) {
            PORT = argv[4];
        }
        else {
            return -1;
        }
    }

    return 0;
}

void signal_handler(int signumber) {
    if (logged_in) {
        cout << "First execute the logout command\n";
    } else {
        freeaddrinfo(udp_res);
        close(udp_socket);
        freeaddrinfo(tcp_res);
        close(tcp_socket);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (setup_client_settings(argc, argv)) {
        cout << "Invalid arguments for starting client\n";
        return 0;
    }

    signal(SIGINT, signal_handler);

    string write_buffer;
    string command;
    string translated_message;
    string arguments;

    while (1) {
        arguments.clear();
        command.clear();
        write_buffer.clear();
        translated_message.clear();

        getline(cin, write_buffer);

        vector<string> input = splitString(write_buffer, ' ');
        command = input[0];

        translated_message = translateInput(command, input);
        if (translated_message.substr(0, 3) == "Inv" || translated_message.substr(0, 3) == "Err" ) {
            cout << translated_message << "\n";;

        } else {
            if (command == "login") {
                if (logged_in) {
                    cout << "First execute the logout command\n";

                } else {
                    int create_udp = createUDPSocket();
                    if (create_udp == -1) {
                        break;
                    }
                    uid = input[1];
                    temp_pass = input[2];
                    sendUDP(translated_message);
                    freeaddrinfo(udp_res);
                    close(udp_socket);
                }

            } else if ((command == "logout") || (command == "unregister") || (command == "myauctions") || (command == "ma")
                        || (command == "mybids") || (command == "mb")) {
                if(!logged_in) {
                cout << "User not logged in\n";

                } else {
                    int create_udp = createUDPSocket();
                    if (create_udp == -1) {
                        break;
                    }
                    sendUDP(translated_message);
                    freeaddrinfo(udp_res);
                    close(udp_socket);
                }

            } else if ((command == "list") || (command == "l") || (command == "show_record") || (command == "sr")) {
                int create_udp = createUDPSocket();
                if (create_udp == -1) {
                    break;
                }
                sendUDP(translated_message);
                freeaddrinfo(udp_res);
                close(udp_socket);

            } else if ((command == "exit")) {
                if (logged_in) {
                    cout << "First execute the logout command\n";

                } else {
                    break;
                }

            } else if (command == "open" || command == "close" || command == "bid" || command == "b" ) {
                if(!logged_in) {
                cout << "User not logged in\n";

                } else {
                    int create_udp = createUDPSocket();
                    if (create_udp == -1) {
                        break;
                    }
                    sendTCP(translated_message);
                    freeaddrinfo(tcp_res);
                    close(tcp_socket);
                }

            } else if(command == "show_asset" || command == "sa") {
                int create_udp = createUDPSocket();
                if (create_udp == -1) {
                    break;
                }
                sendTCP(translated_message);
                freeaddrinfo(tcp_res);
                close(tcp_socket);

            } else {
                cout << "Invalid message format\n";
            }
        }
    }

    return 0;
}
