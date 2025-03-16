#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/Graphics.hpp>
#include <utility>
#include "camera.h"
#include "ui/button.h"
#include <stack>
#include <iostream>
#include <fstream>

class Shape {
protected:
    sf::Vector3f position;
    sf::Vector3f rotation;
    float size;
    
    Button *shapeButton;
    Camera *camera;
    sf::Vector3f *selectedPoint;
    std::string shapeName;
    std::vector<sf::Vector3f> vertices;
    std::vector<std::pair<size_t, size_t>> edges;
public:
    
    std::stack<std::pair<std::vector<sf::Vector3f>, std::vector<std::pair<size_t, size_t>>>> undoStack; //vertices+lines
    std::stack<std::pair<std::vector<sf::Vector3f>, std::vector<std::pair<size_t, size_t>>>> redoStack;


    // constructors
    Shape();
    Shape(Camera *camera);
    Shape(Camera *camera, std::vector<sf::Vector3f> vertices, std::vector<std::pair<size_t, size_t>> edges);
    Shape(Camera *camera, sf::Vector3f position, std::vector<sf::Vector3f> vertices, std::vector<std::pair<size_t, size_t>> edges);
    Shape(Camera *camera, sf::Vector3f position, float size, std::vector<sf::Vector3f> vertices, std::vector<std::pair<size_t, size_t>> edges);

    // methods
    void draw(sf::Color color = sf::Color::White, float thickness = 0) const;
    
    sf::Vector3f getPosition() const;
    void setPosition(sf::Vector3f position);
    
    sf::Vector3f getRotation() const;
    void setRotation(sf::Vector3f rotation);
    
    float getSize() const;
    void setSize(float size);
    
    sf::Vector3f get3DPositionOfVertex(size_t index) const;

    //std::vector<sf::Vector3f> getRotatedVertices() const;

    Button* getShapeButton() const;
    void setShapeButton(Button* button);

    std::string getShapeName() const;
    void setShapeName(std::string);

    std::pair<sf::Vector3f, sf::Vector3f> getSelectedXAxis() const;
    std::pair<sf::Vector3f, sf::Vector3f> getSelectedYAxis() const;
    std::pair<sf::Vector3f, sf::Vector3f> getSelectedZAxis() const;

    void addPoint(const sf::Vector2f& newPoint, float depth, const sf::RenderWindow& window); 
    void deletePoint(const sf::Vector2f& deletedPoint, const sf::RenderWindow& window);
    void undo();
    void redo();
    
    std::vector<sf::Vector3f> getVertices() const;
    std::vector<std::pair<size_t, size_t>> getEdges() const;
    void setVertices(const std::vector<sf::Vector3f>& vertices);
    void setEdges(const std::vector<std::pair<size_t, size_t>>& edges);
    //point
    sf::Vector3f* selectPoint(const sf::Vector2f& newPoint, float depth, const sf::RenderWindow& window);

};

#endif