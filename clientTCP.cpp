#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define PORT "58069"

int main() {
    printf("client started working\n");

    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res; 
    struct sockaddr_in addr;
    char buffer[128];
    char write_buffer[128];

    fd = socket(AF_INET, SOCK_STREAM,0);
    if (fd == -1) {
        printf("Erro nesta bomba - bruh\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints); 
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo("127.0.0.1", PORT, &hints, &res);
    if (errcode != 0) {
        printf("Erro nesta bomba - deu erro\n");
        exit(1);
    }

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao connectou\n");
        exit(1);
    }

    while (1) {
        fgets(write_buffer, 128, stdin);

        n = write(fd, write_buffer, strlen(write_buffer));
        if (n == -1) {
            printf("Erro nesta bomba - nao escreveu\n");
            exit(1);
        }

        n = read(fd, buffer, 128);
        if (n == -1) {
            printf("Erro nesta bomba - nao leu\n");
            exit(1);
        }

        write(1, "echo: ",6);
        write(1, buffer, n);

        if (strcmp(buffer, "stop\n") == 0) {
            break;
        }

        memset(write_buffer, 0, 128);
        memset(buffer, 0, 128);
    }
    
    freeaddrinfo(res);
    close(fd);

    return 0;
}