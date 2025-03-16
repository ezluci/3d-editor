#ifndef POINTS_MATH_H
#define POINTS_MATH_H

#include <SFML/Graphics.hpp>
#include "camera.h"

sf::Vector3f inverseRotatePoint3D(sf::Vector3f point, sf::Vector3f rotation);
sf::Vector3f rotatePoint3D(sf::Vector3f point, sf::Vector3f rotation);
sf::Vector2f projectPoint(sf::Vector3f point, const Camera* camera, const sf::Vector2u& window_size);

#endif