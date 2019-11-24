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
    std::cout << "Limits: " << pTK::Size::Limits::Min << ", " << pTK::Size::Limits::Max << std::endl;
    pTK::Ref<pTK::Checkbox> checkbox = pTK::create<pTK::Checkbox>();
    checkbox->setName("Checkbox Name");
    checkbox->setSize(pTK::Size(30, 30));
    checkbox->setAlign(pTK::Align::Top | pTK::Align::HCenter);
    checkbox->setCornerRadius(3); // 10% of size
    checkbox->setOutlineThickness(1.5f); // 5% of size
    checkbox->setColor(pTK::Color(0x00FF00FF));
    checkbox->setOutlineColor(pTK::Color(0xFF00FFFF));
    checkbox->set(true);
    window.add(checkbox);
    
    pTK::Ref<pTK::Label> label = pTK::create<pTK::Label>();
    label->setText("Toggled: 0 times!");
    label->setFontFamily("PingFang HK");
    label->setFontSize(32);
    label->setAlign(pTK::Align::Bottom | pTK::Align::HCenter);
    window.add(label);
    
    uint toggleCount = 0;
    checkbox->onToggle([&](bool){
        toggleCount++;
        if (toggleCount == 1)
            label->setText("Toggled: " + std::to_string(toggleCount) + " time!" );
        else
            label->setText("Toggled: " + std::to_string(toggleCount) + " times!" );
        
        return true;
    });
    
    return app.exec(&window);
}
