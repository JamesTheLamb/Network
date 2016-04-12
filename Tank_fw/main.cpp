#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "include/Player.h"
#include "include/Map.h"
#include "/home1/scm-studs/p4041543/Documents/cpp01/client.h"

struct client_t
{
    Client c;
    Player& p;
    int & sockfd;
    client_t(int & _sockfd, Player& _p):sockfd(_sockfd), p(_p){}
    void operator()()
    {
        c.recv_loop(sockfd, p);

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
        p.SetDirection(c.recv_msg(sockfd));
    }
};

struct sending
{
    Client c;
    int& sockfd;
    int& player_count;
    sending(int& _sockfd, int& _player_count):sockfd(_sockfd), player_count(_player_count){}

    void operator()()
    {
        player_count = std::stoi(c.recv_msg(sockfd));
    }
};

struct get_players
{
    Client c;
    int & sockfd;
    get_players(int & _sockfd):sockfd(_sockfd){}

    void operator()()
    {
        c.recv_msg(sockfd);
    }

};

int main()
{
    int state = 1;

    int player_count = 0;

    bool threaded = true;

    int height = 600;
    int width = 600;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    Player player[4] = {Player(0, 0),Player(0, 0),Player(0, 0),Player(0, 0)};
    Map mapping;
    Client client;

    int sockfd = client.init();

    client.update(sockfd);

    client_t c(sockfd, player[0]);
    client_two c2(sockfd);
    client_three c3(sockfd, player[0]);
    sending s(sockfd, player_count);

    sf::Thread t(c);
    sf::Thread t2(c2);
    sf::Thread t3(c3);
    sf::Thread t4(s);

    t.launch();

    player_count++;
    player[0].SetPosition(60,140);


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
                    std::string s;
                    s = player[0].Movement(event);
                    if(s != " ")
                    {
                        t2.terminate();
                        threaded = false;

                        t3.launch();

                        client.send_msg("update_position:"+s, sockfd);

                        t3.wait();

                        t3.terminate();


                    }

                    if(event.key.code == sf::Keyboard::Return)
                    {
                        std::cout << "Type your message in here" << std::endl;
                        std::string msg;
                        std::getline(std::cin, msg);

                        client.send_msg("greet_all:"+msg, sockfd);
                    }

                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        client.send_msg("unregister:"+player[0].GetName(), sockfd);
                        t2.terminate();
                        state = 1;
                    }

                    if(event.key.code == sf::Keyboard::G && player_count < 4)
                    {
                        t2.terminate();
                        threaded = false;

                        t4.launch();

                        client.send_msg("get_all_players:", sockfd);

                        t4.terminate();

                        player_count++;
                        player[1].SetPosition(580, 580);
                        player[1].SetDirection("left");
                    }

                    break;

                }
                break;
            case 1:
                if(threaded)
                {
                    t.launch();
                    threaded = false;
                }

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
            for(int i = 0; i < player_count; i++)
            {
                window.draw(player[i].tank);
            }
        }
        else if(state == 1)
        {
            mapping.Title_Screen(window, mapping);
        }
        window.display();
    }

    return 0;
}
