#include "ptk/Application.hpp"
#include "ptk/widgets/Button.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

int main(int argc, char *argv[])
{
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Button Example", SCR_WIDTH, SCR_HEIGHT);
    
    std::shared_ptr<pTK::Button> btn = std::make_shared<pTK::Button>();
    btn->setColor(pTK::Color(0x007BFFFF));
    btn->setName("My Button");
    btn->setText("Test Button");
    btn->setTextColor(pTK::Color(0xFFFFFFFF));
    btn->setCornerRadius(5);
    btn->setClickColor(pTK::Color(0x0068D9FF));
    btn->setHoverColor(pTK::Color(0x0071E6FF));
    window.add(btn);
    btn->onClick([&btn](pTK::MouseButton, const pTK::Position&){
        std::cout << btn->getName() << " clicked!\n";
        return true;
    });
    
    return app.exec(&window);
}
