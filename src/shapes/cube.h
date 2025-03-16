#ifndef CUBE_H
#define CUBE_H

#include <SFML/Graphics.hpp>
#include "../shape.h"
#include "../camera.h"

class Cube : public Shape {
public:
    Cube(Camera *camera);
    Cube(Camera *camera, sf::Vector3f position, float size);
};
#endif