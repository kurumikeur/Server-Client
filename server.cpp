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
    
    pfds = (pollfd*) malloc(sizeof(pollfd) * MAX_CONNS);
}

Server::~Server() {
    close();
    freeaddrinfo(servinfo);
    free(pfds);
}

void Server::start(){
    setup();
    listen(sockfd, 3);
    while (true) {
        int poll_count = poll(pfds, connCount, -1);
        for (int i = 0; i < connCount; i++){
            if (pfds[i].revents & (POLLIN | POLLHUP)){
                if (i == 0){
                    struct sockaddr_storage remoteaddr;
                    socklen_t addrlen = sizeof(remoteaddr);
                    if(connfd = accept(sockfd, (struct sockaddr *)&remoteaddr, &addrlen) != -1){                        
                        pfds[connCount].fd = connfd;
                        pfds[connCount].events = POLLIN;
                        ++connCount;
                        continue;
                    }
                       
                }
                else
                    recvMessage(pfds[i].fd);                    
            }
        }
    }
}

void Server::recvMessage(int connfd){
    char bufferRecv[BUFFER_SIZE];
    ssize_t bytesRead = recv(connfd, bufferRecv, sizeof(bufferRecv) - 1, 0);
    if (bytesRead <= 0) {
        printf("Client disconnected or error occurred.\n");
        return;  
    }  
    if (bufferRecv[bytesRead - 1] == '\n') 
        bufferRecv[bytesRead - 1] = '\0';
    bufferRecv[bytesRead] = '\0';
    printf("Received: %s\n", bufferRecv);
}

void Server::sendMessage(int connfd, void *data){
    char bufferSend[BUFFER_SIZE];
    printf("Enter message to send: ");
    if (fgets(bufferSend, BUFFER_SIZE, stdin) == NULL){
        printf("\nExiting.\n");
        return;
    }
    else {
        if (strlen(bufferSend) <= 0)
            return;
        ssize_t bytesSent = send(connfd, bufferSend, strlen(bufferSend), 0);
        if (bytesSent == -1) 
            printf("Error sending message.\n");
    }
}

void Server::close(){
    connfd != -1 ? ::close(connfd) : connfd ;
    sockfd != -1 ? ::close(sockfd) : sockfd ;
}

void Server::setup(){
    char portstr[20];
    sprintf(portstr, "%d", port);
    if (getaddrinfo(NULL, (const char*)portstr, &hints, &servinfo) != 0)
        err(" ");
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == -1) 
        err(" ");
    bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    pfds[0].fd = sockfd;
    pfds[0].events = POLLIN;
    printf("Server started with selected port: %d. Hiii! :3\n",  ntohs(((sockaddr_in*)servinfo->ai_addr)->sin_port));
}

