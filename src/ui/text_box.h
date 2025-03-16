#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../shape.h"
class TextBox {
public:
    TextBox(float x, float y, float width, float height, const sf::Font& font, unsigned int fontSize = 30);
    bool contains(const sf::Vector2f& point) const;
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;
    void setText(const std::string& text);
    std::string getText() const;
    float getTextf() const;
    void setSelected(bool val);

private:
    sf::RectangleShape box;
    sf::Text text;
    bool isSelected;
};

#endif 