#include "ptk/application.hpp"
#include "ptk/widgets/rectangle.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Rect Example", SCR_WIDTH, SCR_HEIGHT);
    
    std::shared_ptr<pTK::Rectangle> rect = std::make_shared<pTK::Rectangle>();
    rect->setSize(250, 250);
    rect->setPosition(250, 250);
    rect->setCornerRadius(25);
    rect->setColor(pTK::Color(25, 25, 25));
    rect->setOutlineThickness(2);
    rect->setOutlineColor(pTK::Color(225, 225, 225, 100));
    window.add(rect);
    
    rect->onClick([](pTK::MouseButton, const pTK::Vec2<int>&){
        std::cout << "Rectangle Clicked!\n";
    });
    
    return app.exec(&window);
}
