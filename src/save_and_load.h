#ifndef SAVE_AND_LOAD_H
#define SAVE_AND_LOAD_H

#include <string>
#include <vector>
#include "shape.h"
#include "camera.h"

class SaveAndLoad {
public:
    SaveAndLoad();

    void saveScene(const std::string& filename, const std::vector<Shape*>& shapes) const;
    void loadScene(const std::string& filename, std::vector<Shape*>& shapes, Camera& camera) const;
    void loadObjShape(const std::string& filename, std::vector<Shape*>& shapes, Camera& camera) const;

private:
    
};

#endif // SAVE_AND_LOAD_H