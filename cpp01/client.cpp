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
#include "/home1/scm-studs/p4041543/Downloads/Tank_fw/include/Player.h"
#include "/home1/scm-studs/p4041543/Downloads/Tank_fw/include/Map.h""
#include "/home1/scm-studs/p4041543/Documents/cpp01/client.h"

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



void Client::recv_loop(int sockfd)
{
    std::string s;

    bool is_true = false;

    std::cout << std::to_string(is_true) << std::endl;

    while(!is_true)
    {

      s = recv_msg(sockfd);
      std::cout << "received: " << s << std::endl;


    }
}

struct client_t
        {
            Client c;
            int & sockfd;
            client_t(int & _sockfd):sockfd(_sockfd){}
            void operator()() // overloading ()
            {
                c.recv_loop(sockfd); // needs to be implemented
            }
        } /* optional variable list */;


void Client::update()
{
    int height = 600;
    int width = 600;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    Player player;
    Map mapping;

    sf::Time timer = sf::seconds(1.f);

    int sockfd = init();

    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    ioctl(sockfd, SIOCGIFADDR, &ifr);

    establish_conn(sockfd, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    client_t c(sockfd);
    std::thread t(c);

    std::string s;


    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                std::getline (std::cin, s);
                send_msg(s, sockfd);
                break;
            case sf::Event::KeyPressed:
                player.Movement(event);
                break;
            }

        }

        window.clear();
        mapping.Map_One(window, mapping);
        window.draw(player.tank);
        window.display();
    }



    t.join();

    cleanup(sockfd);
}
