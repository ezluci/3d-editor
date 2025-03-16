#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <format>
#include <string>
#include "ui/button.h"
#include "ui/text_box.h"
#include "camera.h"
#include "shape.h"
#include "clock.h"
#include "save_and_load.h"
#include "shapes/cube.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "save_and_load.h"
#include "shapes/pyramid.h"
#include "shapes/sphere.h"
#include "points_math.h"

std::string formatFloat(float value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

int main()
{   
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "3D Editor", sf::Style::Fullscreen);
    
    const int width = window.getSize().x;
    const int height = window.getSize().y;
    const int font_size = height / 44;
    const int button_count = 24;
    const int menu_width = height / 2.7f;
    const int button_start_x = width - menu_width + menu_width / 100.0f;
    const int button_end_x = width - menu_width / 100.0f;
    const int button_height = height / button_count;
    const int button_width = button_end_x - button_start_x;
    const int renameMenu_width = width / 4.0f;
    const int renameMenu_height = height / 15.0f;
    const float rotationByKeys_speed = 1.5f;
    
    SaveAndLoad saveAndLoad;
    Camera camera({0, 0, 0}, 80, window);

    sf::Font font;  /// Create font
    if (!font.loadFromFile("fonts/Arial.ttf"))
    {
        return 1;
    }

    std::vector<Shape*> shapes;

    // create TESTING SHAPES. these should be REMOVED on the final build (or leave just the cube).

    int okdraw=0;
    int okselect=0;
    sf::Vector3f* selectedPoint=nullptr;
    Shape* shapeSelected = nullptr;

    sf::RectangleShape menuBackground(sf::Vector2f(menu_width, height));
    menuBackground.setFillColor(sf::Color(128, 128, 128));
    menuBackground.setPosition(width - menu_width, 0);

    // rename menu
    bool renameMenu_isOpen = false;
    Button applyRenameButton(
        width / 2.0 - renameMenu_width / 2.0 + 4 * (renameMenu_width / 5.0),
        height / 2.0 - renameMenu_height / 2.0,
        renameMenu_width / 5.0, renameMenu_height, "Rename", font, ((float) width / renameMenu_width) * 5
    );
    TextBox typeRenameText(
        width / 2.0 - renameMenu_width / 2.0,
        height / 2.0 - renameMenu_height / 2.0,
        4 * (renameMenu_width / 5.0), renameMenu_height, font, ((float) width / renameMenu_width) * 5
    );
    applyRenameButton.setOnPress([&typeRenameText, &shapeSelected, &renameMenu_isOpen]() {
        renameMenu_isOpen = false;
        if (shapeSelected) {
            shapeSelected->setShapeName(typeRenameText.getText());
        }
    });

    // change by keys buttons
    bool okposbykeys = false, okrotbykeys = false;
    Button positionByKeysButton(button_start_x + button_width / 2, button_height * 0, button_width / 2, button_height, "Move by keys", font, font_size);
    Button rotationByKeysButton(button_start_x + button_width / 2, button_height * 3, button_width / 2, button_height, "Rotate by keys", font, font_size);
    positionByKeysButton.setFillColor(sf::Color::Red);
    rotationByKeysButton.setFillColor(sf::Color::Red);

    positionByKeysButton.setOnPress([&shapeSelected, &positionByKeysButton, &rotationByKeysButton, &okposbykeys, &okrotbykeys]() {
        okposbykeys = !okposbykeys;
        okrotbykeys = false;
        positionByKeysButton.setFillColor((okposbykeys ? sf::Color::Green : sf::Color::Red));
        rotationByKeysButton.setFillColor(sf::Color::Red);
    });
    rotationByKeysButton.setOnPress([&shapeSelected, &positionByKeysButton, &rotationByKeysButton, &okposbykeys, &okrotbykeys]() {
        okposbykeys = false;
        okrotbykeys = !okrotbykeys;
        positionByKeysButton.setFillColor(sf::Color::Red);
        rotationByKeysButton.setFillColor((okrotbykeys ? sf::Color::Green : sf::Color::Red));
    });


    sf::Text positionText;
    positionText.setFont(font);
    positionText.setString("Shape position");
    positionText.setCharacterSize(font_size);
    positionText.setFillColor(sf::Color::White);
    positionText.setPosition(button_start_x, 0);

    sf::Text rotationText;
    rotationText.setFont(font);
    rotationText.setString("Shape rotation");
    rotationText.setCharacterSize(font_size);
    rotationText.setFillColor(sf::Color::White);
    rotationText.setPosition(button_start_x, button_height * 3);

    sf::Text scaleText;
    scaleText.setFont(font);
    scaleText.setString("Shape size");
    scaleText.setCharacterSize(font_size);
    scaleText.setFillColor(sf::Color::White);
    scaleText.setPosition(button_start_x, button_height * 6);

    sf::Text shapeText;
    shapeText.setFont(font);
    shapeText.setString("Select shape:");
    shapeText.setCharacterSize(font_size);
    shapeText.setFillColor(sf::Color::White);
    shapeText.setPosition(button_start_x, button_height * 9);

    
    sf::Text pointText;
    pointText.setFont(font);
    pointText.setString("Point position");
    pointText.setCharacterSize(font_size);
    pointText.setFillColor(sf::Color::White);
    pointText.setPosition(button_start_x, button_height * 18);
    //
    //
    sf::Text helpText;
    helpText.setFont(font);
    helpText.setCharacterSize(font_size);
    helpText.setFillColor(sf::Color::White);
    helpText.setPosition(10, 10); // Poziționăm textul în partea stângă sus a ecranului
    helpText.setString(
        "INSTRUCTIONS FOR EDITOR:\n"
        "\n"
        "CREATE THE SHAPE AND EDIT IT \n"
        "1. For creating a shape, you need to go to 'Select shape:', then left click on 'New shape...' and choose which shape to create. \n"
        "2. To change the shape size you go to 'Shape size', left click on the text box and type the size you would prefer. \n"
        "3. To move the shape, you have to select it after creating it. You need to left click the text box bellow 'Select shape' to select the shape.\n"
        "   The shape should be blue now. At 'Shape position' you can see 3 text boxs: x (left,right), y (up,down), z (front,back). \n"
        "   Alternatively, you can press the 'Change by keys' button in order to change the position of the shape via keyboard buttons:\n"
        "   A-D (X axis), W-S (Y axis), Q-E (Z axis)\n"
        "4. To change the rotation of the shape, you have to select it after creating it. You need to left click the text box bellow 'Select shape'\n"
        "   to select the shape. The shape should be blue now. It is rotating around the x-axis, y-axis and z-axis.\n"
        "   Alternatively, you can press the 'Change by keys' button in order to change the rotation of the shape via keyboard buttons:\n"
        "   W-S (X axis), A-D (Y axis), Q-E (Z axis)\n"
        "5. To change the name of the shape or to delete it, right click on the shape from the 'Select shape:' menu, then press\n"
        "   either 'Rename' or 'Delete'.\n"
        "\n"
        "DRAW AND EDIT POINTS \n"
        "6. To draw points and lines you need to have a shape selected. The 'Draw' button needs to be green, then you can draw with left click a \n"
        "point on screen, and if you left click a point, then another one, a line will create. If you want to delete a point, you right click on it. \n"
        "7. To select a point, the 'Select' button must be green and left click on it (it should be red now). Bellow 'Point position' you have the x, y, z. \n"
        "8. The 'Undo' button will delete the points and lines previously created. Press left click on it to undo. \n"
        "9. The 'Redo' button will recreate the points and lines you previously deleted. Press left click on it to redo. \n"
        "\n"
        "SAVE/LOAD \n"
        "10. If you want to save, then left click on 'Save' button. \n"
        "11. If you want to load, then left click on 'Load' button (it goes to your last save work). \n"
        "\n"
        "CLOSING THE HELP/EDITOR \n"
        "12. The 'Close 3D Editor' button closes the project. Press left click on it to close the project. \n"
        "13. To close the help section, press left click on 'Help' button. \n"
    );
    sf::RectangleShape helpBackground(sf::Vector2f(width - menu_width, height));
    helpBackground.setFillColor(sf::Color::Black);
    helpBackground.setPosition(0, 0);

    //
    //
    TextBox textBox_posX(button_start_x + 0 * button_width / 3.0f, button_height * 1, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_posY(button_start_x + 1 * button_width / 3.0f, button_height * 1, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_posZ(button_start_x + 2 * button_width / 3.0f, button_height * 1, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_rotX(button_start_x + 0 * button_width / 3.0f, button_height * 4, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_rotY(button_start_x + 1 * button_width / 3.0f, button_height * 4, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_rotZ(button_start_x + 2 * button_width / 3.0f, button_height * 4, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_size(button_start_x + button_width / 2, button_height * 6, button_width / 2, button_height, font, font_size);
    TextBox textBox_pointX(button_start_x + 0 * button_width / 3.0f, button_height * 19, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_pointY(button_start_x + 1 * button_width / 3.0f, button_height * 19, button_width / 3.0f, button_height, font, font_size);
    TextBox textBox_pointZ(button_start_x + 2 * button_width / 3.0f, button_height * 19, button_width / 3.0f, button_height, font, font_size);

    bool newShapeMenu_isOpen = false;
    Button newShapeButton(button_start_x + button_width / 2, button_height * 9, button_width / 2, button_height, "New shape...", font, font_size);
    Button undoButton(button_start_x + 0 * button_width / 5, button_height * (button_count - 2), button_width / 5, button_height, "Undo", font, font_size);
    Button redoButton(button_start_x + 1 * button_width / 5, button_height * (button_count - 2), button_width / 5, button_height, "Redo", font, font_size);
    Button helpButton(button_start_x + 2 * button_width / 5, button_height * (button_count - 2), button_width / 5, button_height, "Help", font, font_size);
    Button saveButton(button_start_x + 3 * button_width / 5, button_height * (button_count - 2), button_width / 5, button_height, "Save", font, font_size);
    Button loadButton(button_start_x + 4 * button_width / 5, button_height * (button_count - 2), button_width / 5, button_height, "Load", font, font_size);
    Button closeButton(button_start_x, button_height * (button_count - 1), button_width, button_height, "Close 3D Editor", font, font_size);

    Button loadObjButton(button_start_x + 3 * button_width / 5, button_height * (button_count - 3), 2 * button_width / 5, button_height, "Load from obj", font, font_size);

    Button drawButton(button_start_x + 3 * button_width / 5, button_height * (button_count - 6.05), button_width / 5, button_height, "Draw", font, font_size);
    Button selectButton(button_start_x + 4 * button_width / 5, button_height * (button_count - 6.05), button_width / 5, button_height, "Select", font, font_size);
    // these are the buttons for the "new shape menu"

    std::vector<Button> newShapeMenu_buttons(5);
    float yc = button_height * 10;

    for (size_t i = 0; i < 5; ++i)
    {
        newShapeMenu_buttons[i] = Button(button_start_x + button_width / 2, yc, button_width / 2, button_height, "Shape", font, font_size);
        yc += button_height;
    }
    newShapeMenu_buttons[0].getSfText()->setString("Cube");
    newShapeMenu_buttons[0].setOnPress([&shapes, &camera, &newShapeMenu_isOpen]() {
        shapes.push_back(new Cube(&camera, sf::Vector3f{0, 0, 10}, 1));
        newShapeMenu_isOpen = !newShapeMenu_isOpen;
    });
    newShapeMenu_buttons[1].getSfText()->setString("Pyramid");
    newShapeMenu_buttons[1].setOnPress([&shapes, &camera, &newShapeMenu_isOpen]() {
        shapes.push_back(new Pyramid(&camera, sf::Vector3f{0, 0, 10}, 14, 4));
        newShapeMenu_isOpen = !newShapeMenu_isOpen;
    });
    newShapeMenu_buttons[2].getSfText()->setString("Sphere");
    newShapeMenu_buttons[2].setOnPress([&shapes, &camera, &newShapeMenu_isOpen]() {
        shapes.push_back(new Sphere(&camera, sf::Vector3f{0, 0, 10}, 1.5));
        newShapeMenu_isOpen = !newShapeMenu_isOpen;
    });
    newShapeMenu_buttons[3].getSfText()->setString("Cone");
    newShapeMenu_buttons[3].setOnPress([&shapes, &camera, &newShapeMenu_isOpen]() {
        shapes.push_back(new Cone(&camera, sf::Vector3f{0, 0, 10}, 3,1));
        newShapeMenu_isOpen = !newShapeMenu_isOpen;
    });
    newShapeMenu_buttons[4].getSfText()->setString("Cylinder");
    newShapeMenu_buttons[4].setOnPress([&shapes, &camera, &newShapeMenu_isOpen]() {
        shapes.push_back(new Cylinder(&camera, sf::Vector3f{0, 0, 10}, 2,1));
        newShapeMenu_isOpen = !newShapeMenu_isOpen;
    });

    // this is the context menu when right-clicking a shape
    bool contextMenu_isOpen = false;
    Shape *contextMenu_shape = nullptr;
    sf::Vector2i contextMenu_origin = sf::Vector2i(0, 0);
    std::vector<Button> contextMenu_buttons{
        Button(0, 0, button_width / 3.0f, button_height, "Rename", font, font_size),
        Button(0, 0, button_width / 3.0f, button_height, "Delete", font, font_size)
    };
    contextMenu_buttons[0].setOnPress([&renameMenu_isOpen, &typeRenameText, &shapeSelected]() {
        renameMenu_isOpen = true;
        typeRenameText.setSelected(true);
        typeRenameText.setText(shapeSelected->getShapeName());
    });
    contextMenu_buttons[1].setOnPress([&shapeSelected, &shapes]() {
        shapes.erase(find(shapes.begin(), shapes.end(), shapeSelected));
    });
    newShapeButton.setOnPress([&newShapeMenu_isOpen, &newShapeButton]() {
        newShapeMenu_isOpen = !newShapeMenu_isOpen;
    });
    undoButton.setOnPress([&shapeSelected]() {
        if (shapeSelected) {
            shapeSelected->undo();
        }
    });
    redoButton.setOnPress([&shapeSelected]() {
        if (shapeSelected) {
            shapeSelected->redo();
        }
    });
    bool showHelp = false;
    helpButton.setOnPress([&showHelp](){
        showHelp = ! showHelp;
    });

    drawButton.setFillColor(sf::Color::Red);
    drawButton.setOnPress([&okdraw,&drawButton,&okselect,&selectButton](){
        okdraw = !okdraw;
        okselect = 0;
        drawButton.setFillColor((okdraw ? sf::Color::Green : sf::Color::Red));
        selectButton.setFillColor(sf::Color::Red);
    });
    selectButton.setFillColor(sf::Color::Red);
    selectButton.setOnPress([&okselect,&selectButton,&okdraw,&drawButton](){
        okdraw = 0;
        okselect = !okselect;
        drawButton.setFillColor(sf::Color::Red);
        selectButton.setFillColor((okselect ? sf::Color::Green : sf::Color::Red));
    });

    saveButton.setOnPress([&shapeSelected, &shapes, saveAndLoad]() {
        saveAndLoad.saveScene("savefile.txt", shapes);
        shapeSelected = nullptr;
    });
    loadButton.setOnPress([&shapeSelected, &shapes, saveAndLoad, &camera]() {
        saveAndLoad.loadScene("savefile.txt", shapes, camera);
        shapeSelected = nullptr;
    });
    closeButton.setOnPress([&window]() {
        window.close();
    });
    loadObjButton.setOnPress([&shapeSelected, &shapes, saveAndLoad, &camera]() {
        saveAndLoad.loadObjShape("shape.obj", shapes, camera);
        shapeSelected = nullptr;
    });

    Clock clock;

    while (window.isOpen())
    {
    start_mainloop:
        /// Calculate delta time
        clock.updateclock();
        float deltatime = clock.getdelta();

        /// Updates
        float yc = button_height * 10;
        for (Shape *shape : shapes)
        {
            if (shape->getShapeButton() == nullptr)
            {
                shape->setShapeButton(new Button(0, 0, button_width, button_height, shape->getShapeName(), font, font_size));
                shape->getShapeButton()->setOnPress(
                    [&shapeSelected, shape, &textBox_posX, &textBox_posY, &textBox_posZ, &textBox_rotX,
                    &textBox_rotY, &textBox_rotZ, &textBox_size]() {
                        shapeSelected = shape;
                        textBox_posX.setText(formatFloat(shapeSelected->getPosition().x));
                        textBox_posY.setText(formatFloat(shapeSelected->getPosition().y));
                        textBox_posZ.setText(formatFloat(shapeSelected->getPosition().z));
                        textBox_rotX.setText(formatFloat(shapeSelected->getRotation().x));
                        textBox_rotY.setText(formatFloat(shapeSelected->getRotation().y));
                        textBox_rotZ.setText(formatFloat(shapeSelected->getRotation().z));
                        textBox_size.setText(formatFloat(shapeSelected->getSize()));
                    }
                );
                shape->getShapeButton()->setOnPressRight(
                    [&shapeSelected, shape, &textBox_posX, &textBox_posY, &textBox_posZ, &textBox_rotX,
                    &textBox_rotY, &textBox_rotZ, &textBox_size, &contextMenu_isOpen, &contextMenu_shape,
                    &contextMenu_origin, &window]() {
                        shapeSelected = shape;
                        textBox_posX.setText(formatFloat(shapeSelected->getPosition().x));
                        textBox_posY.setText(formatFloat(shapeSelected->getPosition().y));
                        textBox_posZ.setText(formatFloat(shapeSelected->getPosition().z));
                        textBox_rotX.setText(formatFloat(shapeSelected->getRotation().x));
                        textBox_rotY.setText(formatFloat(shapeSelected->getRotation().y));
                        textBox_rotZ.setText(formatFloat(shapeSelected->getRotation().z));
                        textBox_size.setText(formatFloat(shapeSelected->getSize()));
                        contextMenu_isOpen = true;
                        contextMenu_shape = shape;
                        contextMenu_origin = sf::Mouse::getPosition(window);
                    }
                );
            }
            shape->getShapeButton()->getSfText()->setString(shape->getShapeName());
            shape->getShapeButton()->setPosition(sf::Vector2f{button_start_x, yc});
            yc += button_height;
        }

        if (okposbykeys && shapeSelected) {
            auto oldPosition = shapeSelected->getPosition();
            auto newPosition = oldPosition;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                newPosition.x -= oldPosition.z * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                newPosition.x += oldPosition.z * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                newPosition.y -= oldPosition.z * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                newPosition.y += oldPosition.z * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                newPosition.z -= oldPosition.z * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                newPosition.z += oldPosition.z * deltatime;

            if (oldPosition != newPosition) {
                shapeSelected->setPosition(newPosition);
                textBox_posX.setText(formatFloat(shapeSelected->getPosition().x));
                textBox_posY.setText(formatFloat(shapeSelected->getPosition().y));
                textBox_posZ.setText(formatFloat(shapeSelected->getPosition().z));
            }
        }
        if (okrotbykeys && shapeSelected) {
            auto oldRotation = shapeSelected->getRotation();
            auto newRotation = oldRotation;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                newRotation.y -= rotationByKeys_speed * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                newRotation.y += rotationByKeys_speed * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                newRotation.x += rotationByKeys_speed * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                newRotation.x -= rotationByKeys_speed * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                newRotation.z -= rotationByKeys_speed * deltatime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                newRotation.z += rotationByKeys_speed * deltatime;

            if (oldRotation != newRotation) {
                shapeSelected->setRotation(newRotation);
                textBox_rotX.setText(formatFloat(shapeSelected->getRotation().x));
                textBox_rotY.setText(formatFloat(shapeSelected->getRotation().y));
                textBox_rotZ.setText(formatFloat(shapeSelected->getRotation().z));
            }
        }


        /// Process events
        sf::Event event;
        while (window.pollEvent(event)) 
        {  
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) 
            {
                sf::Vector2f positionMouse(event.mouseButton.x, event.mouseButton.y);

                if (positionMouse.x < width - menu_width)
                    if (shapeSelected)
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                         if (okselect==1 && okdraw==0)  
                         {selectedPoint = shapeSelected->selectPoint(positionMouse,10.0f, window);
                            if (selectedPoint) {
                    textBox_pointX.setText(formatFloat(selectedPoint->x));
                    textBox_pointY.setText(formatFloat(selectedPoint->y));
                    textBox_pointZ.setText(formatFloat(selectedPoint->z));
                }
                         }
                    if (okdraw==1 && okselect==0) shapeSelected->addPoint(positionMouse, shapeSelected->getPosition().z, window);
                        if (event.mouseButton.button == sf::Mouse::Right)
                            shapeSelected->deletePoint(positionMouse, window);
                    }
            }
            helpButton.checkPress(event);
            selectButton.checkPress(event);
            drawButton.checkPress(event);
            newShapeButton.checkPress(event);
            undoButton.checkPress(event);
            redoButton.checkPress(event);
            closeButton.checkPress(event);
            saveButton.checkPress(event);
            if (loadButton.checkPress(event)) {
                goto start_mainloop;
            }
            if (loadObjButton.checkPress(event)) {
                goto start_mainloop;
            }

            if (!newShapeMenu_isOpen && !contextMenu_isOpen && !renameMenu_isOpen) {
                for (Shape *shape : shapes)
                    shape->getShapeButton()->checkPress(event);
            }

            if (newShapeMenu_isOpen) {
                for (Button newShapeButton : newShapeMenu_buttons) {
                    if (newShapeButton.checkPress(event)) {
                        goto start_mainloop;
                    }
                }
            }

            if (renameMenu_isOpen) {
                applyRenameButton.checkPress(event);
                typeRenameText.handleEvent(event);
            }

            if (contextMenu_isOpen) {
                for (Button &button : contextMenu_buttons)
                    button.checkPress(event);
            }
            
            textBox_posX.handleEvent(event);
            textBox_posY.handleEvent(event);
            textBox_posZ.handleEvent(event);
            positionByKeysButton.checkPress(event);

            textBox_rotX.handleEvent(event);
            textBox_rotY.handleEvent(event);
            textBox_rotZ.handleEvent(event);
            rotationByKeysButton.checkPress(event);

            textBox_pointX.handleEvent(event);
            textBox_pointY.handleEvent(event);
            textBox_pointZ.handleEvent(event);

            textBox_size.handleEvent(event);
            if (selectedPoint)
            {selectedPoint->x = textBox_pointX.getTextf();
             selectedPoint->y = textBox_pointY.getTextf();
             selectedPoint->z = textBox_pointZ.getTextf();
            }
            if (shapeSelected) {
                shapeSelected->setPosition(sf::Vector3f{textBox_posX.getTextf(), textBox_posY.getTextf(), textBox_posZ.getTextf()});
                shapeSelected->setRotation(sf::Vector3f{textBox_rotX.getTextf(), textBox_rotY.getTextf(), textBox_rotZ.getTextf()});
                shapeSelected->setSize(textBox_size.getTextf());
            }

            if (event.type == sf::Event::MouseButtonPressed)
                if (event.mouseButton.button == sf::Mouse::Left)
                    contextMenu_isOpen = false;
        }

        
        /// Drawing
        window.clear();
        for (Shape *shape : shapes)
            if (shapeSelected == shape)
                shape->draw(sf::Color::Blue, 4);
            else
                shape->draw();

        // Draw menus
        window.draw(menuBackground);
        window.draw(positionText);
        positionByKeysButton.draw(window);
        textBox_posX.draw(window);
        textBox_posY.draw(window);
        textBox_posZ.draw(window);
        window.draw(rotationText);
        rotationByKeysButton.draw(window);
        textBox_rotX.draw(window);
        textBox_rotY.draw(window);
        textBox_rotZ.draw(window);
        window.draw(scaleText);
        textBox_size.draw(window);
        window.draw(pointText);
        textBox_pointX.draw(window);
        textBox_pointY.draw(window);
        textBox_pointZ.draw(window);

        window.draw(shapeText);
        if (showHelp){
            window.draw(helpBackground);
            window.draw(helpText);
        }

        loadObjButton.draw(window);
        helpButton.draw(window);
        selectButton.draw(window);
        drawButton.draw(window);
        newShapeButton.draw(window);
        saveButton.draw(window);
        loadButton.draw(window);
        undoButton.draw(window);
        redoButton.draw(window);
        closeButton.draw(window);

        for (Shape *shape : shapes)
            shape->getShapeButton()->draw(window);
        
        if (newShapeMenu_isOpen)
        {
            for (Button &button : newShapeMenu_buttons) {
                button.draw(window);
            }
        }

        if (renameMenu_isOpen) {
            applyRenameButton.draw(window);
            typeRenameText.draw(window);
        }

        if (contextMenu_isOpen)
        {
            float yc = 0;
            for (Button &button : contextMenu_buttons) {
                button.setPosition(sf::Vector2f(contextMenu_origin) + sf::Vector2f(0, yc));
                button.draw(window);
                yc += button_height;
            }
        }

        window.display();
    }

    return 0;
}
