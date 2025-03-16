#ifndef CYLINDER_H
#define CYLINDER_H

#include <SFML/Graphics.hpp>
#include "../shape.h"
#include "../camera.h"

class Cylinder : public Shape{
public:
Cylinder(Camera *camera, float height, float radius);
Cylinder(Camera *camera, sf::Vector3f position, float height, float radius);
};
#endif
