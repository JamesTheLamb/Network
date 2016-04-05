#include "/home1/scm-studs/p4041543/Documents/Tank_fw/include/Map.h"
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

int Map::Map_One(sf::RenderWindow& window, Map map_one)
{
    const int level[] =
    {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    };

    if(!map_one.LoadTiles("tiles.png", sf::Vector2u(40,40), level, 15, 15))
        return -1;

    window.draw(map_one);

    return 0;
}



