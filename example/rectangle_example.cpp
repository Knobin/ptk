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
    rect->set_size(250, 250);
    rect->set_position(250, 250);
    rect->set_corner_radius(25);
    rect->set_color(pTK::Color(25, 25, 25));
    rect->set_outline_thickness(2);
    rect->set_outline_color(pTK::Color(225, 225, 225, 100));
    window.add(rect);
    
    rect->on_click([](pTK::MouseButton, const pTK::Vec2<int>&){
        std::cout << "Rectangle Clicked!\n";
    });
    
    return app.exec(&window);
}
