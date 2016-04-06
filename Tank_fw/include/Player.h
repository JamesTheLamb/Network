#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{
    public:
        Player();
        virtual ~Player();

        sf::Texture img;
        sf::Sprite tank;

        std::string Movement(sf::Event event);

        void HUD(sf::RenderWindow& window);

    protected:

    private:
        sf::Vector2f position;
        int health = 100;

        int strength = 1;
        int defense = 1;
        int evasion = 1;

};

#endif // PLAYER_H
