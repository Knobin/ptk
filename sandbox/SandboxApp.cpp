// pTK Headers
#include "ptk/ptk.hpp"

// C++ Headers
#include <random>

// Window Size.
constexpr unsigned int SCR_WIDTH{960};
constexpr unsigned int SCR_HEIGHT{540};

pTK::Color RandomColor()
{
    using color_size = pTK::Color::size_type;
    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    std::uniform_int_distribution<color_size> idist(0, std::numeric_limits<color_size>::max());
    color_size hex = idist(rgen);
    const pTK::Color color{hex};
    return color;
}

class CustomBtn : public pTK::Rectangle, public pTK::Text
{
public:
    CustomBtn(const std::string& text)
    {
        setFontSize(14);
        setText(text);
        setColor(pTK::Color{0x00000000});

        onEnter([this](){
            setColor(pTK::Color{0x2C2C2CFF});
            return false;
        });
        onLeave([this](){
            setColor(pTK::Color{colorCopy});
            return false;
        });
        onClick([this](pTK::Mouse::Button, const pTK::Point&){
            colorCopy = pTK::Color{0x272727FF};
            setColor(pTK::Color{0x272727FF});
            return false;
        });
        onRelease([this](pTK::Mouse::Button, const pTK::Point&){
            setColor(pTK::Color{0x2C2C2CFF});
            return false;
        });
    }
    virtual ~CustomBtn() = default;

    void onDraw(SkCanvas* canvas) override
    {
        Rectangle::onDraw(canvas);

        pTK::Vec2f textPos{static_cast<float>(getPosition().x), static_cast<float>(getPosition().y)};
        textPos.x += 10;
        const float fHeight{static_cast<float>(getSize().height)};
        textPos.y = static_cast<float>(getPosition().y) + ((fHeight - capHeight()) / 2);

        drawTextLine(canvas, getText(), pTK::Color{0xF5F5F5FF}, textPos);
    }

    void onTextUpdate() override
    {
        pTK::Size size = getBounds();
        size.width += 20;
        size.height += 20;

        setMinSize(size);
    }

    pTK::Color colorCopy{0x00000000};
};

struct BtnManager
{
    std::vector<pTK::Ref<CustomBtn>> buttons{};

    void setActive(const pTK::Ref<CustomBtn>& set)
    {
        for (auto& btn : buttons)
        {
            if (btn != set)
            {
                btn->colorCopy = pTK::Color{0x00000000};
                btn->setColor(pTK::Color{0x00000000});
            }
        }
    }
};

pTK::Ref<CustomBtn> CreateCustomBtn(const std::string& text, const pTK::Padding& padding, int maxWidth)
{
    // Home.
    pTK::Ref<CustomBtn> btn = pTK::Create<CustomBtn>(text);
    btn->setPadding(padding);
    btn->setCornerRadius(2.5f);
    btn->setMaxSize({maxWidth, btn->getSize().height});
    btn->setAlign(pTK::Align::Top | pTK::Align::HCenter);

    return btn;
}

int main(int argc, char *argv[]) {

    // App.
    pTK::Application app{"SandboxApp", argc, argv};

    // Menu bar.
    pTK::Ref<pTK::MenuBar> menuBar = pTK::Create<pTK::MenuBar>();

    // First menu in menu bar.
    pTK::Ref<pTK::Menu> menu = pTK::Create<pTK::Menu>("File");
    menuBar->addMenu(menu);

    // NamedMenuItem with shortcut in menu.
    pTK::Shortcut shortcut{{pTK::KeyCode::LeftControl}, pTK::KeyCode::Q};
    pTK::Ref<pTK::NamedMenuItem> quitItem = pTK::Create<pTK::NamedMenuItem>("Quit", shortcut);
    menu->addItem(quitItem);

    pTK::WindowInfo flags{};
    flags.visibility = pTK::WindowInfo::Visibility::Windowed;
    flags.backend = pTK::WindowInfo::Backend::Hardware;
    flags.position = {250, 250};
    flags.menus = menuBar;

    pTK::Window window{"pTK Sandbox Window", {960, 540}, flags};
    window.setBackground(pTK::Color(0x1E1E1EFF));
    
    window.onKey([](pTK::Event::Type type, pTK::Key key, byte){
        std::string str = (type == pTK::KeyEvent::Pressed) ? "pressed" : "released";
        std::cout << "Key " << str << ": " << static_cast<int32>(key) << std::endl;
        return false;
    });

    quitItem->onClick("test click", [&window]() {
        std::cout << "Closing window!" << std::endl;
        window.close();
    });

    pTK::Ref<pTK::HBox> hbox = pTK::Create<pTK::HBox>();
    hbox->setMaxSize(pTK::Size::Max);
    hbox->setBackground(pTK::Color(0x1E1E1EFF));

    // VBox as sidebar.
    pTK::Ref<pTK::VBox> sidebar = pTK::Create<pTK::VBox>();
    sidebar->setMaxSize({static_cast<int>(SCR_WIDTH*0.225f), pTK::Size::Limits::Max});
    sidebar->setAlign(pTK::Align::Left);
    sidebar->setBackground(pTK::Color(0x1B1B1BFF));

    uint32 maxWidth = static_cast<uint32>(sidebar->getMaxSize().width*0.85f);
    uint32 paddingLR = static_cast<uint32>((sidebar->getMaxSize().width*0.15f) / 2.0f);
    uint32 paddingTB = static_cast<uint32>(paddingLR / 8.0f);
    pTK::Padding paddingF{paddingLR + paddingLR/2, paddingTB*4, paddingLR, paddingLR};
    pTK::Padding padding{paddingTB, paddingTB, paddingLR, paddingLR};

    pTK::Ref<pTK::Label> navTitle = pTK::Create<pTK::Label>();
    navTitle->setText("Navigation");
    navTitle->setFontSize(12);
    navTitle->setPadding(paddingF);
    navTitle->setColor(pTK::Color{0x8F8F8FFF});
    navTitle->setAlign(pTK::Align::Left | pTK::Align::Top);

    // Buttons.
    pTK::Ref<CustomBtn> home = CreateCustomBtn("Home", padding, maxWidth);
    home->colorCopy = pTK::Color{0x272727FF};
    home->setColor(pTK::Color{0x272727FF});
    pTK::Ref<CustomBtn> projects = CreateCustomBtn("Projects", padding, maxWidth);
    pTK::Ref<CustomBtn> about = CreateCustomBtn("About", padding, maxWidth);
    pTK::Ref<CustomBtn> contact = CreateCustomBtn("Contact", padding, maxWidth);

    BtnManager btnManager{};
    btnManager.buttons.push_back(home);
    btnManager.buttons.push_back(projects);
    btnManager.buttons.push_back(about);
    btnManager.buttons.push_back(contact);

    home->onClick([&](pTK::Mouse::Button, const pTK::Point&){
        btnManager.setActive(home);
        return false;
    });
    projects->onClick([&](pTK::Mouse::Button, const pTK::Point&){
        btnManager.setActive(projects);
        return false;
    });
    about->onClick([&](pTK::Mouse::Button, const pTK::Point&){
        btnManager.setActive(about);
        return false;
    });
    contact->onClick([&](pTK::Mouse::Button, const pTK::Point&){
        btnManager.setActive(contact);
        return false;
    });

    pTK::Ref<pTK::Rectangle> vline = pTK::Create<pTK::Rectangle>();
    vline->setColor(pTK::Color{0x161616FF});
    vline->setMaxSize({1, pTK::Size::Limits::Max});
    vline->setAlign(pTK::Align::Left);

    sidebar->add(navTitle);
    sidebar->add(home);
    sidebar->add(projects);
    sidebar->add(about);
    sidebar->add(contact);

    hbox->add(sidebar);
    hbox->add(vline);

    // VBox as right side content. (TODO: should be a scrollable area).
    pTK::Ref<pTK::VBox> content = pTK::Create<pTK::VBox>();
    content->setAlign(pTK::Align::Left);
    content->setBackground(pTK::Color{0x1E1E1EFF});

    pTK::Ref<pTK::Label> cTitle = pTK::Create<pTK::Label>();
    cTitle->setText("Home");
    cTitle->setFontSize(26);
    cTitle->setPadding({18, 9, 18, 18});
    cTitle->setColor(pTK::Color{0xFEFEFEFF});
    cTitle->setAlign(pTK::Align::Left | pTK::Align::Top);

    pTK::Ref<pTK::Rectangle> hline = pTK::Create<pTK::Rectangle>();
    //hline->setColor(pTK::Color{0xF0F0F0FF});
    hline->setColor(RandomColor());
    hline->setMaxSize({pTK::Size::Limits::Max, 1});
    hline->setPadding({0, 0, 18, 18});
    hline->setAlign(pTK::Align::Left | pTK::Align::Top);

    pTK::Ref<pTK::TextField> textField = pTK::Create<pTK::TextField>();
    textField->setSize({400, 38});
    textField->setFontSize(20);
    textField->setCornerRadius(2.5f);
    // m_placeholderColor = 0x6D757DFF
    // m_textColor = 0x495057FF
    //textField->setOutlineColor(pTK::Color{0xE5E5E5FF});
    textField->setOutlineColor(pTK::Color{0x181818FF});
    textField->setOutlineThickness(1.5f);
    //textField->setColor(pTK::Color{0xFDFDFDFF});
    textField->setPlaceholderColor(pTK::Color{0xD4D4D4FF});
    textField->setTextColor(pTK::Color{0xF4F4F4FF});
    textField->setColor(pTK::Color{0x262626FF});
    textField->setPlaceholderText("PgjFAlaceholder text!");
    std::cout << "Family: " << textField->getFontFamily() << std::endl;

    textField->onClick([&](pTK::Mouse::Button, const pTK::Point&){
        textField->setOutlineColor(pTK::Color{0x007BFFFF});
        return false;
    });
    textField->onLeaveClick([&](){
        textField->setOutlineColor(pTK::Color{0x181818FF});
        return false;
    });

    content->add(cTitle);
    content->add(hline);
    content->add(textField);

    hbox->add(content);

    window.add(hbox);

    window.setMaxSize({1280, 720});
    // window.show();

    return app.exec(&window);
}
