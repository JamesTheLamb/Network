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
}

int init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    return sockfd;
}

void establish_conn(int sockfd, std::string hostname)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rv = getaddrinfo(hostname.c_str(), PORT, &hints, &res);
    if (rv != 0) {
        printf("%s\n", gai_strerror(rv));
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    rv = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (rv != 0) {
        perror("Error on Connect (client side)\n");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(res);
    std::cout << "connected" << std::endl;
}
        
	
int send_msg(std::string msg, int sockfd)
{
    char buffer[BUFF_SIZE];
    int n = 0;
    memset(buffer, 0, BUFF_SIZE);
    memcpy(buffer, msg.c_str(), msg.length());
    n = write(sockfd, buffer, msg.length());
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
	std::cout << "ERROR" << std::endl;
	cleanup(sockfd);
    }
   
    return std::string(buffer);
}



void recv_loop(int sockfd)
{
    std::string s;

    bool is_true = false;
    while(!is_true)
    {
  
      s = recv_msg(sockfd);
      std::cout << "received: " << s << std::endl;

   
    }
}

struct client_t
        {
            int & sockfd;
            client_t(int & _sockfd):sockfd(_sockfd){}
            void operator()() // overloading ()
            {
                recv_loop(sockfd); // needs to be implemented
            }
        } /* optional variable list */;
	
	
int main()
{
    int sockfd = init();
    establish_conn(sockfd, std::string("localhost"));

    
    client_t c(sockfd);
    std::thread t(c);
    
    
    std::string s;
    
    while(s != "quit\n")
    {
      std::getline (std::cin, s);
      send_msg(s, sockfd);
    }
    

    t.join();


    cleanup(sockfd);
    return 0;
}
