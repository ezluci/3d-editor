#include "cube.h"
#include "../camera.h"
#include <iostream>

Cube::Cube(Camera *camera)
    : Shape(camera)
{
    setShapeName("Cube");
    
    vertices.push_back({-1, -1, -1});//0
    vertices.push_back({-1, +1, -1});//1
    vertices.push_back({+1, -1, -1});//2
    vertices.push_back({+1, +1, -1});//3
    vertices.push_back({-1, -1, +1});//4
    vertices.push_back({-1, +1, +1});//5
    vertices.push_back({+1, -1, +1});//6
    vertices.push_back({+1, +1, +1});//7

    edges.push_back({0, 1});
    edges.push_back({0, 2});
    edges.push_back({0, 4});
    edges.push_back({1, 3});
    edges.push_back({1, 5});
    edges.push_back({2, 3});
    edges.push_back({2, 6});
    edges.push_back({3, 7});
    edges.push_back({4, 5});
    edges.push_back({4, 6});
    edges.push_back({5, 7});
    edges.push_back({6, 7});
}

Cube::Cube(Camera *camera, sf::Vector3f position, float size)
    : Cube(camera)
{
    setPosition(position);
    setSize(size);
}