#ifndef SPHERE_H
#define SPHERE_H

#include <SFML/Graphics.hpp>
#include "../shape.h"
#include "../camera.h"


class Sphere : public Shape{

public:
Sphere(Camera *camera, sf::Vector3f position, float radius);
Sphere(Camera *camera, float radius);
};
#endif