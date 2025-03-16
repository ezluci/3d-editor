#ifndef PYRAMID_H
#define PYRAMID_H

#include <SFML/Graphics.hpp>
#include "../shape.h"
#include "../camera.h"

class Pyramid : public Shape {
public:
    Pyramid(Camera *camera, float height);
    Pyramid(Camera *camera, sf::Vector3f position, float height, float size);
};
#endif