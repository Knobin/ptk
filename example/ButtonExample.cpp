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
    
    std::shared_ptr<pTK::Button> defaultButton = std::make_shared<pTK::Button>(pTK::Button::Style::Default);
    defaultButton->setText("Default Button");
    window.add(defaultButton);
    defaultButton->onClick([&defaultButton](pTK::MouseButton, const pTK::Position&){
        std::cout << defaultButton->getText() << " clicked!\n";
        return true;
    });
    
    std::shared_ptr<pTK::Button> successButton = std::make_shared<pTK::Button>(pTK::Button::Style::Success);
    successButton->setText("Success Button");
    window.add(successButton);
    successButton->onClick([&successButton](pTK::MouseButton, const pTK::Position&){
        std::cout << successButton->getText() << " clicked!\n";
        return true;
    });
    
    std::shared_ptr<pTK::Button> dangerButton = std::make_shared<pTK::Button>(pTK::Button::Style::Danger);
    dangerButton->setText("Danger Button");
    window.add(dangerButton);
    dangerButton->onClick([&dangerButton](pTK::MouseButton, const pTK::Position&){
        std::cout << dangerButton->getText() << " clicked!\n";
        return true;
    });
    
    return app.exec(&window);
}
