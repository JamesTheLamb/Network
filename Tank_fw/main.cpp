#include <iostream>
#include "Function.h"

int main()
{
    Function funky;

    enum Direction {Up, Right, Down, Left};

    int height = 1000;
    int width = 1000;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    float temp = 20;

    sf::Texture img;
    if(!img.loadFromFile("tank1.png"))
        std::cout << "Error, could not load texture" << std::endl;

    sf::Sprite tank;

    tank.setTexture(img);
    tank.setOrigin(tank.getTexture()->getSize().x/2, tank.getTexture()->getSize().y/2);
    tank.setScale(0.5f, 0.5f);

    sf::Vector2f position(tank.getOrigin().x, tank.getOrigin().y);

    tank.setPosition(position);

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
                    position = funky.lerping(position, sf::Vector2f(position.x, position.y+20), temp);
                    tank.setRotation(-90);
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    position.x += 20;
                    tank.setRotation(0);

                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    position.y += 20;
                    tank.setRotation(90);

                }
                else if(event.key.code == sf::Keyboard::Left)
                {
                    position.x -= 20;
                    tank.setRotation(180);

                }

                tank.setPosition(position);
                break;
            }
        }

        window.clear();
        window.draw(tank);
        window.display();
    }
    return 0;
}
