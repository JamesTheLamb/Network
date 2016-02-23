#include "Function.h"

Function::Function()
{
    //ctor
}

Function::~Function()
{
    //dtor
}

sf::Vector2f Function::Lerping(sf::Vector2f pos1, sf::Vector2f pos2, float time)
{
    return (pos1 + time*(pos2-pos1));
}
