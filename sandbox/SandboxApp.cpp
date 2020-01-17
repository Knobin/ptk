#include "ptk/ptk.hpp"

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

// Label sizes
const unsigned int FLARGE = 24;
const unsigned int FSIDEBAR = 16;
const unsigned int FNORMAL = 14;

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
    
    // HBox.
    pTK::Ref<pTK::HBox> hbox = pTK::create<pTK::HBox>();
    hbox->setSize({static_cast<int>(SCR_WIDTH), static_cast<int>(SCR_HEIGHT)});
    
    // VBox as sidebar.
    pTK::Ref<pTK::VBox> sidebar = pTK::create<pTK::VBox>();
    sidebar->setSize({static_cast<int>(SCR_WIDTH*0.225f), pTK::Size::Limits::Max});
    sidebar->setAlign(pTK::Align::Left);
    sidebar->setBackground(pTK::Color(0x303030FF));
    
    // Sidebar title.
    pTK::Ref<pTK::Label> sTitle = pTK::create<pTK::Label>();
    sTitle->setText("SandboxApp");
    sTitle->setName("SandboxApp");
    sTitle->setFontSize(FLARGE);
    sTitle->setAlign(pTK::Align::Top | pTK::Align::HCenter);
    sTitle->setMargin({18, 18, 18, 18});
    
    // Button style for sidebar.
    pTK::Button::Style sbBtnStyle;
    sbBtnStyle.color = pTK::Color(0x252525FF);
    sbBtnStyle.clickColor = pTK::Color(0x151515FF);
    sbBtnStyle.hoverColor = pTK::Color(0x202020FF);
    sbBtnStyle.textColor = pTK::Color(0xFFFFFFFF);
    sbBtnStyle.cornerRadius = 0.0f;
    
    // First button in sidebar.
    pTK::Ref<pTK::Button> b1 = pTK::create<pTK::Button>(sbBtnStyle);
    b1->setText("Button 1");
    b1->setFontSize(FSIDEBAR);
    b1->setCornerRadius(0.0f);
    b1->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b1->setAlign(pTK::Top);
    
    // Second button in sidebar.
    pTK::Ref<pTK::Button> b2 = pTK::create<pTK::Button>(sbBtnStyle);
    b2->setText("Button 2");
    b2->setFontSize(FSIDEBAR);
    b2->setCornerRadius(0.0f);
    b2->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b2->setAlign(pTK::Top);
    
    // Third button in sidebar.
    pTK::Ref<pTK::Button> b3 = pTK::create<pTK::Button>(sbBtnStyle);
    b3->setText("Button 3");
	b3->setFontSize(FSIDEBAR);
    b3->setCornerRadius(0.0f);
    b3->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b3->setAlign(pTK::Top);
    
    // Bottom button in sidebar.
    pTK::Ref<pTK::Button> b4 = pTK::create<pTK::Button>(pTK::Button::Style::Danger);
    b4->setText("Close");
    b4->setFontSize(FSIDEBAR);
    b4->setCornerRadius(0.0f);
    b4->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b4->setAlign(pTK::Bottom);
    b4->onRelease([&](pTK::Mouse::Button, const pTK::Point&){
        window.close();
        return true;
    });
    
    // VBox as right side content. (TODO: should be a scrollable area).
    pTK::Ref<pTK::VBox> content = pTK::create<pTK::VBox>();
    content->setAlign(pTK::Align::Left);
    content->setBackground(pTK::Color(0xF0F0F0FF));
    
    // Title of right side content.
    pTK::Ref<pTK::Label> cTitle = pTK::create<pTK::Label>();
    cTitle->setText("Content");
    cTitle->setName("Content label");
    cTitle->setFontSize(FLARGE);
    cTitle->setMargin({18, 9, 18, 18});
    cTitle->setColor(pTK::Color());
    cTitle->setAlign(pTK::Align::Left | pTK::Align::Top);

    // This should be a pTK::TextArea (when that is implemented).
    pTK::Ref<pTK::Label> cText = pTK::create<pTK::Label>();
    cText->setText("This should be a multiline text!");
    cText->setName("TextArea");
    cText->setFontSize(FNORMAL);
    cText->setMargin({9, 18, 18, 18});
    cText->setColor(pTK::Color());
    cText->setAlign(pTK::Align::Left | pTK::Align::Top);

    // Just a rectangle. Nothing special about it.
    pTK::Ref<pTK::Rectangle> rect = pTK::create<pTK::Rectangle>();
    rect->setName("rect");
    rect->setColor(pTK::Color(0xC0C0C0FF));
    rect->setConstSize({250, 125});

	pTK::Ref<pTK::Checkbox> checkbox = pTK::create<pTK::Checkbox>();
	checkbox->setConstSize({ 25, 25 });
	checkbox->setCornerRadius(3); // 10% of size
	checkbox->setOutlineThickness(1.5f); // 5% of size
	checkbox->setColor(pTK::Color(0x00FF00FF));
	checkbox->setOutlineColor(pTK::Color(0xFF00FFFF));
	checkbox->setAlign(pTK::Align::Left | pTK::Align::Bottom);
	checkbox->setMargin({ 18, 9, 18, 18 });

	pTK::Ref<pTK::Label> cStatus = pTK::create<pTK::Label>();
	cStatus->setText("Checkbox Status: false");
	cStatus->setName("Checkbox Status label");
	cStatus->setFontSize(25);
	cStatus->setMargin({ 18, 9, 18, 18 });
	cStatus->setColor(pTK::Color());
	cStatus->setAlign(pTK::Align::Left | pTK::Align::Top);

	checkbox->onToggle([&](bool status) {
		std::string statusText = "Checkbox Status: ";
		statusText.append((status) ? "true" : "false");
		cStatus->setText(statusText);
		return true;
	});
    
    // Add content to sidebar.
    sidebar->add(sTitle);
    sidebar->add(b1);
    sidebar->add(b2);
    sidebar->add(b3);
    sidebar->add(b4);

    // Set Maxsize of sidebar.
    sidebar->setMaxSize({sidebar->getSize().width, pTK::Size::Limits::Max});
    
    // Add content to right side.
    content->add(cTitle);
    content->add(cText);
    content->add(rect);
	content->add(checkbox);
	content->add(cStatus);
    
    // Add content to hbox.
    hbox->add(sidebar);
    hbox->add(content);

    // Add hbox to window.
    window.add(hbox);
    
    return app.exec(&window);
}
