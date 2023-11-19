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
#define UDP_PORT "58028"
#define TCP_PORT "58029"

using namespace std;

int udp_errcode, udp_socket;
socklen_t udp_addrlen;
struct addrinfo udp_hints,*udp_res;
struct sockaddr_in udp_addr;

int tcp_errcode, tcp_socket;
struct addrinfo tcp_hints,*tcp_res;

char buffer[128];
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

    udp_errcode = getaddrinfo("127.0.0.1", UDP_PORT, &udp_hints, &udp_res); 
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

    tcp_errcode = getaddrinfo("127.0.0.1", TCP_PORT, &tcp_hints, &tcp_res); 
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
    write(1,buffer,n);

    memset(buffer, 0, 128);

}

void sendTCP(char* message) {
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


int main() {
    char write_buffer[128];

    createUDPSocket();
    createTCPSocket();
    printf("cheguei1\n");
    while (1) {
        fgets(write_buffer, 128, stdin);
        printf("cheguei2\n");
        if (strcmp(write_buffer, "stop\n") == 0) {
            break;
        }

        string command = strtok(write_buffer, " ");
        if (command == "login\n") {
            printf("cheguei3\n");
            sendUDP(write_buffer);
        } else if (command == "open\n" || command == "ola\n" ) {
            printf("cheguei4\n");
            sendTCP(write_buffer);
        }

        memset(write_buffer, 0, 128);
    }

    freeaddrinfo(udp_res);
    freeaddrinfo(tcp_res);
    close(udp_socket);
    close(tcp_socket);

    return 0;
}