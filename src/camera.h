#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
public:
    sf::Vector3f position;
    float fov_angle;
    sf::RenderWindow &window;

    Camera(sf::Vector3f position, float fov_angle, sf::RenderWindow &window);
};

#endif
