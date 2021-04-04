#include "ptk/Application.hpp"
#include "ptk/widgets/Label.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    pTK::Application app("LabelExample Application", argc, argv);
    pTK::Window window("LabelExample Window", {SCR_WIDTH, SCR_HEIGHT});
    
    pTK::Ref<pTK::Label> label = pTK::Create<pTK::Label>();
    label->setName("My Label");
    label->setFontSize(48);
    label->setFontFamily("Arial");
    label->setText("Example label");
    label->setColor(pTK::Color(0x161B21FF));
    label->setOutlineThickness(1.5f);
    label->setOutlineColor(pTK::Color(0xF4A950FF));
    window.add(label);
    label->onClick([&label](pTK::Mouse::Button, const pTK::Point&){
        std::cout << label->getName() << " clicked!\n";
        return true;
    });
    
    return app.exec(&window);
}

