#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>

const sf::Color DEFAULT_COLOR = sf::Color(120, 120, 123, 255);
const sf::Color ON_HOVER_COLOR = sf::Color(140, 140, 143, 255);
const sf::Color ON_CLICKED_COLOR = sf::Color(80, 80, 83, 255);

class Button: public sf::Drawable
{
public:
    enum class Status {Default, Hover, Clicked, Disabled};

private:

    sf::RectangleShape box;
    sf::Text text;
    Status status;
    void(*functionOnClick)();

public:
    Button(sf::Text buttonText, const sf::Vector2f& position, const sf::Vector2f& size, void(*buttonFunction)()):
    box(sf::RectangleShape(size)), text(std::move(buttonText)), status(Status::Default), functionOnClick(buttonFunction)
    {
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

    Status getStatus(){
        return status;
    }

    void updateStatus(Status newStatus)
    {
        status = newStatus;
        switch (status)
        {
            case Status::Hover:
                box.setFillColor(ON_HOVER_COLOR);
                break;
            case Status::Disabled:
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

    void callFunction(){
        functionOnClick();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(box);
        target.draw(text);
    }

};

#endif //BUTTON_H
