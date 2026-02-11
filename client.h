#include "util.h"

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
    struct addrinfo hints, *clientinfo;
};