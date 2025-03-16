#include "save_and_load.h"
#include "camera.h"
#include <algorithm>

SaveAndLoad::SaveAndLoad()
{ }

/*Numărul de forme: Prima linie conține numărul total de forme salvate.
Formele: Pentru fiecare formă, sunt salvate următoarele informații:
Numărul de vârfuri: Prima linie pentru fiecare formă conține numărul de vârfuri.
Coordonatele vârfurilor: Următoarele linii conțin coordonatele vârfurilor (x, y, z).
Numărul de muchii: Linia următoare conține numărul de muchii.
Muchiile: Următoarele linii conțin perechi de indici care reprezintă muchiile (indicele primului vârf, indicele celui de-al doilea vârf).
*/

void SaveAndLoad::saveScene(const std::string& filename, const std::vector<Shape*>& shapes) const {
    std::ofstream file(filename);  // deschide fișierul pentru scriere

    if (!file.is_open()) {
        std::cout << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    file << shapes.size() << "\n";  // numarul de forme

    for (const auto& shape : shapes) {
        file << shape->getShapeName() << " " << shape->getVertices().size() << "\n";  // numele formei si nr de varfuri
        for (const auto& vertex : shape->getVertices()) {
            file << vertex.x << " " << vertex.y << " " << vertex.z << "\n";  // scrie coordonatele vârfurilor
        }

        file << shape->getEdges().size() << "\n";  // scrie numărul de muchii
        for (const auto& edge : shape->getEdges()) {
            file << edge.first << " " << edge.second << "\n";  // scrie muchiile
        }

        file << shape->getRotation().x << " " << shape->getRotation().y << " " << shape->getRotation().z << "\n";  // rotation
        file << shape->getPosition().x << " " << shape->getPosition().y << " " << shape->getPosition().z << "\n";  // position
        file << shape->getSize() << "\n";  // size
    }

    file.close();
    std::cout << "All data saved to " << filename << std::endl;
}

void SaveAndLoad::loadScene(const std::string& filename, std::vector<Shape*>& shapes, Camera& camera) const {
    std::ifstream file(filename);  // deschide fișierul pentru citire

    if (!file.is_open()) {
        std::cout << "Unable to open file for reading: " << filename << std::endl;
        return;
    }

    size_t shapeCount;
    file >> shapeCount;  // citește numărul de forme
    shapes.clear();
    shapes.reserve(shapeCount);

    for (size_t i = 0; i < shapeCount; ++i) {
        Shape* shape = new Shape(&camera);
        std::string shapeName;
        size_t vertexCount;
        file >> shapeName >> vertexCount;  // numele formei si nr de varfuri
        shape->setShapeName(shapeName);

        std::vector<sf::Vector3f> vertices(vertexCount);
        for (size_t j = 0; j < vertexCount; ++j) {
            file >> vertices[j].x >> vertices[j].y >> vertices[j].z;  // citește coordonatele vârfurilor
        }
        shape->setVertices(vertices);

        size_t edgeCount;
        file >> edgeCount;  // citește numărul de muchii
        std::vector<std::pair<size_t, size_t>> edges(edgeCount);
        for (size_t j = 0; j < edgeCount; ++j) {
            file >> edges[j].first >> edges[j].second;  // citește muchiile
        }
        shape->setEdges(edges);

        sf::Vector3f rotation;
        file >> rotation.x >> rotation.y >> rotation.z;  // rotation
        shape->setRotation(rotation);

        sf::Vector3f position;
        file >> position.x >> position.y >> position.z;  // position
        shape->setPosition(position);

        float size;
        file >> size;
        shape->setSize(size);   // size

        shapes.push_back(shape);
    }

    file.close();
    std::cout << "All data loaded from " << filename << std::endl;
}


void SaveAndLoad::loadObjShape(const std::string& filename, std::vector<Shape*>& shapes, Camera& camera) const
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Unable to open file for reading: " << filename << std::endl;
        return;
    }

    std::vector<sf::Vector3f> vertices;
    std::vector<std::pair<size_t, size_t>> edges;
    char type;
    while (file >> type)
    {
        if (type == 'v')
        {
            float x, y, z;  file >> x >> y >> z;
            vertices.push_back(sf::Vector3f(x, y, z));
        }
        else if (type == 'f')
        {
            int a, b, c;    file >> a >> b >> c;
            a--;    b--;    c--;
            edges.push_back(std::make_pair(a, b));
            edges.push_back(std::make_pair(a, c));
            edges.push_back(std::make_pair(b, c));
        }
    }

    for (size_t i = 0; i < edges.size(); ++i)
        if (edges[i].second > edges[i].first)   std::swap(edges[i].first, edges[i].second);
    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());

    shapes.push_back(new Shape(&camera));
    shapes.back()->setSize(1);
    shapes.back()->setVertices(vertices);
    shapes.back()->setEdges(edges);

    file.close();
    std::cout << "All data loaded from " << filename << std::endl;
}