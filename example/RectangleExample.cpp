#include "ptk/Application.hpp"
#include "ptk/widgets/Rectangle.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Rect Example", SCR_WIDTH, SCR_HEIGHT);
    
    pTK::Ref<pTK::Rectangle> rect = pTK::create<pTK::Rectangle>();
    rect->setName("My Rectangle");
    rect->setSize(pTK::Size(250, 100));
    rect->setPosHint(pTK::Point(250, 250));
    rect->setCornerRadius(10);
    rect->setColor(pTK::Color(0x161B21FF));
    rect->setOutlineThickness(2);
    rect->setOutlineColor(pTK::Color(0xF4A950FF));
    window.add(rect);
    rect->onClick([&rect](pTK::MouseButton, const pTK::Point&){
        std::cout << rect->getName() << " clicked!\n";
        return true;
    });
    
    return app.exec(&window);
}
