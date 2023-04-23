#include <iostream>
#include <SFML/Graphics.hpp>
#include "tga.h"

using namespace std;

int main()
{
    auto window = sf::RenderWindow{ { 1280u, 720u }, "Edge Detective" };
    auto sidePanel = sf::RenderWindow{{400u, 600u}, "Options Menu"};
    sidePanel.setFramerateLimit(60);
    window.setFramerateLimit(60);

    sf::Font font;
    if(!font.loadFromFile("fonts/arial.ttf"))
    {
        cout <<"Error font could not be loaded" << endl;
    }

    sf::Text text;

    // select the font
    text.setFont(font); // font is a sf::Font

    // set the string to display
    text.setString("Hello world");

    // set the character size
    text.setCharacterSize(200); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::Red);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);



    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    while (sidePanel.isOpen())
    {
          for (auto event = sf::Event{}; sidePanel.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                sidePanel.close();
            }
        }

        sidePanel.clear();
        sidePanel.draw(text);
        sidePanel.display();

    }
}

//cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
//cmake --build build