#include "ptk/ptk.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    // Application and Window.
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Sandbox", SCR_WIDTH, SCR_HEIGHT);
    window.setBackground(pTK::Color(0x232323FF));

    // Set ESC key to close the window.
    window.onKey([&](pTK::Event::Type type, int32 key) {
        if ((type == pTK::Event::Type::KeyReleased) && (key == 256))
            window.close();
        return true;
    });

    // Checkbox.
    pTK::Ref<pTK::Checkbox> checkbox = pTK::create<pTK::Checkbox>();
    checkbox->setName("Checkbox Name");
    checkbox->setConstSize(pTK::Size(30, 30));
    //checkbox->setAlign(pTK::Align::Top | pTK::Align::HCenter);
    checkbox->setCornerRadius(3); // 10% of size
    checkbox->setOutlineThickness(1.5f); // 5% of size
    checkbox->setColor(pTK::Color(0x00FF00FF));
    checkbox->setOutlineColor(pTK::Color(0xFF00FFFF));
    checkbox->set(true);
    //checkbox->setMarginTop(-50);
    
    // Label.
    pTK::Ref<pTK::Label> label = pTK::create<pTK::Label>();
    label->setText("Toggle Checkbox");
    label->setFontFamily("Roboto");
    label->setFontSize(32);
    label->setMarginTopBottom(5, 5);
    //label->setAlign(pTK::Align::Right | pTK::Align::Bottom);

    // Button.
    pTK::Ref<pTK::Button> button = pTK::create<pTK::Button>(pTK::Button::Style::Danger);
    button->setText("Reset Label Button");
    button->setFontFamily("Roboto");
    button->setFontSize(14);
    button->setConstSize(button->getSize());
    // button->setAlign(pTK::Align::Top | pTK::Align::HCenter);
    
    // Change Label Text on Checkbox toggle.
    uint toggleCount = 0;
    checkbox->onToggle([&](bool){
        toggleCount++;
        if (toggleCount == 1)
        {
            label->setText("Toggled: " + std::to_string(toggleCount) + " time!" );
            window.add(button);
        } else
            label->setText("Toggled: " + std::to_string(toggleCount) + " times!" );
        
        return true;
    });

    // Reset Label Text on Button click.
    button->onClick([&](pTK::Mouse::Button, const pTK::Point&) {
        toggleCount = 0;
        label->setText("Toggle Checkbox");
        window.remove(button);

        // TODO: Fix bug when removing a Widget from Window.
        window.sendEvent<pTK::Event>(pTK::Event::Category::Window, pTK::Event::Type::WindowDraw);
        return true;
    });

    window.add(checkbox);
    window.add(label);
    // window.add(button);
    
    return app.exec(&window);
}
