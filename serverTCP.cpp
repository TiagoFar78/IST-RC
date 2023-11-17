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
    printf("server started working\n");

    int fd,errcode, newfd;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res; 
    struct sockaddr_in addr;
    char buffer[128];

    fd = socket(AF_INET,SOCK_STREAM,0);
    if (fd == -1) {
        printf("Erro nesta bomba - socket mal criado\n");
        exit(1);
    }

    memset(&hints,0,sizeof hints); 
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    errcode = getaddrinfo(NULL, PORT, &hints, &res);
    if (errcode != 0) {
        printf("Erro nesta bomba - nao deu bind\n");
        exit(1);
    }

    n = bind(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        printf("Erro nesta bomba - nao deu bind\n");
        exit(1);
    }

    if(listen(fd,5) == -1) {
        printf("Erro nesta bomba - nao ouviu ig\n");
        exit(1);
    }

    while (1) { 
        addrlen = sizeof(addr);
        if ((newfd = accept(fd, (struct sockaddr*)&addr, &addrlen)) == -1) {
            printf("Erro nesta bomba - nao aceitou\n");
            exit(1);
        }

        while (strcmp(buffer, "stop\n") != 0) {
            memset(buffer, 0, 128);

            n = read(newfd, buffer, 128);
            if (n == -1) {
                printf("Erro nesta bomba - nao leu\n");
                exit(1);
            }

            write(1, "received: ", 10);
            write(1, buffer, n);

            n = write(newfd, buffer, n);
            if (n == -1) {
                printf("Erro nesta bomba - nao escreveu\n");
                exit(1);
            }
        }
        memset(buffer, 0, 128);
        
        close(newfd);
    }

    freeaddrinfo(res);
    close(fd);

    return 0;
}