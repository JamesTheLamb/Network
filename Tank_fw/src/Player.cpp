#include "Player.h"

Player::Player()
{
    //ctor

    if(!img.loadFromFile("tank1.png"))
        std::cout << "Error, could not load texture" << std::endl;

    tank.setTexture(img);
    tank.setOrigin(tank.getTexture()->getSize().x/2, tank.getTexture()->getSize().y/2);
    tank.setScale(0.5f, 0.5f);

}

Player::~Player()
{
    //dtor
}
