// pTK Headers
#include "ptk/ptk.hpp"
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <iostream>
#include <random>
#include <thread>
#include <atomic>
#include <chrono>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

// Label sizes
const unsigned int FLARGE = 20;
const unsigned int FSIDEBAR = 16;
const unsigned int FNORMAL = 13;

// Colors
uint colSidebar{0x252525FF};
uint colContent{0x171717FF};
uint colText{0xF5F5F5FF};

pTK::Color interpolateColor(const pTK::Color& c1, const pTK::Color& c2, double factor)
{
    pTK::Color result{c1};
    result.r = static_cast<pTK::Color::value_type>(std::round(c1.r + factor * (c2.r - c1.r)));
    result.g = static_cast<pTK::Color::value_type>(std::round(c1.g + factor * (c2.g - c1.g)));
    result.b = static_cast<pTK::Color::value_type>(std::round(c1.b + factor * (c2.b - c1.b)));
    return result;
}

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
        return true;
    });

    window.onMove([&](const pTK::Point& pos){
        std::cout << "MOVE " << pos.x << "x" << pos.y << std::endl;
        return true;
    });

    window.onLostFocus([&](){
        std::cout << "LOST FOCUS" << std::endl;
        return true;
    });

    window.onClose([&](){
        std::cout << "CLOSE" << std::endl;
        return true;
    });

    window.onResize([&](const pTK::Size& size){
        std::cout << "RESIZE: " << size.width << "x" << size.height << std::endl;
        return true;
    });

    window.onMinimize([&](){
        std::cout << "MINIMIZED" << std::endl;
        return true;
    });

    window.onRestore([&](){
        std::cout << "RESTORED" << std::endl;
        return true;
    });
}

int main(int argc, char *argv[]) {
    pTK::Application app{argc, argv};
    pTK::Window window{"pTK Sandbox Window", {960, 540}, pTK::BackendType::HARDWARE};
    window.setBackground(pTK::Color(0x232323FF));
    setWindowCallbacks(window);

    // Set ESC key to close the window.
    window.onKey([&](pTK::Event::Type type, pTK::KeyCode key) {
        if ((type == pTK::KeyEvent::Released) && (key == pTK::Key::Escape))
            window.close();
        if ((type == pTK::KeyEvent::Released) && (key == pTK::Key::Space))
            window.postEvent<pTK::ResizeEvent>(pTK::Size{1000, 600});
        if ((type == pTK::KeyEvent::Released) && (key == pTK::Key::M))
        {
            window.minimize();
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
            window.minimize();
            std::this_thread::sleep_for(1s);
            window.restore();
        }

        return true;
    });

    // HBox.
    pTK::Ref<pTK::HBox> hbox = pTK::create<pTK::HBox>();
    hbox->setSize({static_cast<int>(SCR_WIDTH), static_cast<int>(SCR_HEIGHT)});
    std::cout << hbox->getSize().width << "x" << hbox->getSize().height << std::endl;

    // VBox as sidebar.
    pTK::Ref<pTK::VBox> sidebar = pTK::create<pTK::VBox>();
    sidebar->setSize({static_cast<int>(SCR_WIDTH*0.225f), pTK::Size::Limits::Max});
    sidebar->setAlign(pTK::Align::Left);
    sidebar->setBackground(pTK::Color(colSidebar));

    // Sidebar title.
    pTK::Ref<pTK::Label> sTitle = pTK::create<pTK::Label>();
    sTitle->setText("SandboxApp");
    //sTitle->setFontFamily("Arial");
    sTitle->setFontSize(FLARGE);
    sTitle->setColor(pTK::Color(colText));
    sTitle->setAlign(pTK::Align::Top | pTK::Align::HCenter);
    sTitle->setPadding({18, 18, 18, 18});

    // Button style for sidebar.
    pTK::Button::Style sbBtnStyle;
    sbBtnStyle.color = pTK::Color(0x212121FF);
    sbBtnStyle.clickColor = pTK::Color(0x171717FF);
    sbBtnStyle.hoverColor = pTK::Color(0x1f1f1fFF);
    sbBtnStyle.textColor = pTK::Color(colText);
    sbBtnStyle.cornerRadius = 0.0f;

    // First button in sidebar.
    pTK::Ref<pTK::Button> b1 = pTK::create<pTK::Button>(sbBtnStyle);
    b1->setText("Button 1");
    b1->setFontSize(FSIDEBAR);
    b1->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b1->setAlign(pTK::Top);

    // Second button in sidebar.
    pTK::Ref<pTK::Button> b2 = pTK::create<pTK::Button>(sbBtnStyle);
    b2->setText("Button 2");
    b2->setFontSize(FSIDEBAR);
    b2->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b2->setAlign(pTK::Top);

    // Third button in sidebar.
    pTK::Ref<pTK::Button> b3 = pTK::create<pTK::Button>(sbBtnStyle);
    b3->setText("Button 3");
    b3->setFontSize(FSIDEBAR);
    b3->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    b3->setAlign(pTK::Top);

    // Bottom button in sidebar.
    pTK::Ref<pTK::Button> quit = pTK::create<pTK::Button>(pTK::Button::Style::Danger);
    quit->setText("Close");
    quit->setFontSize(FSIDEBAR);
    quit->setCornerRadius(0.0f);
    quit->setMaxSize({pTK::Size::Limits::Max, SCR_HEIGHT/10});
    quit->setAlign(pTK::Bottom);
    quit->onRelease([&](pTK::Mouse::Button, const pTK::Point&){
        window.close();
        return true;
    });

    // VBox as right side content. (TODO: should be a scrollable area).
    pTK::Ref<pTK::VBox> content = pTK::create<pTK::VBox>();
    content->setAlign(pTK::Align::Left);
    content->setBackground(pTK::Color(colContent));

    // Title of right side content.
    pTK::Ref<pTK::Label> cTitle = pTK::create<pTK::Label>();
    cTitle->setText("Content");
    cTitle->setFontSize(FLARGE);
    cTitle->setPadding({18, 9, 18, 18});
    cTitle->setColor(pTK::Color(colText));
    //cTitle->setAlign(pTK::Align::Left | pTK::Align::Top);

    pTK::Ref<pTK::Rectangle> r1 = pTK::create<pTK::Rectangle>();
    r1->setColor(pTK::Color(0x2D2D2DFF));
    r1->setConstSize({250, 125});
    r1->setCornerRadius(5.0f);
    r1->setPadding({10, 10, 18, 18});

    pTK::Ref<pTK::Rectangle> r2 = pTK::create<pTK::Rectangle>(*r1.get());
    r2->onClick([&](pTK::Mouse::Button btn, const pTK::Point&) {
      if (btn == pTK::Mouse::Button::Left)
          r2->setColor(randomColor());
      return true;
    });

    pTK::Ref<pTK::HBox> h1 = pTK::create<pTK::HBox>();
    h1->setAlign(pTK::Align::Left);
    h1->setName("HBOX");
    h1->setBackground(content->getBackground());

    // This should be a pTK::TextArea (when that is implemented).
    pTK::Ref<pTK::Label> cText = pTK::create<pTK::Label>();
    cText->setText("This should be a multiline text!");
    cText->setFontSize(16);
    cText->setPadding({9, 18, 18, 18});
    cText->setColor(pTK::Color(colText));
    //cText->setAlign(pTK::Align::Left | pTK::Align::Bottom);

    pTK::Ref<pTK::Checkbox> checkbox = pTK::create<pTK::Checkbox>();
    checkbox->setConstSize({ 25, 25 });
    checkbox->setCornerRadius(3); // 10% of size
    checkbox->setOutlineThickness(1.5f); // 5% of size
    checkbox->setColor(pTK::Color(0x00FF00FF));
    checkbox->setOutlineColor(pTK::Color(0xFF00FFFF));
    checkbox->setAlign(pTK::Align::Bottom | pTK::Align::HCenter);
    checkbox->setPadding({ 18, 9, 18, 18 });

    pTK::Ref<pTK::Label> cStatus = pTK::create<pTK::Label>();
    cStatus->setText("Checkbox Status: false");
    cStatus->setFontSize(25);
    cStatus->setPadding({ 18, 9, 18, 18 });
    cStatus->setColor(pTK::Color(colText));
    // cStatus->setAlign(pTK::Align::Left | pTK::Align::Bottom);

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
    sidebar->add(quit);

    // Set Maxsize of sidebar.
    sidebar->setMaxSize({sidebar->getSize().width, pTK::Size::Limits::Max});

    // Add content to h1.
    h1->add(r1);
    h1->add(r2);

    // Set MaxSize of h1.
    h1->setMaxSize({pTK::Size::Limits::Max, h1->getMinSize().height});

    // Add content to right side.
    content->add(cTitle);
    content->add(cText);
    content->add(h1);
    content->add(checkbox);
    content->add(cStatus);

    // Add content to hbox.
    hbox->add(sidebar);
    hbox->add(content);

    // Add hbox to window.
    window.add(hbox);

    //window.setMaxSize({1280, 720});

    std::atomic<bool> run{true};
    std::thread t1{[&](){
        pTK::Color c1{randomColor()};
        pTK::Color c2{randomColor()};

        std::size_t steps{250};
        double stepFactor{1.0 / (steps - 1.0)};
        size_t step{0};

        while (run)
        {
            if (step > steps)
            {
                c1 = r1->getColor();
                c2 = randomColor();
                step = 1;
            }
            r1->setColor(interpolateColor(c1, c2, stepFactor * step));
            std::this_thread::sleep_for(std::chrono::milliseconds (1000 / 144));
            ++step;
        }
    }};

    int ret{app.exec(&window)};
    run = false;
    t1.join();

    return ret;
}
