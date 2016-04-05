#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "/home1/scm-studs/p4041543/Downloads/Tank_fw/include/Player.h"
#include "/home1/scm-studs/p4041543/Downloads/Tank_fw/include/Map.h""
#include "/home1/scm-studs/p4041543/Downloads/cpp01/client.h"

int main()
{
    int height = 600;
    int width = 600;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    Client client;

    client.update();

    Player player;
    Map mapping;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
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





    return 0;
}
