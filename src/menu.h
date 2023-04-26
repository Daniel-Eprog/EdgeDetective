#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "button.h"

class Menu //STATIC
{
public:
    static void openMenu()
    {
        //Setup of variables
        auto window = sf::RenderWindow{ { 1280u, 720u }, "Edge Detective" };
        window.setFramerateLimit(30);

        sf::Font font;
        if(!font.loadFromFile("../src/fonts/arial.ttf"))
            std::cout << "Error: font could not be loaded" << std::endl;

        std::vector<Button> buttonsList;
        buttonsList.reserve(10);
        buttonsList.emplace_back(sf::Text("Load File", font), sf::Vector2f(300,400), sf::Vector2f(200,60));

        sf::Vector2i mousePositionInt;
        sf::Vector2f mousePositionFloat;

        bool mousePressedThisFrame = false;
        bool mouseReleasedThisFrame = false;

        //SFML main loop
        while (window.isOpen())
        {
            window.clear(sf::Color(60, 60, 60, 255));


            //Handle User Inputs
            mousePressedThisFrame = false;
            mouseReleasedThisFrame = false;
            for (auto event = sf::Event{}; window.pollEvent(event);)
            {
                switch(event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::MouseButtonPressed:
                        mousePressedThisFrame = true;
                        break;
                    case sf::Event::MouseButtonReleased:
                        mouseReleasedThisFrame = true;
                        break;
                    default:
                        break;
                }
            }

            //Change status of each button depending on the mouse, then draw each button
            mousePositionInt = sf::Mouse::getPosition(window);
            mousePositionFloat = sf::Vector2f(static_cast<float>(mousePositionInt.x), static_cast<float>(mousePositionInt.y));
            for(Button& currentButton: buttonsList)
            {
                switch (currentButton.status)
                {
                    case Button::Status::Default:
                        if (currentButton.mouseIsOver(mousePositionFloat))      //If mouse is over button
                            currentButton.updateStatus(Button::Status::Hover);
                        break;
                    case Button::Status::Hover:
                        if (!currentButton.mouseIsOver(mousePositionFloat))     //If mouse is NOT over button
                            currentButton.updateStatus(Button::Status::Default);
                        else if(mousePressedThisFrame)                                      //If mouse is over button and clicked
                            currentButton.updateStatus(Button::Status::Clicked);
                        break;
                    case Button::Status::Clicked:
                        if (!currentButton.mouseIsOver(mousePositionFloat))     //If mouse is NOT over button
                            currentButton.updateStatus(Button::Status::Default);
                        else if(mouseReleasedThisFrame)                                     //If mouse is over button and released
                            currentButton.updateStatus(Button::Status::Hover);
                        break;
                }
                window.draw(currentButton);
            }
            window.display();
        }
    }
};

#endif