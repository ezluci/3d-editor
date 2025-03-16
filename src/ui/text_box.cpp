#include "text_box.h"

TextBox::TextBox(float x, float y, float width, float height, const sf::Font& font, unsigned int fontSize)
    : isSelected(false)
{
    box.setPosition(x, y);
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(1);

    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 5, y + 5);
}

void TextBox::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            isSelected = true;
        } else {
            isSelected = false;
        }
    }

    if (isSelected && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) { // Backspace
            std::string str = text.getString();
            if (!str.empty()) {
                str.pop_back();
                text.setString(str);
            }
        } else if (event.text.unicode < 128) { // ASCII characters
            text.setString(text.getString() + static_cast<char>(event.text.unicode));
        }
    }
}

void TextBox::draw(sf::RenderWindow& window) const
{
    window.draw(box);
    window.draw(text);
}

void TextBox::setText(const std::string& text)
{
    this->text.setString(text);
}

std::string TextBox::getText() const
{
    return text.getString();
}

float TextBox::getTextf() const
{
    float x;
    try {
        x = std::stof(static_cast<std::string>(this->getText()));
    } catch (const std::exception &e) {
        x = 0;
    }
    return x;
}
bool TextBox::contains(const sf::Vector2f& point) const {
    return box.getGlobalBounds().contains(point);
}

void TextBox::setSelected(bool val)
{
    this->isSelected = val;
}