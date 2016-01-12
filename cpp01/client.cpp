#include <iostream>
#include <string>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifdef __linux__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#elif __APPLE__
#include <unistd.h>
#endif

#define PORT 4300
#define BUFF_SIZE 1024

// initializes the socket
int init()
{
    int sockfd;
    // TODO: socket
    //
    return sockfd;
}

void establish_conn(int sockfd, std::string hostname)
{
    struct sockaddr_in address;
    // TODO: connect
    //
}

int send_msg(std::string msg, int sockfd)
{
    char buffer[BUFF_SIZE];
    int n;
    memset(buffer, 0, BUFF_SIZE);
    memcpy(buffer, msg.c_str(), msg.length());
    // TODO: write
    //
    return n;
}

std::string recv_msg(int sockfd)
{
    char buffer[BUFF_SIZE];
    memset(buffer, 0, BUFF_SIZE);
    // TODO: read
    // 
    return std::string(buffer);
}

void cleanup(int sockfd)
{
    // TODO: close
}

int main(int argc, const char *argv[])
{
    int sockfd = init();
    establish_conn(sockfd, std::string("scm-88587.local"));
    send_msg(std::string("Hello\n"), sockfd);
    std::string s = recv_msg(sockfd);
    std::cout << "final: " << s << std::endl;
    cleanup(sockfd);
    return 0;
}
