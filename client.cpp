#include "client.h"


int main(int argc, char* argv[]) {
    Client client;
    for (int i = 1; i < argc; i++){
        char* endptr;
        errno = 0;
        client.port = strtol(argv[i], &endptr, 10);
        if (errno == ERANGE || client.port < 0){
            fprintf(stderr, "[!] Wrong port info.\n");
            exit(1);
        }
        break;   
    }
    client.start();
}

Client::Client() {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;  
}

Client::~Client() {
    freeaddrinfo(clientinfo);
}

void Client::start() { 
    setup();

    printf("Starting client. Hiii!! :3 :3 :3\n");
    //while(true){
        for (int i = 0; connect(sockfd, clientinfo->ai_addr, clientinfo->ai_addrlen) == -1; i++){
            if (i == 3)
                err("Couldn't connect to server. Exiting.\n");
            sleep(2);
        }
        char IPaddr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(((sockaddr_in*)clientinfo->ai_addr)->sin_addr), IPaddr, INET_ADDRSTRLEN);
        printf("Succesfully connected to server at: %s:%d\n", IPaddr, ntohs(((sockaddr_in*)clientinfo->ai_addr)->sin_port));
        char bufferSend[BUFFER_SIZE];
        char bufferRecv[BUFFER_SIZE];
        while (true) {
            printf("Enter message to send: ");
            if (fgets(bufferSend, BUFFER_SIZE, stdin) == NULL){
                printf("\nExiting.\n");
                break;
            }
            if (strlen(bufferSend) <= 0)
                continue;
            ssize_t bytesSent = send(sockfd, bufferSend, strlen(bufferSend), 0);
            if (bytesSent == -1) 
                printf("Error sending message.\n");

            ssize_t bytesRead = recv(sockfd, bufferRecv, sizeof(bufferRecv) - 1, 0);
            if (bytesRead <= 0) {
                printf("Client disconnected or error occurred.\n");
                break;
            } 
            if (bufferRecv[bytesRead - 1] == '\n') 
                bufferRecv[bytesRead - 1] = '\0';
            else
                bufferRecv[bytesRead] = '\0';
            printf("Received: %s\n", bufferRecv);    
        }
   // }
}

void Client::close() {
    sockfd != -1 ? ::close(sockfd) : sockfd ;  
}

void Client::setup(){
    char portstr[20];
    sprintf(portstr, "%d", port);

    if (getaddrinfo(hostname, (const char*)portstr, &hints, &clientinfo) != 0)
        err("1");

    sockfd = socket(clientinfo->ai_family, clientinfo->ai_socktype, clientinfo->ai_protocol);
    if (sockfd == -1)
        err("1");

}
