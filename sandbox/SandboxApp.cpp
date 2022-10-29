// pTK Headers
#include "ptk/ptk.hpp"

// C++ Headers
#include <random>

// Window Size.
static constexpr pTK::Size::value_type SCR_WIDTH{960};
static constexpr pTK::Size::value_type SCR_HEIGHT{540};

static pTK::Color RandomColor()
{
    using color_size = pTK::Color::size_type;
    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    std::uniform_int_distribution<color_size> idist(0, std::numeric_limits<color_size>::max());
    color_size hex = idist(rgen);
    hex |= 0xFF; // set alpha bits to 255.
    const pTK::Color color{hex};

    return color;
}

class CustomBtn : public pTK::Rectangle, public pTK::Text
{
public:
    CustomBtn(const std::string& str)
    {
        setFontSize(14);
        text = str;
        setColor(pTK::Color{0x00000000});
        onTextUpdate();

        onEnter([this](const pTK::EnterEvent&){
            setColor(hoverColor);
            return false;
        });
        onLeave([this](const pTK::LeaveEvent&){
            setColor(pTK::Color{colorCopy});
            return false;
        });
        onClick([this](const pTK::ClickEvent&){
            colorCopy = clickColor;
            setColor(clickColor);
            return false;
        });
        onRelease([this](const pTK::ReleaseEvent&){
            setColor(hoverColor);
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

        pTK::Text::StrData data{ text.c_str(), text.size(), pTK::Text::Encoding::UTF8 };
        drawTextLine(canvas, data, pTK::Color{ 0xF5F5F5FF }, textPos);
    }

    void onTextUpdate() override
    {
        pTK::Vec2f textBounds{getBoundsFromStr(text)};
        textBounds.x += 20;
        textBounds.y += 20;

        setMinSize(Vec2ToSize(textBounds, pTK::Math::ceilf));
    }

    pTK::Color colorCopy{0x00000000};
    pTK::Color hoverColor{0x2C2C2CFF};
    pTK::Color clickColor{0x272727FF};
    std::string text{};
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

pTK::Ref<CustomBtn> CreateCustomBtn(const std::string& text, const pTK::Margin& margin, pTK::Size::value_type maxWidth)
{
    pTK::Ref<CustomBtn> btn = pTK::Create<CustomBtn>(text);
    btn->setSizePolicy(pTK::SizePolicy::Type::Expanding);
    btn->setMargin(margin);
    btn->setCornerRadius(2.5f);
    btn->setMaxSize({maxWidth, btn->getSize().height});
    btn->setAlign(pTK::Align::Top, pTK::Align::HCenter);

    return btn;
}

class SpinningRect : public pTK::Widget
{
public:
    SpinningRect() = default;
    virtual ~SpinningRect() = default;

    void onDraw(SkCanvas *canvas) override
    {
        // Size and pos to Skia variants.
        const SkPoint pos{convertToSkPoint(getPosition())};
        SkPoint size{convertToSkPoint(getSize())};
        SkRect rect = SkRect::MakeXYWH(pos.x(), pos.y(), size.x(), size.y());

        SkPaint paint{};
        paint.setAntiAlias(true);
        paint.setARGB(255, 100, 90, 110);

        canvas->drawRoundRect(rect, 4, 4, paint);

        canvas->save();

        SkRect smallRect = SkRect::MakeXYWH(pos.x() + size.x() / 4, pos.y() + size.y() / 4, size.x() / 2, size.y() / 2);
        paint.setARGB(255, 150, 140, 160);
        canvas->rotate(m_degrees, rect.centerX(), rect.centerY());
        canvas->drawRoundRect(smallRect, 4, 4, paint);

        canvas->restore();
    }

    void setDegrees(float degrees)
    {
        m_degrees = degrees;
        draw();
    }

    float getDegrees() const
    {
        return m_degrees;
    }

private:
    float m_degrees{0.0f};
};

#include <type_traits>

//template <typename T>
//struct test_type<T, T::callback_type> : std::true_type {};

/*template <typename T, typename T::callback_type>
struct test_type
{
    static constexpr bool value = true;
};*/

template <typename T, typename = void>
struct callback_type_exists : std::false_type {};

template <typename T>
struct callback_type_exists<T, std::void_t<typename T::callback_type>> : std::true_type {};

struct TotallyUniqueClass
{
    using callback_type = void(int);
};

struct TotallyUniqueClass2
{

};

int main(int argc, char *argv[])
{
    std::cout << "callback_type_exists<int> : " << callback_type_exists<int>::value << std::endl;
    std::cout << "callback_type_exists<TotallyUniqueClass> : " << callback_type_exists<TotallyUniqueClass>::value << std::endl;
    std::cout << "callback_type_exists<TotallyUniqueClass2> : " << callback_type_exists<TotallyUniqueClass2>::value << std::endl;

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

    pTK::Window window{"pTK Sandbox Window", {SCR_WIDTH, SCR_HEIGHT}, flags};
    window.setBackground(pTK::Color(0x1E1E1EFF));

    window.onKey([](const pTK::KeyEvent& evt){
        std::string str = (evt.type == pTK::KeyEvent::Pressed) ? "pressed" : "released";
        std::cout << "Key " << str << ": " << static_cast<int32_t>(evt.keycode) << std::endl;
        return false;
    });

    window.addListener<pTK::MinimizeEvent>([](){
        std::cout << "Window minimized!" << std::endl;
        return false;
    });

    window.addListener<pTK::RestoreEvent>([](){
        std::cout << "Window restored!" << std::endl;
        return false;
    });

    window.addListener<pTK::FocusEvent>([](){
        std::cout << "Window focused!" << std::endl;
        return false;
    });

    window.addListener<pTK::LostFocusEvent>([](){
        std::cout << "Window lost focus!" << std::endl;
        return false;
    });

    window.addListener<int>([]() {
        std::cout << "Integer callback 1"<< std::endl;
        return false;
    });

    window.addListener<int>([](const int& val){
        std::cout << "Integer callback 2 with value = " << val << std::endl;
        return false;
    });

    // Removes callback when R is pressed.
    uint64_t cb_id = window.onKey([&](const pTK::KeyEvent& evt) {
        if (pTK::IsKeyCodeDigit(evt.keycode))
            window.triggerEvent<int>(pTK::KeyCodeToDigit(evt.keycode));

        return (evt.keycode == pTK::Key::R);
    });

    std::cout << "Callback id to auto-remove: " << cb_id << std::endl;

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

    auto maxWidth = static_cast<pTK::Size::value_type>(static_cast<float>(sidebar->getMaxSize().width)*0.85f);
    auto marginLR = static_cast<pTK::Size::value_type>((static_cast<float>(sidebar->getMaxSize().width)*0.15f) / 2.0f);
    auto marginTB = static_cast<pTK::Size::value_type>(static_cast<float>(marginLR) / 8.0f);
    pTK::Margin marginF{marginLR + marginLR/2, marginTB*4, marginLR, marginLR};
    pTK::Margin margin{marginTB, marginTB, marginLR, marginLR};

    pTK::Ref<pTK::Label> navTitle = pTK::Create<pTK::Label>();
    navTitle->setText("Navigation");
    navTitle->setFontSize(12);
    navTitle->setMargin(marginF);
    navTitle->setColor(pTK::Color{0x8F8F8FFF});
    navTitle->setAlign(pTK::Align::Left, pTK::Align::Top);

    // Buttons.
    pTK::Ref<CustomBtn> home = CreateCustomBtn("Home", margin, maxWidth);
    home->colorCopy = pTK::Color{0x272727FF};
    home->setColor(pTK::Color{0x272727FF});
    pTK::Ref<CustomBtn> projects = CreateCustomBtn("Projects", margin, maxWidth);
    pTK::Ref<CustomBtn> about = CreateCustomBtn("About", margin, maxWidth);
    pTK::Ref<CustomBtn> contact = CreateCustomBtn("Contact", margin, maxWidth);

    BtnManager btnManager{};
    btnManager.buttons.push_back(home);
    btnManager.buttons.push_back(projects);
    btnManager.buttons.push_back(about);
    btnManager.buttons.push_back(contact);

    home->onClick([&](const pTK::ClickEvent&){
        btnManager.setActive(home);
        return false;
    });
    projects->onClick([&](const pTK::ClickEvent&){
        btnManager.setActive(projects);
        return false;
    });
    about->onClick([&](const pTK::ClickEvent&){
        btnManager.setActive(about);
        return false;
    });
    contact->onClick([&](const pTK::ClickEvent&){
        btnManager.setActive(contact);
        return false;
    });

    pTK::Margin marginL{ marginTB * 4, marginLR + marginLR / 2, marginLR, marginLR };
    pTK::Ref<CustomBtn> close = CreateCustomBtn("Close", marginL, maxWidth);
    close->hoverColor = pTK::Color{0xDE2A33FF};
    close->clickColor = pTK::Color{0xB8232AFF};
    close->setAlign(pTK::Align::HCenter, pTK::Align::Bottom);
    close->onRelease([&](const pTK::ReleaseEvent&) {
        app.close();
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
    sidebar->add(close);

    hbox->add(sidebar);
    hbox->add(vline);

    // VBox as right side content. (TODO: should be a scrollable area).
    pTK::Ref<pTK::VBox> content = pTK::Create<pTK::VBox>();
    content->setAlign(pTK::Align::Left);
    content->setBackground(pTK::Color{0x1E1E1EFF});

    pTK::Ref<pTK::Label> cTitle = pTK::Create<pTK::Label>();
    cTitle->setText("Home");
    cTitle->setFontSize(26);
    cTitle->setMargin({18, 9, 18, 18});
    cTitle->setColor(pTK::Color{0xFEFEFEFF});
    cTitle->setAlign(pTK::Align::Left, pTK::Align::Top);

    pTK::Ref<pTK::Rectangle> hline = pTK::Create<pTK::Rectangle>();
    //hline->setColor(pTK::Color{0xF0F0F0FF});
    hline->setColor(RandomColor());
    hline->setSizePolicy(pTK::SizePolicy::Type::Expanding);
    hline->setMaxSize({pTK::Size::Limits::Max, 1});
    hline->setMargin({0, 0, 18, 18});
    hline->setAlign(pTK::Align::Left, pTK::Align::Top);

    pTK::Ref<pTK::Button> btn = pTK::Create<pTK::Button>(pTK::Button::Style::Default);
    btn->setAlign(pTK::Align::Left, pTK::Align::Top);
    btn->setText("Random Color!");
    btn->setMargin({ 18, 18, 18, 18 });
    btn->onClick([&hline](const pTK::ClickEvent&) {
        hline->setColor(RandomColor());
        return false;
    });

    pTK::Ref<pTK::TextField> textField = pTK::Create<pTK::TextField>();
    textField->setSize({400, 38});
    pTK::SizePolicy policy{pTK::SizePolicy::Policy::Expanding, pTK::SizePolicy::Policy::Fixed};
    textField->setSizePolicy(policy);
    textField->setMargin({ 18, 18, 40, 40 });
    textField->setFontSize(20);
    textField->setCornerRadius(2.5f);
    textField->setOutlineColor(pTK::Color{0x181818FF});
    textField->setOutlineThickness(1.5f);
    textField->setPlaceholderColor(pTK::Color{0xD4D4D4FF});
    textField->setTextColor(pTK::Color{0xF4F4F4FF});
    textField->setColor(pTK::Color{0x262626FF});
    textField->setPlaceholderText("Add text here!");
    std::cout << "Deafult font family: " << textField->getFontFamily() << std::endl;

    textField->onClick([&](const pTK::ClickEvent&){
        textField->setOutlineColor(pTK::Color{0x007BFFFF});
        return false;
    });
    textField->onLeaveClick([&](const pTK::LeaveClickEvent&){
        textField->setOutlineColor(pTK::Color{0x181818FF});
        return false;
    });

    pTK::Ref<SpinningRect> sRect = pTK::Create<SpinningRect>();
    sRect->setSize({250, 250});

    std::atomic<bool> sRun = true;
    std::thread sThread{[&](){
        while (sRun)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
            sRect->setDegrees(sRect->getDegrees() + 1.0f);
        }
    }};

    content->add(cTitle);
    content->add(hline);
    content->add(btn);
    content->add(textField);
    content->add(sRect);

    hbox->add(content);

    window.add(hbox);

    window.setMaxSize({1280, 720});
    // window.show();

    {
        pTK::Widget widget{};
        widget.addListener<int>([](){return false;});
    }


    // return app.exec(&window);
    auto ret = app.exec(&window);
    sRun = false;
    sThread.join();
    return ret;
}
