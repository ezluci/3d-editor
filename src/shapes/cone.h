#ifndef CONE_H
#define CONE_H

#include <SFML/Graphics.hpp>
#include "../shape.h"
#include "../camera.h"

class Cone : public Shape {
public: 
Cone(Camera *camera, float height, float radius);
Cone(Camera *camera, sf::Vector3f position, float height,float radius);
};
#endif