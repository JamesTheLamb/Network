#include <iostream>
#include <string>
#include <thread>
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

void cleanup(int sockfd)
{
    close(sockfd);
    // closesocket
}

int init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    return sockfd;
}

void establish_conn(int sockfd, std::string hostname)
{
    struct sockaddr_in address;
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
    int n = 0;
    memset(buffer, 0, BUFF_SIZE);
    memcpy(buffer, msg.c_str(), msg.length());
    n = write(sockfd, buffer, msg.length());
    //
    return n;
}

std::string recv_msg(int sockfd)
{
    char buffer[BUFF_SIZE];
    int n;
    memset(buffer, 0, BUFF_SIZE);
    n = read(sockfd, buffer, BUFF_SIZE);

    if (n < 0)
    {
	//error("Cannot read");
	std::cout << "ERROR" << std::endl;
	cleanup(sockfd);
    }
    //
    return std::string(buffer);
}



void recv_loop(int sockfd)
{
    std::string s;

    bool is_true = false;
  while(!is_true)
    {
      //std::cin >> s;
      //send_msg(s, sockfd);
      s = recv_msg(sockfd);
      std::cout << s << std::endl;

      if(s.length() > 1)
        is_true = true;
    }
}

int update()
{
    int sockfd = init();
    establish_conn(sockfd, std::string("localhost"));

    client_t c(sockfd);
    std::thread t(c);
    std::string s;

    bool n = false;

    while(!n)
    {
      std::getline (std::cin, s);
      send_msg(s, sockfd);

      if(send_msg(s, sockfd) > 0)
        n = true;

      //s = recv_msg(sockfd);
      //std::cout << s << std::endl;
    }

    t.join();

    cleanup(sockfd);
    return 0;
}
