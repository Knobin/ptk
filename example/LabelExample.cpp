#include "ptk/Application.hpp"
#include "ptk/widgets/Label.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Label Example", SCR_WIDTH, SCR_HEIGHT);
    
    std::shared_ptr<pTK::Label> label = std::make_shared<pTK::Label>();
    label->setName("My Label");
    label->setFontSize(48);
    label->setFont("Arial");
    label->setText("Example label");
    label->setColor(pTK::Color(0x161B21FF));
    window.add(label);
    label->onClick([&label](pTK::MouseButton, const pTK::Position&){
        std::cout << label->getName() << " clicked!\n";
        return true;
    });
    
    return app.exec(&window);
}

