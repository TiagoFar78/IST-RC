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
#include <iostream>
#define PORT "58011"

using namespace std;

int udp_errcode, udp_socket;
socklen_t udp_addrlen;
struct addrinfo udp_hints,*udp_res;
struct sockaddr_in udp_addr;

int tcp_errcode, tcp_socket;
struct addrinfo tcp_hints,*tcp_res;

ssize_t n;

void createUDPSocket() {
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        printf("Erro neste socket - bruh\n");
        exit(1);
    }

    memset(&udp_hints,0,sizeof udp_hints);
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;

    udp_errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", PORT, &udp_hints, &udp_res); 
    if(udp_errcode != 0) {
        printf("Erro nesta bomba - deu erro\n");
        exit(1);
    }
}

void createTCPSocket() {
    tcp_socket = socket(AF_INET, SOCK_STREAM,0);
    if (tcp_socket == -1) {
        printf("Erro neste socket - bruh\n");
        exit(1);
    }

    memset(&tcp_hints,0,sizeof tcp_hints);
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM;

    tcp_errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", PORT, &tcp_hints, &tcp_res); 
    if(tcp_errcode != 0) {
        printf("Erro nesta bomba - deu erro\n");
        exit(1);
    }

    n = connect(tcp_socket, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao connectou\n");
        exit(1);
    }
}

void sendUDP(char* message) {
    char buffer[128];

    n = sendto(udp_socket, message, strlen(message), 0, udp_res->ai_addr, udp_res->ai_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao escreveu\n");
        exit(1);
    };

    udp_addrlen = sizeof(udp_addr);
    n = recvfrom(udp_socket, buffer, 128, 0, (struct sockaddr*)&udp_addr, &udp_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao escreveu\n");
        exit(1);
    };

    write(1,"echo: ",6); 
    write(1, buffer, n);

    memset(buffer, 0, 128);

}

void sendTCP(char* message) {
    char buffer[128];
    n = write(tcp_socket, message, strlen(message));
    if (n == -1) {
        printf("Erro nesta bomba - nao escreveu\n");
        exit(1);
    }

    n = read(tcp_socket, buffer, 128);
    if (n == -1) {
        printf("Erro nesta bomba - nao leu\n");
        exit(1);
    }

    write(1, "echo: ",6);
    write(1, buffer, n);

    memset(buffer, 0, 128);
}

void translate (char* command, char* arguments, char * translated_message) {
    const char* prefix;

   if (strcmp(command, "login") == 0) {
        prefix = "LIN ";
    }

    strcpy(translated_message, prefix);
    strcat(translated_message, arguments);
    strcat(translated_message, "\n");
}


int main() {
    char write_buffer[128];
    char command[128];
    char translated_message[128];

    createUDPSocket();

    while (1) {

        fgets(write_buffer, 128, stdin);
        if (strcmp(write_buffer, "stop\n") == 0) {
            break;
        }
        
        strcpy(command, write_buffer);
        strtok(command, " ");
        char * arguments = strtok(NULL, "\n");

        translate(command, arguments, translated_message);

        if (!strcmp(command, "login")) {
            sendUDP(translated_message);

        } else if (!strcmp(command, "open")) {
            createTCPSocket();
            sendTCP(translated_message);
            freeaddrinfo(tcp_res);
            close(tcp_socket);
        }

        memset(command, 0, 128);
        memset(write_buffer, 0, 128);
        memset(translated_message, 0, 128);
    }

    freeaddrinfo(udp_res);
    close(udp_socket);
    
    return 0;
}