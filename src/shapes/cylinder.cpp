#include "cylinder.h"
#include "../camera.h"
#include <iostream>
#include <cmath>

Cylinder::Cylinder(Camera *camera, float height, float radius)
    : Shape(camera) 
{
    this->position = position; //center of inferior base
    setShapeName("Cylinder");
    setSize(1);

    int segments =radius * 8;

    //inferior base
    for (int i = 0; i < segments; i++) {
        float angle = i * 2 * M_PI / segments;
        float x =  radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back(sf::Vector3f(x, 0-height, z));
    }

    //upper base
    for (int i = 0; i < segments; i++) {
        float angle = i * 2 * M_PI / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back(sf::Vector3f(x, 0 + height, z));
    }

    //connect inferior base
    for (int i = 0; i < segments; i++) {
        edges.push_back({i, (i + 1) % segments}); 
    }

    // connect upper base
    for (int i = 0; i < segments; i++) {
        edges.push_back({i + segments, (i + 1) % segments + segments});
    }

    //connect up with down
    for (int i = 0; i < segments; i++) {
        edges.push_back({i, i + segments}); 
    }
}


Cylinder::Cylinder(Camera *camera, sf::Vector3f position, float height, float radius)
    : Cylinder(camera, height, radius)
{
    this->setPosition(position);
}