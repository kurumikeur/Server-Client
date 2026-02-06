#include "server.h"

int main(int argc, char* argv[]) {
    Server server;
    for (int i = 1; i < argc; i++){
        char* endptr;
        errno = 0;
        server.port = strtol(argv[i], &endptr, 10);
        if (errno == ERANGE || server.port < 0){
            fprintf(stderr, "[!] Wrong port info.\n");
            exit(1);
        }
        break;   
    }
    server.start();    
}

Server::Server() {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    
}

Server::~Server() {
    freeaddrinfo(servinfo);
}

void Server::start(){
    char portstr[20];
    sprintf(portstr, "%d", port);
    printf("Dbg1 %d, %s\n", port, (const char*)portstr);
    if (getaddrinfo(NULL, (const char*)portstr, &hints, &servinfo) != 0){
        printf("err");
        exit(1);
    }

    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == -1) {
        printf("err");
        exit(1);
    }
    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

    printf("Server started with selected port: %d. Hiii! :3\n", port);
    listen(sockfd, 3);
    connfd = accept(sockfd, NULL, NULL);
    while (true) {
        recv(connfd, NULL, 0, 0);
        char buffer[1024];
        ssize_t bytesRead = recv(connfd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        } 
        if (buffer[bytesRead - 1] == '\n') 
            buffer[bytesRead - 1] = '\0';
        else
            buffer[bytesRead] = '\0';
        printf("Received: %s\n", buffer);
    }
}

void Server::close(){
    connfd != -1 ? ::close(connfd) : connfd ;
    sockfd != -1 ? ::close(sockfd) : sockfd ;
}

