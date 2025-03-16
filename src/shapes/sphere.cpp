#include "sphere.h"
#include "../camera.h"
#include <iostream>
#include <cmath>

Sphere::Sphere(Camera* camera, float radius)
    : Shape(camera)
{
    setShapeName("Sphere");
    
    float segments=radius*10;
    for (int i = 0; i <= segments; ++i) {
        float theta = i * M_PI / segments; 
        
        for (int j = 0; j <= segments; ++j) {
            float phi = j * 2 * M_PI / segments;  
            float x = radius * sin(theta) * cos(phi);//coordonats
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);
            
            vertices.push_back(sf::Vector3f(x, y, z));
        }
    }
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
        
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;

            edges.push_back({first, second});
            edges.push_back({first, first + 1});
            edges.push_back({second, second + 1});
        }
    }
}

Sphere::Sphere(Camera* camera, sf::Vector3f position, float radius)
    : Sphere(camera, radius)
{
    this->setPosition(position);
    this->setSize(1);
}