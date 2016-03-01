#include <SFML/Graphics.hpp>
#include <iostream>
#include "Function.h"
#include "Player.h"
#include

int main()
{
    enum Direction {Up, Right, Down, Left};

    int height = 1000;
    int width = 1000;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    Player player;

    sf::Vector2f position(player.tank.getOrigin().x - 45, player.tank.getOrigin().y - 30);

    player.tank.setPosition(position);

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
                if(event.key.code == sf::Keyboard::Up)
                {
                    position.y -= 20;
                    player.tank.setRotation(-90);
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    position.x += 20;
                    player.tank.setRotation(0);

                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    position.y += 20;
                    player.tank.setRotation(90);

                }
                else if(event.key.code == sf::Keyboard::Left)
                {
                    position.x -= 20;
                    player.tank.setRotation(180);

                }

                player.tank.setPosition(position);
                break;
            }
        }

        window.clear();
        window.draw(player.tank);
        window.display();
    }
    return 0;
}
