#include "button.h"

Button::Button() {}

Button::Button(float x, float y, float width, float height, std::string text, const sf::Font& font, unsigned int fontSize = 30)
    : onPress(nullptr) {
    box.setPosition(x, y);
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(1);

    this->text.setString(text);
    this->text.setFont(font);
    this->text.setCharacterSize(fontSize);
    this->text.setFillColor(sf::Color::Black);
    this->text.setPosition(x + 5, y + 5);
}

void Button::setOnPress(std::function<void()> callback) {
    onPress = callback;
}

void Button::setOnPressRight(std::function<void()> callback) {
    onPressRight = callback;
}

bool Button::checkPress(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (box.getGlobalBounds().contains(mousePos)) {
                if (onPress) {
                    onPress();
                }
                return true;
            }
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (box.getGlobalBounds().contains(mousePos)) {
                if (onPressRight) {
                    onPressRight();
                }
                return true;
            }
        }
    }
    return false;
}

void Button::setPosition(const sf::Vector2f pos)
{
    this->box.setPosition(pos);
    this->text.setPosition(pos + sf::Vector2f{5, 5});
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(box);
    window.draw(text);
}

sf::Text* Button::getSfText() {
    return &this->text;
}

void Button::setFillColor(const sf::Color& color) {
    box.setFillColor(color);
}