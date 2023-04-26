#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <utility>

const sf::Color DEFAULT_COLOR = sf::Color(120, 120, 123, 255);
const sf::Color ON_HOVER_COLOR = sf::Color(135, 135, 138, 255);
const sf::Color ON_CLICKED_COLOR = sf::Color(100, 100, 103, 255);

class Button: public sf::Drawable
{
public:

    enum class Status {Default, Hover, Clicked};

    sf::RectangleShape box;
    sf::Text text;
    Status status;

    Button(sf::Text buttonText, const sf::Vector2f& position, const sf::Vector2f& size): text(std::move(buttonText)), status(Status::Default)
    {
        box = sf::RectangleShape(size);
        box.setPosition(position);
        box.setOrigin(box.getLocalBounds().getPosition() + (box.getGlobalBounds().getSize() * 0.5f));
        box.setFillColor(DEFAULT_COLOR);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(3);


        text.setCharacterSize((size.y * 0.9) - 10);
        text.setPosition(position);
        text.setOrigin(text.getLocalBounds().getPosition() + (text.getGlobalBounds().getSize() * 0.5f));
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
    }

    std::string getStatus(){
        switch (status)
        {
            case Status::Hover:
                return "Hover";
            case Status::Clicked:
                return "Clicked";
            default:
                return "Default";
        }
    }

    void updateStatus(Status newStatus)
    {
        status = newStatus;
        switch (status)
        {
            case Status::Hover:
                box.setFillColor(ON_HOVER_COLOR);
                break;
            case Status::Clicked:
                box.setFillColor(ON_CLICKED_COLOR);
                break;
            default:
                box.setFillColor(DEFAULT_COLOR);
                break;
        }
    }

    bool mouseIsOver(const sf::Vector2f& mousePosition){
        return box.getGlobalBounds().contains(mousePosition);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(box);
        target.draw(text);
    }

};

#endif //BUTTON_H
