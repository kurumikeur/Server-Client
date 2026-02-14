//Headers, defines and functions that are used in both server and client.

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#define BUFFER_SIZE 1024

void err(const char * msg){
    printf("%s", msg);
    exit(1);
}