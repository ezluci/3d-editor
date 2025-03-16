#include "pyramid.h"
#include "../camera.h"
#include <iostream>

Pyramid::Pyramid(Camera *camera, float height)
    : Shape(camera)
{
    setShapeName("Pyramid");
    
    float inaltime= height/2;
    vertices.push_back({-1, -1, 0});//0
    vertices.push_back({-1, +1, 0});//1
    vertices.push_back({+1, -1, 0});//2
    vertices.push_back({+1, +1, 0});//3
    vertices.push_back({0,0,inaltime});//4

    edges.push_back({0,1});
    edges.push_back({1,3});
    edges.push_back({3,2});
    edges.push_back({2,0});
    edges.push_back({0,4});
    edges.push_back({1,4});
    edges.push_back({2,4});
    edges.push_back({3,4});
}

Pyramid::Pyramid(Camera *camera, sf::Vector3f position, float height, float size)
    : Pyramid(camera, height)
{
    this->setPosition(position);
    this->setSize(size);
}