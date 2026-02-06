#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <unistd.h>
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
    int sockfd;
    struct addrinfo hints, *clientinfo;
};