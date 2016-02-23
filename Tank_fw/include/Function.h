#ifndef FUNCTION_H
#define FUNCTION_H

#include <SFML/Graphics.hpp>


class Function
{
    public:
        Function();
        virtual ~Function();

        sf::Vector2f Lerping(sf::Vector2f pos1, sf::Vector2f pos2, float time);

    protected:
    private:
};

#endif // FUNCTION_H
