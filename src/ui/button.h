#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <string>

class Button {
public:
    Button();
    Button(float x, float y, float width, float height, std::string text, const sf::Font& font, unsigned int fontSize);
    void setOnPress(std::function<void()> callback);
    void setOnPressRight(std::function<void()> callback);
    bool checkPress(const sf::Event& event);
    void setPosition(const sf::Vector2f pos);
    void draw(sf::RenderWindow& window) const;

    sf::Text* getSfText();
    void setFillColor(const sf::Color& color);
private:
    sf::RectangleShape box;
    sf::Text text;
    std::function<void()> onPress;
    std::function<void()> onPressRight;
};

#endif // BUTTON_H