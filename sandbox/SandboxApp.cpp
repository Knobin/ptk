// pTK Headers
#include "ptk/ptk.hpp"

// C++ Headers
#include <iostream>
#include <random>
#include <thread>
#include <atomic>
#include <chrono>

// settings
constexpr unsigned int SCR_WIDTH{960};
constexpr unsigned int SCR_HEIGHT{540};

// Label sizes
constexpr unsigned int FLARGE{20};
constexpr unsigned int FSIDEBAR{16};
constexpr unsigned int FNORMAL{13};

// Colors
uint colSidebar{0xF2F3F5FF};
uint colContent{0xFFFFFFFF};
uint colText{0x1C1D19FF};

// Sidebar button style
const pTK::Button::Style sbBtnStyle{pTK::Color(0xE3E5E8FF),
                            pTK::Color(0xE0E2E5FF),
                            pTK::Color(0xDCDFE2FF),
                            pTK::Color(colText), 0.0f};

pTK::Color randomColor()
{
    using color_size = pTK::Color::size_type;
    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    std::uniform_int_distribution<color_size> idist(0, std::numeric_limits<color_size>::max());
    color_size hex = idist(rgen);
    const pTK::Color color{hex};
    return color;
}

void setWindowCallbacks(pTK::Window& window)
{
    window.onFocus([&](){
        std::cout << "FOCUS" << std::endl;
        return false;
    });

    window.onMove([&](const pTK::Point& pos){
        std::cout << "MOVE " << pos.x << "x" << pos.y << std::endl;
        return false;
    });

    window.onLostFocus([&](){
        std::cout << "LOST FOCUS" << std::endl;
        return false;
    });

    window.onClose([&](){
        std::cout << "CLOSE" << std::endl;
        return false;
    });

    window.onResize([&](const pTK::Size& size){
        std::cout << "RESIZE: " << size.width << "x" << size.height << std::endl;
        return false;
    });

    window.onMinimize([&](){
        std::cout << "MINIMIZED" << std::endl;
        return false;
    });

    window.onRestore([&](){
        std::cout << "RESTORED" << std::endl;
        return false;
    });
}

pTK::Ref<pTK::MenuBar> CreateMenuBar()
{
    // Menu bar.
    pTK::Ref<pTK::MenuBar> menuBar = pTK::Create<pTK::MenuBar>();

    // First menu in menu bar.
    pTK::Ref<pTK::Menu> menu = pTK::Create<pTK::Menu>("TEST MENU");
    menuBar->addMenu(menu);

    // NamedMenuItem with shortcut in menu.
    pTK::Shortcut shortcut{{pTK::KeyCode::LeftControl}, pTK::KeyCode::S};
    pTK::Ref<pTK::NamedMenuItem> menuItem1 = pTK::Create<pTK::NamedMenuItem>("TEST ITEM 1", shortcut);
    menu->addItem(menuItem1);
    menuItem1->onClick("test click", [](){
        std::cout << "menuItem1 clicked!" << std::endl;
    });

    // Separator in menu.
    menu->addItem(pTK::CreateSeparator());

    // NamedMenuItem in menu.
    pTK::Ref<pTK::NamedMenuItem> menuItem3 = pTK::Create<pTK::NamedMenuItem>("TEST ITEM 3");
    menu->addItem(menuItem3);
    menuItem3->onClick("test click", [](){
        std::cout << "menuItem3 clicked!" << std::endl;
    });

    // Separator in menu.
    menu->addItem(pTK::CreateSeparator());

    // CheckboxMenuItem in menu.
    pTK::Ref<pTK::CheckboxMenuItem> checkItem = pTK::Create<pTK::CheckboxMenuItem>("Checkbox item");
    menu->addItem(checkItem);
    checkItem->onClick("test click", [](){
        std::cout << "checkItem clicked!" << std::endl;
    });
    checkItem->onToggle("test toggle", [](pTK::MenuItemStatus status){
        std::cout << "checkItem toggled to: " << MenuItemStatusToStr(status) << std::endl;
    });

    // Submenu for menu.
    pTK::Ref<pTK::Menu> submenu = pTK::Create<pTK::Menu>("TEST SUBMENU");
    menu->addItem(submenu);

    // NamedMenuItem in submenu.
    pTK::Ref<pTK::NamedMenuItem> menuItem2 = pTK::Create<pTK::NamedMenuItem>("TEST ITEM 2");
    submenu->addItem(menuItem2);
    menuItem2->onClick("test click", [](){
        std::cout << "menuItem2 clicked!" << std::endl;
    });

    return menuBar;
}

int main(int argc, char *argv[]) {

    pTK::Application app{"SandboxApp", argc, argv};

    pTK::WindowInfo flags{};
    flags.visibility = pTK::WindowInfo::Visibility::Windowed;
    flags.backend = pTK::WindowInfo::Backend::Hardware;
    flags.position = {250, 250};
    flags.menus = CreateMenuBar();

    pTK::Window window{"pTK Sandbox Window", {960, 540}, flags};
    window.setBackground(pTK::Color(0x232323FF));
    setWindowCallbacks(window);

    // Set ESC key to close the window (and other debug stuff).
    window.onKey([&](pTK::Event::Type, pTK::KeyCode key, byte) {
        std::cout << "Key event " << static_cast<int32>(key) << std::endl;
        return false;
    });
    
    window.onClick([](pTK::Mouse::Button, const pTK::Point& pos){
        std::cout << "Click: " << pos.x << "x" << pos.y << std::endl;
        return false;
    });

    // HBox.
    pTK::Ref<pTK::HBox> hbox = pTK::Create<pTK::HBox>();
    hbox->setSize({static_cast<int>(SCR_WIDTH), static_cast<int>(SCR_HEIGHT)});
    std::cout << hbox->getSize().width << "x" << hbox->getSize().height << std::endl;

    // VBox as sidebar.
    pTK::Ref<pTK::VBox> sidebar = pTK::Create<pTK::VBox>();
    sidebar->setSize({static_cast<int>(SCR_WIDTH*0.225f), SCR_HEIGHT});
    sidebar->setAlign(pTK::Align::Left);
    sidebar->setBackground(pTK::Color(colSidebar));

    // Sidebar title.
    pTK::Ref<pTK::Label> sTitle = pTK::Create<pTK::Label>();
    sTitle->setText("SandboxApp");
    //sTitle->setFontFamily("Arial");
    sTitle->setFontSize(FLARGE);
    sTitle->setColor(pTK::Color(colText));
    sTitle->setAlign(pTK::Align::Top | pTK::Align::HCenter);
    sTitle->setPadding({18, 18, 18, 18});

    // First button in sidebar.
    pTK::Ref<pTK::Button> b1 = pTK::Create<pTK::Button>(sbBtnStyle);
    b1->setText("Button 1");
    b1->setFontSize(FSIDEBAR);
    b1->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b1->setAlign(pTK::Top);
    b1->onRelease([](pTK::Mouse::Button, const pTK::Point&) {
        std::cout << "Do stuff with B1" << std::endl;
        return false;
    });

    // Second button in sidebar.
    pTK::Ref<pTK::Button> b2 = pTK::Create<pTK::Button>(sbBtnStyle);
    b2->setText("Button 2");
    b2->setFontSize(FSIDEBAR);
    b2->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b2->setAlign(pTK::Top);
    b2->onRelease([](pTK::Mouse::Button, const pTK::Point&) {
      std::cout << "Do stuff with B2" << std::endl;
      return false;
    });

    // Third button in sidebar.
    pTK::Ref<pTK::Button> b3 = pTK::Create<pTK::Button>(sbBtnStyle);
    b3->setText("Button 3");
    b3->setFontSize(FSIDEBAR);
    b3->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b3->setAlign(pTK::Top);
    b3->onRelease([](pTK::Mouse::Button, const pTK::Point&) {
      std::cout << "Do stuff with B3" << std::endl;
      return false;
    });

    // Bottom button in sidebar.
    pTK::Ref<pTK::Button> quit = pTK::Create<pTK::Button>(pTK::Button::Style::Danger);
    quit->setText("Close");
    quit->setFontSize(FSIDEBAR);
    quit->setCornerRadius(0.0f);
    quit->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    quit->setAlign(pTK::Bottom);
    quit->onRelease([&](pTK::Mouse::Button, const pTK::Point&){
        window.close();
        return false;
    });

    // VBox as right side content. (TODO: should be a scrollable area).
    pTK::Ref<pTK::VBox> content = pTK::Create<pTK::VBox>();
    content->setAlign(pTK::Align::Left);
    content->setBackground(pTK::Color(colContent));

    // Title of right side content.
    pTK::Ref<pTK::Label> cTitle = pTK::Create<pTK::Label>();
    cTitle->setText("Content");
    cTitle->setFontSize(FLARGE);
    cTitle->setPadding({18, 9, 18, 18});
    cTitle->setColor(pTK::Color(colText));
    //cTitle->setAlign(pTK::Align::Left | pTK::Align::Top);

    pTK::Ref<pTK::Rectangle> r1 = pTK::Create<pTK::Rectangle>();
    pTK::Color rColor{randomColor()};
    rColor.a = 255;
    r1->setColor(rColor);
    r1->setSize({250, 125});
    r1->setCornerRadius(5.0f);
    r1->setPadding({10, 10, 18, 18});

    pTK::Ref<pTK::Rectangle> r2 = pTK::Create<pTK::Rectangle>(*r1.get());
    rColor = randomColor();
    rColor.a = 255;
    r2->setColor(rColor);
    r2->onClick([&](pTK::Mouse::Button btn, const pTK::Point&) {
      if (btn == pTK::Mouse::Button::Left)
          r2->setColor(randomColor());
      return false;
    });

    pTK::Ref<pTK::HBox> h1 = pTK::Create<pTK::HBox>();
    h1->setAlign(pTK::Align::Left);
    h1->setName("HBOX");
    h1->setBackground(content->getBackground());

    // This should be a pTK::TextArea (when that is implemented).
    pTK::Ref<pTK::Label> cText = pTK::Create<pTK::Label>();
    cText->setText("This should be a multiline text!");
    cText->setFontSize(16);
    cText->setPadding({9, 18, 18, 18});
    cText->setColor(pTK::Color(colText));
    //cText->setAlign(pTK::Align::Left | pTK::Align::Bottom);

    pTK::Ref<pTK::Checkbox> checkbox = pTK::Create<pTK::Checkbox>();
    checkbox->setSize({ 25, 25 });
    checkbox->setCornerRadius(3); // 10% of size
    checkbox->setOutlineThickness(1.5f); // 5% of size
    checkbox->setColor(pTK::Color(0x00FF00FF));
    checkbox->setOutlineColor(pTK::Color(0xFF00FFFF));
    checkbox->setAlign(pTK::Align::Bottom | pTK::Align::HCenter);
    checkbox->setPadding({ 18, 9, 18, 18 });

    pTK::Ref<pTK::Label> cStatus = pTK::Create<pTK::Label>();
    cStatus->setText("Checkbox Status: false");
    cStatus->setFontSize(25);
    cStatus->setPadding({ 18, 9, 18, 18 });
    cStatus->setColor(pTK::Color(colText));
    // cStatus->setAlign(pTK::Align::Left | pTK::Align::Bottom);

    checkbox->onToggle([&](bool status) {
        std::string statusText = "Checkbox Status: ";
        statusText.append((status) ? "true" : "false");
        cStatus->setText(statusText);
        return false;
    });

    // Add content to sidebar.
    sidebar->add(sTitle);
    sidebar->add(b1);
    sidebar->add(b2);
    sidebar->add(b3);
    sidebar->add(quit);

    // Set Maxsize of sidebar.
    sidebar->setMaxSize({sidebar->getSize().width, pTK::Size::Limits::Max});

    // Add content to h1.
    h1->add(r1);
    h1->add(r2);

    // Set MaxSize of h1.
    h1->setMaxSize({pTK::Size::Limits::Max, h1->getMinSize().height});

    // Add content to right side.
    cTitle->onKey([](pTK::Event::Type, pTK::KeyCode code, byte) {
        std::cout << "cTitle key event: " << pTK::KeyCodeToAlpha(code) << std::endl;
        return false;
    });

    pTK::Ref<pTK::TextField> textField = pTK::Create<pTK::TextField>();
    textField->setSize({400, 38});
    textField->setFontSize(20);
    //textField->setFontFamily("Roboto");
    textField->setCornerRadius(2.5f);
    textField->setOutlineColor(pTK::Color{0xE5E5E5FF});
    textField->setOutlineThickness(1.5f);
    textField->setColor(pTK::Color{0xFDFDFDFF});
    textField->setPlaceholderText("RPlaceholder text!");

    textField->onClick([&](pTK::Mouse::Button, const pTK::Point&){
        textField->setOutlineColor(pTK::Color{0x007BFFFF});
        return false;
    });
    textField->onLeaveClick([&](){
        textField->setOutlineColor(pTK::Color{0xE5E5E5FF});
        return false;
    });

    content->add(cTitle);
    content->add(cText);
    content->add(textField);
    content->add(h1);
    content->add(checkbox);
    content->add(cStatus);

    // Add content to hbox.
    hbox->add(sidebar);
    hbox->add(content);

    // Add hbox to window.
    window.add(hbox);

    window.setMaxSize({1280, 720});
    // window.show();

    return app.exec(&window);
}
