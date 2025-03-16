#include <SFML/Graphics.hpp>
#include <cmath>
#include "camera.h"

sf::Vector3f inverseRotatePoint3D(sf::Vector3f point, sf::Vector3f rotation)
{
    sf::Vector3f rotated_point;
    float cx = cos(rotation.x), sx = sin(rotation.x);
    float cy = cos(rotation.y), sy = sin(rotation.y);
    float cz = cos(rotation.z), sz = sin(rotation.z);

    rotated_point.x = point.x * (cy * cz)
                    + point.y * (sx * sy * cz + cx * sz)
                    + point.z * (cx * sy * cz - sx * sz);

    rotated_point.y = point.x * (-cy * sz)
                    + point.y * (cx * cz - sx * sy * sz)
                    + point.z * (sx * cz + cx * sy * sz);

    rotated_point.z = point.x * (sy)
                    + point.y * (-sx * cy)
                    + point.z * (cx * cy);

    return rotated_point;
}

sf::Vector3f rotatePoint3D(sf::Vector3f point, sf::Vector3f rotation)
{
    sf::Vector3f rotated_point;
    float cx = cos(rotation.x), sx = sin(rotation.x);
    float cy = cos(rotation.y), sy = sin(rotation.y);
    float cz = cos(rotation.z), sz = sin(rotation.z);

    rotated_point.x = point.x * (cy * cz)
                    + point.y * (cy * sz)
                    + point.z * (-sy);

    rotated_point.y = point.x * (sx * sy * cz - cx * sz)
                    + point.y * (sx * sy * sz + cx * cz)
                    + point.z * (sx * cy);

    rotated_point.z = point.x * (cx * sy * cz + sx * sz)
                    + point.y * (cx * sy * sz - sx * cz)
                    + point.z * (cx * cy);
    
    return rotated_point;
}


sf::Vector2f projectPoint(sf::Vector3f point, const Camera* camera, const sf::Vector2u& window_size)
{   float fov_rad = camera->fov_angle * M_PI / 180.0f;
    float tan_fov = tan(fov_rad / 2);
    float aspect_ratio = static_cast<float>(window_size.x) / window_size.y;

    sf::Vector2f projection;
    projection.x = (point.x / (point.z * tan_fov * aspect_ratio));
    projection.y = (point.y / (point.z * tan_fov));
    
    projection.x = (projection.x + 1.0f) / 2 * window_size.x;
    projection.y = (projection.y + 1.0f) / 2 * window_size.y;

    return projection;
}