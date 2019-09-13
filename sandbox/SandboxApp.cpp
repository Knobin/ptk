#include "ptk/ptk.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Sandbox", SCR_WIDTH, SCR_HEIGHT);
    window.setBackground(pTK::Color(0x232323FF));
    
    pTK::Ref<pTK::Checkbox> checkbox = pTK::create<pTK::Checkbox>();
    checkbox->setName("Checkbox Name");
    checkbox->setSize(pTK::Size(30, 30));
    checkbox->setMargin(pTK::Margin::Auto);
    checkbox->setCornerRadius(3); // 10% of size
    checkbox->setOutlineThickness(1.5f); // 5% of size
    checkbox->setColor(pTK::Color(0x00FF00FF));
    checkbox->setOutlineColor(pTK::Color(0xFF00FFFF));
    checkbox->set(true);
    window.add(checkbox);
    
    return app.exec(&window);
}
