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
#include <unistd.h>
#elif __APPLE__
#include <unistd.h>
#endif

#define PORT "4300"
#define BUFF_SIZE 1024

//http://www.linuxhowtos.org/C_C++/socket.htm

// initializes the socket
int init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    return sockfd;
}

void establish_conn(int sockfd, std::string hostname)
{
    struct sockaddr_in address;
    // TODO: connect
    struct addrinfo hints, *res, *res0;
    int error;
    const char *cause = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    error = getaddrinfo(hostname.c_str(), PORT, &hints, &res);
    
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) 
    {
	cause = "connect";
	cleanup(sockfd);
	sockfd = -1;
    }
}

int send_msg(std::string msg, int sockfd)
{
    char buffer[BUFF_SIZE];
    int n;
    memset(buffer, 0, BUFF_SIZE);
    memcpy(buffer, msg.c_str(), msg.length());
    // TODO: write
    n = write(sockfd, buffer, msg.length());
    //
    return n;
}

std::string recv_msg(int sockfd)
{
    char buffer[BUFF_SIZE];
    int n;
    memset(buffer, 0, BUFF_SIZE);
    // TODO: read
    n = read(sockfd, buffer, BUFF_SIZE);
    
    if (n < 0)
    {
	error("Cannot read");
	cleanup(sockfd);
    }
    // 
    return std::string(buffer);
}

void cleanup(int sockfd)
{
    // TODO: close
    close(sockfd);
    // closesocket
}

int main(int argc, const char *argv[])
{
    int sockfd = init();
    establish_conn(sockfd, std::string("localhost"));
    send_msg(std::string("Hello\n"), sockfd);
    std::string s = recv_msg(sockfd);
    std::cout << "final: " << s << std::endl;
    cleanup(sockfd);
    return 0;
}
