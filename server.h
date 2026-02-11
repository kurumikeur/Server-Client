#include "util.h"


class Server {
public:
    int port;
    Server();
    ~Server();
    void start();
    void close();
private:
    int sockfd = -1;
    int connfd = -1;
    struct addrinfo hints, *servinfo;
};