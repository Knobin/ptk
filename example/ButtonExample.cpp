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
    
    pTK::Ref<pTK::Button> defaultBtn = pTK::create<pTK::Button>(pTK::Button::Style::Default);
    defaultBtn->setText("Default Button");
    window.add(defaultBtn);
    defaultBtn->onClick([&defaultBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << defaultBtn->getText() << " clicked!\n";
        return true;
    });
    
    pTK::Ref<pTK::Button> successBtn = pTK::create<pTK::Button>(pTK::Button::Style::Success);
    successBtn->setText("Success Button");
    window.add(successBtn);
    successBtn->onClick([&successBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << successBtn->getText() << " clicked!\n";
        return true;
    });
    
    pTK::Ref<pTK::Button> dangerBtn = pTK::create<pTK::Button>(pTK::Button::Style::Danger);
    dangerBtn->setText("Danger Button");
    window.add(dangerBtn);
    dangerBtn->onClick([&dangerBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << dangerBtn->getText() << " clicked!\n";
        return true;
    });
    
    pTK::Ref<pTK::Button> customBtn = pTK::create<pTK::Button>();
    customBtn->setText("Custom Button");
    customBtn->setColor(pTK::Color(0x808080FF));
    customBtn->setTextColor(pTK::Color(0xFFFFFFFF));
    customBtn->setCornerRadius(5);
    customBtn->setClickColor(pTK::Color(0x656565FF));
    customBtn->setHoverColor(pTK::Color(0x707070FF));
    window.add(customBtn);
    customBtn->onClick([&customBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << customBtn->getText() << " clicked!\n";
        return true;
    });
    
    pTK::Ref<pTK::Button> customStyleBtn = pTK::create<pTK::Button>();
    pTK::Button::Style style = pTK::Button::Style::Default;
    style.color = pTK::Color(0x808080FF);
    style.hoverColor = pTK::Color(0x707070FF);
    style.clickColor = pTK::Color(0x656565FF);
    customStyleBtn->setStyle(style);
    customStyleBtn->setText("Custom Style Button");
    window.add(customStyleBtn);
    customStyleBtn->onClick([&customStyleBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << customStyleBtn->getText() << " clicked!\n";
        return true;
    });
    
    pTK::Ref<pTK::Button> customSizeBtn = pTK::create<pTK::Button>(style);
    customSizeBtn->setSize(pTK::Size(250, 100));
    customSizeBtn->setText("Custom Size Button");
    window.add(customSizeBtn);
    customSizeBtn->onClick([&customSizeBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << customSizeBtn->getText() << " clicked!\n";
        return true;
    });
    
    pTK::Ref<pTK::Button> customLabelSizeBtn = pTK::create<pTK::Button>(style);
    customLabelSizeBtn->setFontSize(24);
    customLabelSizeBtn->setText("Custom Label Size Button");
    window.add(customLabelSizeBtn);
    customLabelSizeBtn->onClick([&customLabelSizeBtn](pTK::MouseButton, const pTK::Position&){
        std::cout << customLabelSizeBtn->getText() << " clicked!\n";
        return true;
    });

    return app.exec(&window);
}
