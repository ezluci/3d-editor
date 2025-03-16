#include "camera.h"

Camera::Camera(sf::Vector3f position, float fov_angle, sf::RenderWindow &window)
    : position(position), fov_angle(fov_angle), window(window) {}
