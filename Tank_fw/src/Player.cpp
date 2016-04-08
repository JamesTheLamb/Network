#include "/home1/scm-studs/p4041543/Documents/Tank_fw/include/Player.h"
#include <iostream>
#include <sstream>

Player::Player(int x, int y)
{
    //ctor

    if(!img.loadFromFile("tank1.png"))
        std::cout << "Error, could not load texture" << std::endl;

    tank.setTexture(img);
    tank.setOrigin(tank.getTexture()->getSize().x/2, tank.getTexture()->getSize().y/2);
    tank.setScale(0.2f, 0.2f);

    tank.setPosition(20, 20);
    SetPosition(20,20);

}

Player::~Player()
{
    //dtor
}

std::string Player::Movement(sf::Event event)
{
    std::string s = " ";
    if(event.key.code == sf::Keyboard::Up)
    {
        if(tank.getRotation() != 270)
            tank.setRotation(270);
        else
        {
            position.y -= 40;

            if(position.y < 20)
                position.y = 20;
            else
                s = "up";

        }

    }
    else if(event.key.code == sf::Keyboard::Right)
    {
        if(tank.getRotation() != 0)
            tank.setRotation(0);
        else
        {
            position.x += 40;

            if(position.x > 580)
                position.x = 580;
            else
                s = "right";
        }


    }
    else if(event.key.code == sf::Keyboard::Down)
    {
        if(tank.getRotation() != 90)
            tank.setRotation(90);
        else
        {
            position.y += 40;

            if(position.y > 580)
                position.y = 580;
            else
                s = "down";
        }


    }
    else if(event.key.code == sf::Keyboard::Left)
    {
        if(tank.getRotation() != 180)
            tank.setRotation(180);
        else
        {
            position.x -= 40;

            if(position.x < 20)
                position.x = 20;
            else
                s = "left";
        }


    }
    SetPosition(position);
    tank.setPosition(position.x, position.y);
    return s;

}

void Player::SetPosition(sf::Vector2i pos)
{
    position = pos;
}

void Player::SetPosition(int x, int y)
{
    position.x = x;
    position.y = y;

    tank.setPosition(position.x, position.y);
}

void Player::SetX(int x)
{
    position.x = x;
    tank.setPosition(position.x, position.y);
}

void Player::SetY(int y)
{
    position.y = y;
    tank.setPosition(position.x, position.y);
}

void Player::SetDirection(std::string s)
{
    direction = s;

    if(direction == "up")
        tank.setRotation(270);

    if(direction == "down")
        tank.setRotation(90);

    if(direction == "right")
        tank.setRotation(0);

    if(direction == "left")
        tank.setRotation(180);
}

std::string Player::GetDirection()
{
    return direction;
}

void Player::SetName(std::string s)
{
    name = s;
}

std::string Player::GetName()
{
    return name;
}
