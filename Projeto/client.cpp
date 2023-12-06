#include <unistd.h>
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
#include <fstream>
#include "AuctionManager.h"

#define PORT "58011"

using namespace std;

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

string translateInput(string command, string arguments) {
    string translated_message;
    string prefix;

    if (command == "login") {
        prefix = "LIN ";

    } else if (command == "logout") {
        prefix = "LOU ";
        arguments += uid + " " + password;

    } else if (command == "unregister") {
        prefix = "UNR ";
        arguments += uid + " " + password;

    } else if (command == "open") {
        prefix = "OPA ";
        vector<string> old_arguments = splitString(arguments, ' ');

        string fileContents;
        
        //TODO verificar se correu bem a ambas as funcoes
        read_from_file(old_arguments[1], fileContents);
        string fileSize = to_string(getFileSize(old_arguments[1]));

        arguments = uid + " " + password + " " + old_arguments[0] + " " + old_arguments[2] + " " 
                    + old_arguments[3] + " " + old_arguments[1] + " " + fileSize + " " + fileContents;

        cout << "a mandar: " << fileContents << "\n";
        
    } else if (command == "close") {
        prefix = "CLS ";
        arguments = uid + " " + password + " " + arguments;
        
    } else if ((command == "myauctions") || (command == "ma")) {
        prefix = "LMA ";
        arguments = uid;

    } else if ((command == "mybids") || (command == "mb")) {
        prefix = "LMB ";
        arguments = uid;

    } else if ((command == "list") || (command == "l")) {
        prefix = "LST";

    }  else if ((command == "show_asset") || (command == "sa")) {
        prefix = "SAS ";

    } else if ((command == "bid") || (command == "b")) {
        prefix = "BID ";
        arguments = uid + " " + password + " " + arguments;

    }  else if ((command == "show_record") || (command == "sr")) {
        prefix = "SRC ";
    }

    translated_message = prefix + arguments + "\n"; 
    return translated_message;
}

string translateOutput(string message) {
    vector<string> output = splitString(message, ' ');

    string command = output[0];
    string status = output[1];

    if (command == "RLI") {
        if (status == "OK\n") {
            logged_in = true;
            password = temp_pass;
            return "successful login\n";

        } else if (status == "NOK\n") {
            return "incorrect login attempt\n";

        } else if (status == "REG\n") {
            logged_in = true;
            password = temp_pass;
            return "new user registered\n";
        }

    } else if (command == "RLO") {
        if (status == "OK\n") {
            logged_in = false;
            return "successful logout\n";

        } else if (status == "NOK\n") {
            return "user not logged in\n";

        } else if (status == "UNR\n") {
            return "unknown user\n";
        }
    } else if (command == "RUR") {
        if (status == "OK\n") {
            logged_in = false;
            return "successful unregister\n";

        } else if (status == "NOK\n") {
            return "incorrect unregister attempt\n";

        } else if (status == "UNR\n") {
            return "unknown user\n";
        }

    } else if (command == "ROA") {
        if (status == "OK") {
            return "successful " + output[2];

        } else if (status == "NOK\n") {
            return "auction could not be started\n";

        } else if (status == "NLG\n") {
            return "user not logged in\n";
        }

    } else if (command == "RCL") {
        if (status == "OK\n") {
            return "successful " + output[2];

        } else if (status == "NLG\n") {
            return "user not logged in\n";

        } else if (status == "EAU\n") {
            return "auction does not exist\n";

        } else if (status == "EOW\n") {
            return "auction is not owned by user " + uid + "\n";

        } else if (status == "END\n") {
            return "auction has already finished\n";
        }

    } else if (command == "RMA") {
        if (status == "OK") {
            string message;
            for(int i = 2; i < output.size() - 1; i = i + 2) {
                message += output[i] + " " + output[i + 1];
                if(i != output.size() - 2) {
                    message += "\n";
                }
            }
            return message;
        
        } else if (status == "NOK\n") {
            return "no ongoing auctions\n";

        } else if (status == "NLG\n") {
            return "user not logged in\n";
        } 
    
    } else if (command == "RMB") {
        if (status == "OK") {
            string message;
            for(int i = 2; i < output.size() - 1; i = i + 2) {
                message += output[i] + " " + output[i + 1];
                if(i != output.size() - 2) {
                    message += "\n";
                }
            }
            return message;
        
        } else if (status == "NOK\n") {
            return "no ongoing bids\n";

        } else if (status == "NLG\n") {
            return "user not logged in\n";
        }

    } else if (command == "RLS") {
        if (status == "OK") {
            string message;
            for(int i = 2; i < output.size() - 1; i = i + 2) {
                message += output[i] + " " + output[i + 1];
                if(i != output.size() - 2) {
                    message += "\n";
                }
            }
            return message;
        
        } else if (status == "NOK\n") {
            return "no auction was yet started\n";
        }
        
    } else if (command == "RBD") {
        if (status == "OK") {
            string message;
            for(int i = 2; i < output.size() - 1; i = i + 2) {
                message += output[i] + " " + output[i + 1];
                if(i != output.size() - 2) {
                    message += "\n";
                }
            }
            return message;
        
        } else if (status == "NOK\n") {
            return "auction is not active\n";

        } else if (status == "NLG\n") {
            return "user not logged in\n";

        } else if (status == "ACC\n") {
            return "bid was accepted\n";

        } else if (status == "REF\n") {
            return "bid was refused because a larger bid has already been placed previously\n";
            
        } else if (status == "ILG\n") {
            return "tried to make a bid in an auction hosted by yourself\n";
        }

    } else if (command == "RRC") {
        if (status == "OK") {
            //TODO pq é q n vem com \n
            return message;
        
        } else if (status == "NOK\n") {
            return "auction does not exist\n";
        }
        
    } else if (command == "RSA") {
        if (status == "OK") {
            string content;
            for(int i = 4; i < output.size(); i++) {
                content += output[i];
                if(i != output.size() - 1) {
                    content += " ";
                }
            }
            create_file(output[2]);
            write_on_file(output[2], content, true);
            return output[2] + " " + output[3] + "\n";
        
        } else if (status == "NOK\n") {
            return "there is no file to be sent, or some other problem\n";
        }
    }

    return message;
}

void createUDPSocket() {
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        printf("Erro neste socket - bruh\n");
        exit(1);
    }

    memset(&udp_hints, 0, sizeof udp_hints);
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;

    udp_errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", PORT, &udp_hints, &udp_res);
    if (udp_errcode != 0) {
        printf("Erro nesta bomba - deu erro\n");
        exit(1);
    }
}

void createTCPSocket() {
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1) {
        printf("Erro neste socket - bruh\n");
        exit(1);
    }

    memset(&tcp_hints, 0, sizeof tcp_hints);
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM;

    tcp_errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", PORT, &tcp_hints, &tcp_res);
    if (tcp_errcode != 0) {
        printf("Erro nesta bomba - deu erro\n");
        exit(1);
    }

    n = connect(tcp_socket, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao connectou\n");
        exit(1);
    }
}

void sendUDP(string message) {
    char buffer[10000];
    memset(buffer, 0, 10000);
    string translated_message;

    n = sendto(udp_socket, message.c_str(), message.length(), 0, udp_res->ai_addr, udp_res->ai_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao escreveu\n");
        exit(1);
    }

    udp_addrlen = sizeof(udp_addr);

    //como estou a endereçar mudar char buffer
    n = recvfrom(udp_socket, buffer , 10000, 0, (struct sockaddr*)&udp_addr, &udp_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao escreveu\n");
        exit(1);
    }

    string substring_buffer(buffer, buffer + n);
    translated_message = translateOutput(substring_buffer);
    cout << translated_message;
}

void sendTCP(string message) {
    char buffer[10000];
    memset(buffer, 0, 10000);
    string translated_message;

    n = write(tcp_socket, message.c_str(), message.length());
    if (n == -1) {
        printf("Erro nesta bomba - nao escreveu\n");
        exit(1);
    }

    n = read(tcp_socket, buffer, 10000);
    cout << "buffer: " << buffer << "\n";
    if (n == -1) {
        printf("Erro nesta bomba - nao leu\n");
        exit(1);
    }

    string substring_buffer(buffer, buffer + n);
    translated_message = translateOutput(substring_buffer);
    cout << translated_message;
}


int main() {
    string write_buffer;
    string command;
    string translated_message;
    string arguments;

    createUDPSocket();

    while (1) {
        arguments.clear();
        command.clear();
        write_buffer.clear();
        translated_message.clear();

        getline(cin, write_buffer);

        vector<string> input = splitString(write_buffer, ' ');
        command = input[0];

        for (size_t i = 1; i < input.size(); i++) {
            arguments += input[i];

            if (i != (input.size() - 1))
                arguments += " ";
        }

        translated_message = translateInput(command, arguments);
       

        if (command == "login") {
            if (logged_in) {
                cout << "first execute the logout command\n";

            } else {
                uid = input[1];
                temp_pass = input[2];
                sendUDP(translated_message);
            }
    
        } else if ((command == "logout") || (command == "unregister") || (command == "myauctions") || (command == "ma") 
                    || (command == "mybids") || (command == "mb")) { 
            if(!logged_in) {
               cout << "user not logged in\n";
               
            } else {
                sendUDP(translated_message);
            }
        
        } else if ((command == "list") || (command == "l") || (command == "show_record") || (command == "sr")) {
            sendUDP(translated_message);

        } else if ((command == "exit")) {
            if (logged_in) {
                cout << "first execute the logout command\n";

            } else {
                break;
            }

        } else if (command == "open" || command == "close" || command == "bid" || command == "b" ) {
            if(!logged_in) {
               cout << "user not logged in\n";
               
            } else {
                createTCPSocket();
                sendTCP(translated_message);
                freeaddrinfo(tcp_res);
                close(tcp_socket);
            }

        } else if(command == "show_asset" || command == "sa") {
            createTCPSocket();
            sendTCP(translated_message);
            freeaddrinfo(tcp_res);
            close(tcp_socket);
        }
    
    }

    freeaddrinfo(udp_res);
    close(udp_socket);

    return 0;
}
