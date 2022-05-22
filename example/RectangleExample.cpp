#include "ptk/ptk.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    // Create app and window.
    pTK::Application app("RectangleExample Application", argc, argv);
    pTK::Window window("RectangleExample Window", {960, 540});

    // Create rectangle.
    pTK::Ref<pTK::Rectangle> rect = pTK::Create<pTK::Rectangle>();

    // This is something optional to set, it can be used to identify the rectangle.
    rect->setName("My Rectangle");

    // Set size, position and corner radius.
    rect->setSize(pTK::Size(250, 100));
    rect->setPosHint(pTK::Point(250, 250));
    rect->setCornerRadius(10);

    // Set colors and outline thickness.
    rect->setColor(pTK::Color(0x161B21FF));
    rect->setOutlineThickness(2);
    rect->setOutlineColor(pTK::Color(0xF4A950FF));

    // Callback for when the rectangle is clicked.
    rect->onClick([&rect](const pTK::ClickEvent&){
        std::cout << rect->getName() << " clicked!\n";
        return false;
    });

    // Add the label to the window.
    window.add(rect);

    // Enables rendering and shows the window.
    window.show();

    // Run the application.
    return app.exec(&window);
}
