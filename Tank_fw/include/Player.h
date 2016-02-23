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

    protected:

    private:


};

#endif // PLAYER_H
