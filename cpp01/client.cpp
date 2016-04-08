#include <iostream>
#include <string>
#include <thread>
#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "client.h"
#include "../Tank_fw/include/Player.h"
#include "../Tank_fw/include/Map.h"

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

Client::Client()
{
}

Client::~Client()
{
}

void Client::cleanup(int sockfd)
{
    close(sockfd);
}

int Client::init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    return sockfd;
}

void Client::establish_conn(int sockfd, std::string hostname)
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


int Client::send_msg(std::string msg, int sockfd)
{
    char buffer[BUFF_SIZE];
    int n = 0;
    memset(buffer, 0, BUFF_SIZE);
    memcpy(buffer, msg.c_str(), msg.length());
    n = write(sockfd, buffer, msg.length());
    return n;
}

std::string Client::recv_msg(int sockfd)
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

std::string Client::registration(int sockfd)
{
    std::string s;

    std::cout << "Enter name: ";

    std::cin >> s ;

    std::string x;
    int x_int;

    std::cout << "Enter X between 1 - 15: ";

    std::cin >> x;
    x_int = std::stoi(x);
    x_int *= 40;
    x_int -= 20;

    std::string y;
    int y_int;

    std::cout << "Enter Y between 1 - 15: ";

    std::cin >> y;
    y_int = std::stoi(y);
    y_int *= 40;
    y_int -= 20;

    send_msg("reg:"+s+":"+std::to_string(x_int)+":"+std::to_string(y_int), sockfd);
    return s;

}

void Client::recv_loop(int sockfd, Player& p)
{
    std::string s;
    std::string n;

    bool is_true = false;

    while(!is_true)
    {
      s = recv_msg(sockfd);
      std::cout << "received: " << s << std::endl;

      if(s.substr(0,3) != n.substr(0,3))
      {

        n = registration(sockfd);
        p.SetName(n);

      }
      else
      {
        is_true = true;
      }

    }
}

void Client::recv_looping(int sockfd)
{
    std::string s;

    bool is_true = false;

    while(!is_true)
    {
        s = recv_msg(sockfd);
        std::cout << "received: " << s << std::endl;

        if(s.find("!") == s.length() - 1)
        {
            std::cout << "Player!" << std::endl;
        }

    }
}

void Client::recv_loops(int sockfd, Player p)
{
    std::string s;
    //sf::Mutex mutex;

    bool is_true = false;
    bool is_x = false;
    bool is_y = false;

    int x;
    int y;

    while(!is_true)
    {
        s = recv_msg(sockfd);

        //mutex.lock();
        //try
        //{
            std::cout << "received: " << s << std::endl;
        //}
        //catch (std::string e)
        //{
        //    mutex.unlock();
        //    throw e;
        //}
        //mutex.unlock();

        if(is_y)
        {
            x = std::stoi(s);
            p.SetX(x);
            return;
        }
        else if(is_x)
        {
            y = std::stoi(s);
            p.SetY(y);
            return;
        }

        if(s == "X")
        {
            is_x = true;
        }
        else if(s == "Y")
        {
            is_y = true;
        }

    }

    std::cout << "Out" << std::endl;
}

void Client::update(int sockfd)
{
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    ioctl(sockfd, SIOCGIFADDR, &ifr);

    std::cout << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) << std::endl;

    establish_conn(sockfd, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));


    //t.join();

    //cleanup(sockfd);
}
