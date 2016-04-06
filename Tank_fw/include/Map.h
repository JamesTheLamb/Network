#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Map : sf::Drawable, sf::Transformable
{
    public:
        Map();
        virtual ~Map();

        bool LoadTiles(const std::string& tile_name, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height);

        sf::Text Text_Setup(sf::Text text, sf::Font font, std::string str, int char_size, sf::Color color, bool bold, sf::Vector2f pos);

        int Title_Screen(sf::RenderWindow& window, Map title);

        int Map_One(sf::RenderWindow& window, Map map_one);

    protected:
    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::VertexArray vertices;
        sf::Texture tile_set;
};

#endif // MAP_H
