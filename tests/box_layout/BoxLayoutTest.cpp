//
//  tests/box_layout/BoxLayoutTest.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-04-08.
//

// pTK Headers
#include "ptk/widgets/BoxLayout.hpp"
#include "ptk/Application.hpp"
#include "ptk/core/Text.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WidgetEvents.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/SizePolicy.hpp"
#include "ptk/widgets/HBox.hpp"
#include "ptk/widgets/Label.hpp"

// C++ Headers
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>

class TestSquare : public pTK::Widget, public pTK::Text
{
public:
    TestSquare() = delete;
    TestSquare(std::string_view str, pTK::Color color, pTK::Size size)
        : pTK::Widget(),
          pTK::Text(),
          m_str{str},
          m_color{color}
    {
        updateSize(size);
        setSizePolicy(pTK::SizePolicy::Type::Fixed);
        setFontSize(18);
    }
    ~TestSquare() override = default;

    void onDraw(pTK::Canvas* canvas) override
    {
        const auto pos = getPosition();
        const auto size = getSize();

        canvas->drawRoundRect(pos, getSize(), m_color, 2.0f);
        const pTK::Text::StrData data{m_str.data(), m_str.size(), pTK::Text::Encoding::UTF8};
        const pTK::Vec2f textBounds{getBoundsFromStr(m_str)};
        pTK::Vec2f textPos{
            static_cast<float>(pos.x) + (static_cast<float>(size.width) / 2) - (static_cast<float>(textBounds.x) / 2),
            static_cast<float>(pos.y) + (static_cast<float>(size.height) / 2) - (static_cast<float>(textBounds.y) / 2),
        };
        canvas->drawTextLine(data, pTK::Color::MakeRGB(255, 255, 255), textPos, &skFont());
    }

private:
    std::string m_str{};
    pTK::Color m_color{};
};

constexpr std::string_view DirectionToStr(pTK::BoxLayout::Direction direction) noexcept
{
    constexpr auto lookup = std::array{"LeftToRight", "RightToLeft", "TopToBottom", "BottomToTop"};

    using dir_utype = std::underlying_type_t<pTK::BoxLayout::Direction>;
    return lookup[static_cast<decltype(lookup)::size_type>(static_cast<dir_utype>(direction))];
}

static pTK::Size ScaleSize(const pTK::Size& size, float x, float y)
{
    const float width{std::ceil(static_cast<float>(size.width) * x)};
    const float height{std::ceil(static_cast<float>(size.height) * y)};
    return {static_cast<pTK::Size::value_type>(width), static_cast<pTK::Size::value_type>(height)};
}

class Arrow : public pTK::Widget
{
public:
    enum class Direction : uint8_t
    {
        Up = 0x0001,
        Down = 0x0002,
        Left = 0x0004,
        Right = 0x0008
    };

public:
    Arrow(Direction direction, pTK::Color bg, pTK::Color color, pTK::Color hover, pTK::Color click)
        : m_color{color},
          m_backgroundColor{bg},
          m_hoverColor{hover},
          m_clickColor{click},
          m_activeColor{0},
          m_activeArrowColor{pTK::Color::MakeRGB(120, 120, 120)},
          m_direction{direction}
    {
        setSizePolicy(pTK::SizePolicy::Type::Fixed);
    }
    ~Arrow() override = default;

    void enable(bool enabled)
    {
        m_enabled = enabled;
        m_activeColor = (enabled) ? m_backgroundColor : pTK::Color{0};
        m_activeArrowColor = (enabled) ? m_color : pTK::Color::MakeRGB(120, 120, 120);
        draw();
    }
    [[nodiscard]] bool isEnabled() const { return m_enabled; }

    void onEnterEvent(const pTK::EnterEvent&) override
    {
        if (isEnabled())
        {
            m_isMouseOver = true;
            m_activeColor = m_hoverColor;
            draw();
        }
    }

    void onLeaveEvent(const pTK::LeaveEvent&) override
    {
        if (isEnabled())
        {
            m_isMouseOver = false;
            if (!m_clicked)
            {
                m_activeColor = m_backgroundColor;
                draw();
            }
        }
    }

    void onClickEvent(const pTK::ClickEvent&) override
    {
        if (isEnabled())
        {
            m_clicked = true;
            m_activeColor = m_clickColor;
            draw();
        }
    }

    void onReleaseEvent(const pTK::ReleaseEvent&) override
    {
        if (isEnabled())
        {
            m_clicked = false;
            m_activeColor = (m_isMouseOver) ? m_hoverColor : m_backgroundColor;
            draw();
        }
    }

    void onDraw(pTK::Canvas* canvas) override
    {
        // Size and pos to Skia variants.
        const SkPoint pos{convertToSkPoint(getPosition())};
        const SkPoint size{convertToSkPoint(ScaleSize(getSize(), 0.6f, 0.6f))};
        const SkPoint sizeOffset{convertToSkPoint(ScaleSize(getSize(), 0.2f, 0.2f))};

        const SkPoint topCenter{pos};
        const auto thickness = static_cast<SkScalar>(size.fY * 0.1f);
        const auto halfThickness = static_cast<SkScalar>(thickness * 0.5f);

        // Angled rect (45).
        const auto angledHeight = static_cast<SkScalar>(size.fY * 0.6f);
        SkPoint angledSize1{thickness, angledHeight};
        angledSize1 += topCenter;
        SkRect aRect1{};
        aRect1.set(topCenter, angledSize1);

        // Angled rect (-45).
        SkPoint aPos = {topCenter.fX - thickness, topCenter.fY};
        SkPoint angledSize2{thickness, angledHeight};
        angledSize2 += aPos;
        SkRect aRect2{};
        aRect2.set(aPos, angledSize2);

        // Set Color
        SkPaint paint{};
        paint.setAntiAlias(true);
        paint.setARGB(m_activeArrowColor.a, m_activeArrowColor.r, m_activeArrowColor.g, m_activeArrowColor.b);

        // Center rect.
        const auto cHeight = static_cast<SkScalar>(size.fY * 0.9f);
        SkPoint cPos{topCenter.fX - halfThickness, topCenter.fY + (size.fY - cHeight)};
        SkPoint cSize{thickness, cHeight};
        cSize += cPos;
        SkRect rect{};
        rect.set(cPos, cSize);

        SkCanvas* skCanvas = canvas->skCanvas;

        if (m_activeColor.a > 0)
        {
            // Background here.
            SkPoint tSize{convertToSkPoint(getSize())};
            tSize += pos;
            SkRect tRect{};
            tRect.set(pos, tSize);
            SkPaint tPaint{};
            tPaint.setAntiAlias(true);
            tPaint.setARGB(m_activeColor.a, m_activeColor.r, m_activeColor.g, m_activeColor.b);
            skCanvas->drawRoundRect(tRect, 4, 4, tPaint);
        }

        skCanvas->save();

        const SkScalar degreesOffset{directionDegreesOffset(m_direction)};
        const SkPoint offset = directionOffset(m_direction, size) + sizeOffset;
        skCanvas->translate(offset.fX, offset.fY);

        skCanvas->save();
        skCanvas->rotate(degreesOffset, topCenter.fX, topCenter.fY);
        skCanvas->drawRoundRect(rect, 2, 2, paint);
        skCanvas->restore();

        skCanvas->save();
        skCanvas->rotate(45 + degreesOffset, topCenter.fX, topCenter.fY);
        skCanvas->drawRoundRect(aRect1, 2, 2, paint);
        skCanvas->restore();

        skCanvas->save();
        skCanvas->rotate(-45 + degreesOffset, topCenter.fX, topCenter.fY);
        skCanvas->drawRoundRect(aRect2, 2, 2, paint);
        skCanvas->restore();

        skCanvas->restore();
    }

private:
    [[nodiscard]] static SkScalar directionDegreesOffset(Direction direction) noexcept
    {
        switch (direction)
        {
            case Direction::Up:
                return 0;
            case Direction::Down:
                return 180;
            case Direction::Left:
                return 270;
            case Direction::Right:
                return 90;
        }

        return 0;
    }

    [[nodiscard]] static SkPoint directionOffset(Direction direction, const SkPoint& size) noexcept
    {
        switch (direction)
        {
            case Direction::Up:
                return {static_cast<SkScalar>(size.fX / 2), 0};
            case Direction::Down:
                return {static_cast<SkScalar>(size.fX / 2), size.fY};
            case Direction::Left:
                return {0, static_cast<SkScalar>(size.fY / 2)};
            case Direction::Right:
                return {size.fX, static_cast<SkScalar>(size.fY / 2)};
        }

        return {0, 0};
    }

private:
    pTK::Color m_color{};
    pTK::Color m_backgroundColor{0};
    pTK::Color m_hoverColor{0};
    pTK::Color m_clickColor{0};
    pTK::Color m_activeColor{};
    pTK::Color m_activeArrowColor{0};
    Direction m_direction;
    bool m_enabled{false};
    bool m_isMouseOver{false};
    bool m_clicked{false};
};

void ApplyDirection(std::shared_ptr<pTK::BoxLayout>& layout, pTK::BoxLayout::Direction direction,
                    std::shared_ptr<Arrow>& left, std::shared_ptr<Arrow>& right, std::shared_ptr<pTK::Label>& label)
{
    std::cout << "Changing layout direction to \"" << DirectionToStr(direction) << "\"." << std::endl;
    label->setText(std::string{DirectionToStr(direction)});
    layout->setDirection(direction);

    if (direction == pTK::BoxLayout::Direction::LeftToRight)
    {
        left->enable(false);
        right->enable(true);
    }
    else if (direction == pTK::BoxLayout::Direction::BottomToTop)
    {
        left->enable(true);
        right->enable(false);
    }
    else
    {
        left->enable(true);
        right->enable(true);
    }
}

int main(int argc, char* argv[])
{
    pTK::Application app("BoxLayout Test Application", argc, argv);
    pTK::Window window("BoxLayout Test Window", {960, 540});
    window.setBackground(pTK::Color::MakeHex(0x1F1F1F));

    auto topBar = std::make_shared<pTK::HBox>();
    topBar->setMaxSize({pTK::Size::Limits::Max, 20});
    topBar->setBackground(window.getBackground());
    // topBar->setBackground(pTK::Color::MakeRGB(123, 21, 132));

    // Arrow Colors.
    constexpr auto ArrowBackground = pTK::Color::MakeHex(0x3F3F3F);
    constexpr auto ArrowColor = pTK::Color::MakeHex(0xF1F2F3);
    constexpr auto ArrowHover = pTK::Color::MakeHex(0x5F5F5F);
    constexpr auto ArrowClick = pTK::Color::MakeHex(0x2F2F2F);

    // (Direction direction, pTK::Color bg, pTK::Color color, pTK::Color hover)
    auto left = std::make_shared<Arrow>(Arrow::Direction::Left, ArrowBackground, ArrowColor, ArrowHover, ArrowClick);
    left->setSize({28, 28});
    left->setMarginLeftRight(200, 50);
    left->setAlign(pTK::Align::Left, pTK::Align::VCenter);
    topBar->add(left);

    auto label = std::make_shared<pTK::Label>();
    label->setText("LeftToRight");
    label->setFontSize(24);
    label->setAlign(pTK::Align::Center);
    label->setMarginTopBottom(10, 10);
    topBar->add(label);

    auto right = std::make_shared<Arrow>(Arrow::Direction::Right, ArrowBackground, ArrowColor, ArrowHover, ArrowClick);
    right->setSize({28, 28});
    right->setMarginLeftRight(50, 200);
    right->setAlign(pTK::Align::Right, pTK::Align::VCenter);
    right->enable(true);
    topBar->add(right);

    window.add(topBar);

    auto layout = std::make_shared<pTK::BoxLayout>(pTK::BoxLayout::Direction::LeftToRight);
    layout->setBackground(window.getBackground());
    layout->add(std::make_shared<TestSquare>("1", pTK::Color::MakeRGB(200, 100, 100), pTK::Size{50, 50}));
    layout->add(std::make_shared<TestSquare>("2", pTK::Color::MakeRGB(100, 200, 100), pTK::Size{50, 50}));
    layout->add(std::make_shared<TestSquare>("3", pTK::Color::MakeRGB(100, 100, 200), pTK::Size{50, 50}));
    window.add(layout);

    auto CurrentDirectionIndex =
        static_cast<std::underlying_type_t<pTK::BoxLayout::Direction>>(pTK::BoxLayout::Direction::LeftToRight);

    left->onClick([&](const pTK::ClickEvent&) {
        if (left->isEnabled())
        {
            if (CurrentDirectionIndex > 0)
            {
                CurrentDirectionIndex--;
                ApplyDirection(layout, static_cast<pTK::BoxLayout::Direction>(CurrentDirectionIndex), left, right,
                               label);
            }
        }
        return false;
    });

    right->onClick([&](const pTK::ClickEvent&) {
        if (right->isEnabled())
        {
            if (CurrentDirectionIndex < 3)
            {
                CurrentDirectionIndex++;
                ApplyDirection(layout, static_cast<pTK::BoxLayout::Direction>(CurrentDirectionIndex), left, right,
                               label);
            }
        }
        return false;
    });

    window.onKey([&](const pTK::KeyEvent& evt) {
        using bld = pTK::BoxLayout::Direction;
        const auto lookup = std::map<pTK::Key, bld>{{pTK::Key::D1, bld::LeftToRight},
                                                    {pTK::Key::D2, bld::RightToLeft},
                                                    {pTK::Key::D3, bld::TopToBottom},
                                                    {pTK::Key::D4, bld::BottomToTop}};

        auto it = std::find_if(lookup.cbegin(), lookup.cend(), [&](const auto& pair) {
            return pair.first == evt.keycode;
        });

        if (it != lookup.cend() && it->second != layout->direction())
        {
            CurrentDirectionIndex = static_cast<std::underlying_type_t<bld>>(it->second);
            ApplyDirection(layout, it->second, left, right, label);
        }

        return false;
    });

    window.show();
    return app.exec(&window);
}
