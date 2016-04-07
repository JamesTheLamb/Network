#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "include/Player.h"
#include "include/Map.h"
#include "/home1/scm-studs/p4041543/Documents/cpp01/client.h"

struct client_t
{
    Client c;
    int & sockfd;
    client_t(int & _sockfd):sockfd(_sockfd){}
    void operator()()
    {
        c.recv_loop(sockfd);
    }

};

struct client_two
{
    Client c;
    int & sockfd;
    client_two(int & _sockfd):sockfd(_sockfd){}

    void operator()()
    {
        c.recv_looping(sockfd);
    }
};

struct client_three
{
    Client c;
    Player& p;
    int & sockfd;
    client_three(int & _sockfd, Player& _p):sockfd(_sockfd), p(_p){}

    void operator()()
    {
        c.recv_loops(sockfd, p);
    }
};

int main()
{
    int state = 1;

    bool threaded = false;

    int height = 600;
    int width = 600;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    Player player;
    Map mapping;
    Client client;

    int sockfd = client.init();

    client.update(sockfd);

    client_t c(sockfd);
    client_two c2(sockfd);
    client_three c3(sockfd, player);

    sf::Thread t(c);
    sf::Thread t2(c2);
    sf::Thread t3(c3);

    t.launch();


    std::string s;

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(state)
            {
            case 0:
                if(!threaded)
                {
                    threaded = true;
                    t2.launch();
                }
                switch(event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    s = player.Movement(event);
                    if(s != " ")
                    {
                        client.send_msg("update_position:"+s, sockfd);
                    }

                    if(event.key.code == sf::Keyboard::Return)
                    {
                        std::cout << "Type your message in here" << std::endl;
                        std::string msg;
                        std::getline(std::cin, msg);

                        client.send_msg("greet_all:"+msg, sockfd);
                    }
                    break;
                }
                break;
            case 1:
                switch(event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if(sf::Mouse::getPosition(window).x > 260 && sf::Mouse::getPosition(window).y > 300 &&
                        sf::Mouse::getPosition(window).x < 330 && sf::Mouse::getPosition(window).y < 335)
                    {
                        client.send_msg("Login", sockfd);
                        t.wait();
                        t.terminate();
                        state = 0;
                    }

                    break;
                }
                break;
            }

        }

        window.clear();
        if(state == 0)
        {
            mapping.Map_One(window, mapping);
            window.draw(player.tank);
        }
        else if(state == 1)
        {
            mapping.Title_Screen(window, mapping);
        }
        window.display();
    }

    return 0;
}
