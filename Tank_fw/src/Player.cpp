#include "Player.h"
#include <iostream>
#include <sstream>

Player::Player()
{
    //ctor

    if(!img.loadFromFile("tank1.png"))
        std::cout << "Error, could not load texture" << std::endl;

    tank.setTexture(img);
    tank.setOrigin(tank.getTexture()->getSize().x/2, tank.getTexture()->getSize().y/2);
    tank.setScale(0.2f, 0.2f);

    position = sf::Vector2f(20, 20);

    tank.setPosition(position);

}

Player::~Player()
{
    //dtor
}

void Player::Movement(sf::Event event)
{
    if(event.key.code == sf::Keyboard::Up)
    {
        if(tank.getRotation() != 270)
            tank.setRotation(270);
        else
        {
            position.y -= 20;

            if(position.y <= 20)
                position.y = 20;
        }

    }
    else if(event.key.code == sf::Keyboard::Right)
    {
        if(tank.getRotation() != 0)
            tank.setRotation(0);
        else
        {
            position.x += 20;

            if(position.x >= 480)
                position.x = 480;
        }

    }
    else if(event.key.code == sf::Keyboard::Down)
    {
        if(tank.getRotation() != 90)
            tank.setRotation(90);
        else
        {
            position.y += 20;

            if(position.y >= 480)
                position.y = 480;

        }

    }
    else if(event.key.code == sf::Keyboard::Left)
    {
        if(tank.getRotation() != 180)
            tank.setRotation(180);
        else
        {
            position.x -= 20;

            if(position.x <= 20)
                position.x = 20;
        }
    }
    tank.setPosition(position);

}

void Player::HUD(sf::RenderWindow& window)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text HP("Health: ", font, 20);
    HP.setColor(sf::Color(255,0,0,255));

    std::stringstream ss;
    ss << health;
    std::string s = ss.str();
    sf::Text health_(s, font, 20);
    health_.setColor(sf::Color(255,0,0,255));
    health_.setPosition(sf::Vector2f(100, 0));

    window.draw(HP);
    window.draw(health_);
}
