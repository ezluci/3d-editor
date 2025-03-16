#include "cone.h"
#include "../camera.h"
#include <iostream>
#include <cmath>

Cone::Cone(Camera *camera, float height, float radius)
    : Shape(camera) 
{
    setShapeName("Cone");
    setSize(1);

    int segments = radius * 8;

    //apex
    vertices.push_back(sf::Vector3f(0, 0 + height, 0));

    
    for (int i = 0; i < segments; i++) {
        float angle = i * 2 * M_PI / segments;
        float x = radius * cos(angle);
        float z =  radius * sin(angle);
        vertices.push_back(sf::Vector3f(x, 0, z));
    }

    
    for (int i = 1; i <= segments; i++) {
        edges.push_back({0, i});
        edges.push_back({i, i % segments + 1}); 
    }
}


Cone::Cone(Camera *camera, sf::Vector3f position, float height, float radius)
    : Cone(camera, height, radius)
{
    this->setPosition(position);
}