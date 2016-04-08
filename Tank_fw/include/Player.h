#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>


class Player
{
    public:
        Player(int x, int y);
        virtual ~Player();

        sf::Texture img;
        sf::Sprite tank;

        std::string Movement(sf::Event event);

        void SetPosition(sf::Vector2i pos);
        void SetPosition(int x, int y);
        void SetX(int x);
        void SetY(int y);

        void SetDirection(std::string s);
        std::string GetDirection();

        void SetName(std::string s);
        std::string GetName();

    protected:

    private:
        std::string name;

        sf::Vector2i position;
        int health = 100;

        int strength = 1;
        int defense = 1;
        int evasion = 1;

        std::string direction;

};

#endif // PLAYER_H
