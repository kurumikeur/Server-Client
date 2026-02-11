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
    close();
    freeaddrinfo(servinfo);
}

void Server::start(){
    char portstr[20];
    sprintf(portstr, "%d", port);

    //Socket set-up
    if (getaddrinfo(NULL, (const char*)portstr, &hints, &servinfo) != 0)
        err(" ");
    // if ( ((sockaddr_in*)servinfo->ai_addr)->sin_port != (unsigned short int)port  )
    //     printf("[WARN] Wasn't able to get socket with set port. Other port was set.\n");
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == -1) 
        err(" ");

    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    printf("Server started with selected port: %d. Hiii! :3\n",  ((sockaddr_in*)servinfo->ai_addr)->sin_port);
    listen(sockfd, 3);
    connfd = accept(sockfd, NULL, NULL);
    while (true) {
        char bufferRecv[BUFFER_SIZE];
        char bufferSend[BUFFER_SIZE];
        ssize_t bytesRead = recv(connfd, bufferRecv, sizeof(bufferRecv) - 1, 0);
        if (bytesRead <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        } 
        if (bufferRecv[bytesRead - 1] == '\n') 
            bufferRecv[bytesRead - 1] = '\0';
        else
            bufferRecv[bytesRead] = '\0';
        printf("Received: %s\n", bufferRecv);

        printf("Enter message to send: ");
            if (fgets(bufferSend, BUFFER_SIZE, stdin) == NULL){
                printf("\nExiting.\n");
                break;
            }
            if (strlen(bufferSend) <= 0)
                continue;
            ssize_t bytesSent = send(connfd, bufferSend, strlen(bufferSend), 0);
            if (bytesSent == -1) 
                printf("Error sending message.\n");

    }
}

void Server::close(){
    connfd != -1 ? ::close(connfd) : connfd ;
    sockfd != -1 ? ::close(sockfd) : sockfd ;
}

