#include <SFML/Graphics.hpp>

int main()
{
    int height = 1000;
    int width = 1000;
    sf::RenderWindow window(sf::VideoMode(height, width), "SFML works!");

    sf::Image image;
    image.create(1000, 1000, sf::Color::Black);

    bool isBlackPixel = false;
    sf::Color blackPixel(0,0,0,255);
    sf::Color whitePixel(255, 255, 255, 255);

    //Loop through each vertical row of the image
    for (int y = 0; y < 1000; y++)
    {
        //then horizontal, setting pixels to black or white in blocks of 8
        for (int x = 0; x < 1000; x++)
        {
            if (isBlackPixel)
                image.setPixel(x, y, blackPixel);
            else
                image.setPixel(x, y, whitePixel);
            // Every 8th flip colour
            if (!(x % 50))
                isBlackPixel = !isBlackPixel;
        }
    // Offset again on vertical lines to create a checkerboard effect
        if(!(y%50))
        isBlackPixel = !isBlackPixel;
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
