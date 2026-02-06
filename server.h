#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "util.h"


class Server {
public:
    int port;
    Server();
    ~Server();
    void start();
    void close();
private:
    int sockfd;
    int connfd;
    struct addrinfo hints, *servinfo;
};