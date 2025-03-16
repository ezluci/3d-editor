
#include "shape.h"
#include "camera.h"
#include "points_math.h"
#include <iostream>
#include <cmath>
#include <fstream>

Shape::Shape() { }

Shape::Shape(Camera *camera)
    : camera(camera), shapeButton(nullptr), selectedPoint(nullptr), shapeName("Shape")
{
    position = rotation = sf::Vector3f{0, 0, 0};
    size = 0;
}

Shape::Shape(Camera *camera, std::vector<sf::Vector3f> vertices, std::vector<std::pair<size_t, size_t>> edges)
    : camera(camera), vertices(vertices), edges(edges), shapeButton(nullptr), selectedPoint(nullptr), shapeName("Shape")
{
    position = rotation = sf::Vector3f{0, 0, 0};
    size = 0;
}

Shape::Shape(Camera *camera, sf::Vector3f position, std::vector<sf::Vector3f> vertices, std::vector<std::pair<size_t, size_t>> edges)
    : camera(camera), position(position), vertices(vertices), edges(edges), shapeButton(nullptr), selectedPoint(nullptr), shapeName("Shape")
{
    rotation = sf::Vector3f{0, 0, 0};
    size = 0;
}

Shape::Shape(Camera *camera, sf::Vector3f position, float size, std::vector<sf::Vector3f> vertices, std::vector<std::pair<size_t, size_t>> edges)
    : camera(camera), position(position), size(size), vertices(vertices), edges(edges), shapeButton(nullptr), selectedPoint(nullptr), shapeName("Shape")
{
    rotation = sf::Vector3f{0, 0, 0};
}

void Shape::draw(sf::Color color, float thickness) const
{
    const sf::Vector2u window_size = this->camera->window.getSize();
    
    for (auto [i, j] : this->edges)
    {
        sf::Vector2f point1 = projectPoint(this->get3DPositionOfVertex(i), this->camera, window_size);
        sf::Vector2f point2 = projectPoint(this->get3DPositionOfVertex(j), this->camera, window_size);

        if (thickness == 0)
        {
            sf::Vertex line[] = {
                sf::Vertex(point1, color),
                sf::Vertex(point2, color)
            };
            this->camera->window.draw(line, 2, sf::Lines);
        }
        else
        {
            sf::Vector2f direction = point2 - point1;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            sf::RectangleShape line(sf::Vector2f(length, thickness));
            line.setFillColor(color);

            line.setOrigin(sf::Vector2f{0, thickness / 2});
            line.setRotation(std::atan2(direction.y, direction.x) * 180 / M_PI);
            line.setPosition(point1);

            this->camera->window.draw(line);
        }

    }

    for (size_t i = 0; i < this->vertices.size(); ++i) {
        sf::Vector3f point3D = this->get3DPositionOfVertex(i);
        sf::Vector2f projected_point = projectPoint(point3D, this->camera, window_size);
        sf::CircleShape point(5); // reset this
        point.setFillColor(sf::Color::Green);
        point.setPosition(projected_point.x - point.getRadius(), projected_point.y - point.getRadius());
        if (&vertices[i] == selectedPoint) {
            point.setFillColor(sf::Color::Red);
        } else {
            point.setFillColor(sf::Color::Green);
        }
        this->camera->window.draw(point);
    }
}

sf::Vector3f Shape::getPosition() const
{
    return this->position;
}

void Shape::setPosition(sf::Vector3f position)
{   
    this->position = position;
}

sf::Vector3f Shape::getRotation() const
{
    return this->rotation;
}

void Shape::setRotation(sf::Vector3f rotation)
{
    this->rotation = rotation;
}

float Shape::getSize() const
{
    return this->size;
}

void Shape::setSize(float size)
{
    this->size = size;
}


sf::Vector3f Shape::get3DPositionOfVertex(size_t index) const
{
    sf::Vector3f point = this->vertices[index];
    point *= this->getSize();
    point = rotatePoint3D(point, this->rotation);
    point += this->position;
    return point;
}


// std::vector<sf::Vector3f> Shape::getRotatedVertices() const
// {
//     std::vector<sf::Vector3f> rotated_vertices;
//     for (const sf::Vector3f& vertex : this->vertices) {
//         rotated_vertices.push_back(rotatePoint3D(vertex, this->rotation));
//     }
//     return rotated_vertices;
// }

Button* Shape::getShapeButton() const
{
    return shapeButton;
}
void Shape::setShapeButton(Button* button)
{
    this->shapeButton = button;
}

std::string Shape::getShapeName() const
{
    return this->shapeName;
}
void Shape::setShapeName(std::string name)
{
    this->shapeName = name;
}

void Shape::undo() {
    if (!undoStack.empty()) {
        redoStack.push({vertices, edges});
  
        auto lastState = undoStack.top();
        undoStack.pop();

        vertices = lastState.first;
        edges = lastState.second;
    }
}

void Shape::redo() {
    if (!redoStack.empty()) {
        undoStack.push({vertices, edges});

        auto lastState = redoStack.top();
        redoStack.pop();

        vertices = lastState.first;
        edges = lastState.second;
    }
}

void Shape::addPoint(const sf::Vector2f& newPoint, float z, const sf::RenderWindow& window) {
    sf::Vector2u window_size = window.getSize();
    undoStack.push({vertices, edges});//curent state

    float aspect_ratio = static_cast<float>(window_size.x) / window_size.y;

    sf::Vector2f normalizedPoint;
    normalizedPoint.x = (2.0f * newPoint.x / window_size.x - 1.0f) * aspect_ratio;
    normalizedPoint.y = (2.0f * newPoint.y / window_size.y - 1.0f);

    float tan_fov = tan(camera->fov_angle * M_PI / 180.0f / 2.0f);

    sf::Vector3f newPoint3D;
    newPoint3D.x = normalizedPoint.x * z * tan_fov;
    newPoint3D.y = normalizedPoint.y * z * tan_fov;
    newPoint3D.z = z;

    newPoint3D -= this->position;
    newPoint3D = inverseRotatePoint3D(newPoint3D, -this->rotation);
    newPoint3D /= this->size;

    sf::Vector3f* newSelectedPoint = nullptr;
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        sf::Vector2f projected_point = projectPoint(this->get3DPositionOfVertex(i), this->camera, window_size);
        sf::CircleShape point(10);
        point.setPosition(projected_point.x - point.getRadius(), projected_point.y - point.getRadius());
        if (point.getGlobalBounds().contains(newPoint))
            newSelectedPoint = &this->vertices[i];
    }


    if (this->selectedPoint == nullptr && newSelectedPoint)
        this->selectedPoint = newSelectedPoint;

    if (this->selectedPoint && newSelectedPoint && this->selectedPoint != newSelectedPoint)
    {
        size_t selectedIndex = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), *selectedPoint));
        size_t newSelectedIndex = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), *newSelectedPoint));
        if (std::find(edges.begin(), edges.end(), std::make_pair(selectedIndex, newSelectedIndex)) == edges.end() &&
            std::find(edges.begin(), edges.end(), std::make_pair(newSelectedIndex, selectedIndex)) == edges.end()) {
            edges.emplace_back(selectedIndex, newSelectedIndex);
        }
        this->selectedPoint = nullptr;
    }

    if (!newSelectedPoint) {
        vertices.push_back(newPoint3D);
    }
}



void Shape::deletePoint(const sf::Vector2f& deletedPoint, const sf::RenderWindow& window) {
    undoStack.push({vertices, edges});

    sf::Vector2u window_size = window.getSize();

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        sf::Vector2f projected_point = projectPoint(this->get3DPositionOfVertex(i), this->camera, window_size);
        sf::CircleShape point(10);
        point.setPosition(projected_point.x - point.getRadius(), projected_point.y - point.getRadius());
        
        if (point.getGlobalBounds().contains(deletedPoint)) {
            edges.erase(
                std::remove_if(edges.begin(), edges.end(), [i](const std::pair<size_t, size_t>& edge) {
                    return edge.first == i || edge.second == i;
                }),
                edges.end()
            );

            for (auto& edge : edges) {
                if (edge.first > i) edge.first--;
                if (edge.second > i) edge.second--;
            }

            vertices.erase(vertices.begin() + i);

            return; 
        }
    }
}


std::pair<sf::Vector3f, sf::Vector3f> Shape::getSelectedXAxis() const
{
    return std::make_pair(
        position,
        position + rotatePoint3D(sf::Vector3f{1, 0, 0}, rotation)
    );
}

std::pair<sf::Vector3f, sf::Vector3f> Shape::getSelectedYAxis() const
{
    return std::make_pair(
        position,
        position + rotatePoint3D(sf::Vector3f{0, 1, 0}, rotation)
    );
}

std::pair<sf::Vector3f, sf::Vector3f> Shape::getSelectedZAxis() const
{
    return std::make_pair(
        position,
        position + rotatePoint3D(sf::Vector3f{0, 0, 1}, rotation)
    );
}

std::vector<sf::Vector3f> Shape::getVertices() const {
    return this->vertices;
}

std::vector<std::pair<size_t, size_t>> Shape::getEdges() const {
    return this->edges;
}

void Shape::setVertices(const std::vector<sf::Vector3f>& vertices) {
    this->vertices = vertices;
}

void Shape::setEdges(const std::vector<std::pair<size_t, size_t>>& edges) {
    this->edges = edges;
}
sf::Vector3f* Shape::selectPoint(const sf::Vector2f& newPoint, float depth, const sf::RenderWindow& window) {
    sf::Vector2u window_size = window.getSize();
    float aspect_ratio = static_cast<float>(window_size.x) / window_size.y;

    sf::Vector2f normalizedPoint;
    normalizedPoint.x = (2.0f * newPoint.x / window_size.x - 1.0f) * aspect_ratio;
    normalizedPoint.y = (2.0f * newPoint.y / window_size.y - 1.0f);

    float tan_fov = tan(camera->fov_angle * M_PI / 180.0f / 2.0f);

    sf::Vector3f newPoint3D;
    newPoint3D.x = normalizedPoint.x * depth * tan_fov;
    newPoint3D.y = normalizedPoint.y * depth * tan_fov;
    newPoint3D.z = depth;

    this->selectedPoint = nullptr;
    for (size_t i = 0; i < vertices.size(); ++i) {
        sf::Vector2f projected_point = projectPoint(this->get3DPositionOfVertex(i), this->camera, window_size);
        
        sf::CircleShape point(10);
        point.setPosition(projected_point.x - point.getRadius(), projected_point.y - point.getRadius());

        if (point.getGlobalBounds().contains(newPoint)) {
            this->selectedPoint = &this->vertices[i];
            return this->selectedPoint;
        }
    }

    return nullptr; 
}