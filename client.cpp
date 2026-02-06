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
    char portstr[20];
    sprintf(portstr, "%d", port);

    if (getaddrinfo(hostname, (const char*)portstr, &hints, &clientinfo) != 0){
        printf("err");
        exit(1);
    }

    sockfd = socket(clientinfo->ai_family, clientinfo->ai_socktype, clientinfo->ai_protocol);
    if (sockfd == -1) {
        printf("err");
        exit(1);
    }

    printf("TCP client test. Hiii!! :3 :3 :3\n");
    connect(sockfd, clientinfo->ai_addr, clientinfo->ai_addrlen);
    char buffer[BUFFER_SIZE];
    while (true) {
        printf("Enter message to send: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL){
            printf("\nExiting.\n");
            break;
        }
        if (strlen(buffer) <= 0)
            continue;
        ssize_t bytesSent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytesSent == -1) {
            printf("Error sending message.\n");
        }
    }
}

void Client::close() {
    sockfd != -1 ? ::close(sockfd) : sockfd ;
}
