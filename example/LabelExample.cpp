#include "ptk/ptk.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    // Create app and window.
    pTK::Application app("LabelExample Application", argc, argv);
    pTK::Window window("LabelExample Window", {960, 540});

    // Create label.
    std::shared_ptr<pTK::Label> label = std::make_shared<pTK::Label>();

    // Name is NOT the displayed text.
    // This is something optional to set, it can be used to identify the label.
    label->setName("My Label");

    // Set font information.
    label->setFontSize(48);
    label->setFontFamily("Arial");

    // Set displayed text.
    label->setText("Example label");

    // Set colors and outline thickness.
    label->setColor(pTK::Color(0x161B21FF));
    label->setOutlineThickness(1.5f);
    label->setOutlineColor(pTK::Color(0xF4A950FF));

    // Callback for when the label is clicked.
    label->onClick([&label](const pTK::ClickEvent&) {
        std::cout << label->getName() << " clicked!\n";
        return false;
    });

    // Add the label to the window.
    window.add(label);

    // Enables rendering and shows the window.
    window.show();

    return app.exec(&window);
}
