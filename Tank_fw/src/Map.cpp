#include "../include/Map.h"

Map::Map()
{
    //ctor
}

Map::~Map()
{
    //dtor
}

bool Map::LoadTiles(const std::string& tile_name, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height)
{
    if(!tile_set.loadFromFile(tile_name))
        return false;

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    for(unsigned int j = 0; j < height; j++)
    {
        for(unsigned int i = 0; i < width; i++)
        {
            int tile_number = tiles[i+j*width];

            int U = tile_number % (tile_set.getSize().x/tile_size.x);
            int V = tile_number / (tile_set.getSize().x / tile_size.x);

            sf::Vertex* q = &vertices[(i + j * width) * 4];

            q[0].position = sf::Vector2f(i*tile_size.x, j*tile_size.y);
            q[1].position = sf::Vector2f((i+1)*tile_size.x, j*tile_size.y);
            q[2].position = sf::Vector2f((i+1)*tile_size.x, (j+1)*tile_size.y);
            q[3].position = sf::Vector2f(i*tile_size.x, (j+1)*tile_size.y);

            q[0].texCoords = sf::Vector2f(U*tile_size.x, V*tile_size.y);
            q[1].texCoords = sf::Vector2f((U+1)*tile_size.x, V*tile_size.y);
            q[2].texCoords = sf::Vector2f((U+1)*tile_size.x, (V+1)*tile_size.y);
            q[3].texCoords = sf::Vector2f(U*tile_size.x, (V+1)*tile_size.y);
        }
    }

    return true;
}

void Map::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
    states.transform *= getTransform();

    states.texture = &tile_set;

    target.draw(vertices, states);
}

sf::Text Map::Text_Setup(sf::Text text, sf::Font font, std::string str, int char_size, sf::Color color, bool bold, sf::Vector2f pos)
{
    text.setFont(font);

    text.setString(str);

    text.setCharacterSize(char_size);

    text.setColor(color);

    if(bold)
        text.setStyle(sf::Text::Bold);

    text.setPosition(pos);

    return text;
}

int Map::Title_Screen(sf::RenderWindow& window, Map title)
{
    sf::Text text;

    sf::Text play;

    bool bold = true;

    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
        return -1;

    window.draw(Text_Setup(text, font, "Tanky Gamey", 35, sf::Color::Red, bold, sf::Vector2f(190, 200)));

    if(sf::Mouse::getPosition(window).x > 260 && sf::Mouse::getPosition(window).y > 300 &&
        sf::Mouse::getPosition(window).x < 330 && sf::Mouse::getPosition(window).y < 335)
        window.draw(Text_Setup(play, font, "Play", 24, sf::Color::Magenta, bold, sf::Vector2f(270, 300)));
    else
        window.draw(Text_Setup(play, font, "Play", 24, sf::Color::Red, bold, sf::Vector2f(270, 300)));
}

int Map::Map_One(sf::RenderWindow& window, Map map_one)
{
    const int level[] =
    {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,3,3,3,1,1,1,3,3,3,1,1,1,
        1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,
        1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,
        1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,
        1,1,1,3,3,3,1,1,1,3,3,3,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    };

    if(!map_one.LoadTiles("tiles.png", sf::Vector2u(40,40), level, 15, 15))
        return -1;

    window.draw(map_one);

    return 0;
}



