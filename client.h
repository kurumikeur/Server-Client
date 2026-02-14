#include "util.h"
#define RECONN_ATTEMPTS_COUNT 100

class Client{
public:
    int port;
    const char* hostname = "localhost";
    Client();
    ~Client();
    void start();
    void close();
private:
    void setup();
    int sockfd = -1;
    int connfd = -1;
    struct addrinfo hints, *clientinfo;
};