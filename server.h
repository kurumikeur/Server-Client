#include "util.h"
#define MAX_CONNS 10


class Server {
public:
    int port;
    Server();
    ~Server();
    void start();
    void close();
private:
    void setup();
    void recvMessage(int connfd);
    void sendMessage(int connfd, void* data);
    int connCount = 1;
    int sockfd = -1;
    int connfd = -1;
    struct addrinfo hints, *servinfo;
    struct pollfd* pfds;
};