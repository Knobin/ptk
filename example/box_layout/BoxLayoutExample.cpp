//
//  example/box_layout/BoxLayoutExample.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-04-08.
//

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/widgets/BoxLayout.hpp"
#include "ptk/widgets/HBox.hpp"
#include "ptk/widgets/Label.hpp"

// C++ Headers
#include <iostream>
#include <memory>
#include <type_traits>

// NumberRect, paints a rectangle with the text in the middle.
// Expects the provided str to fit inside the provided size.
class NumberRect : public pTK::Widget, public pTK::Text
{
public:
    NumberRect() = delete;
    NumberRect(std::string_view str, pTK::Color color, pTK::Size size)
        : pTK::Widget(),
          pTK::Text(),
          m_str{str},
          m_color{color}
    {
        updateSize(size);                            // Set size as current size (no callback version).
        setSizePolicy(pTK::SizePolicy::Type::Fixed); // Size can't be auto resized.
        setFontSize(18);
    }
    ~NumberRect() override = default;

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

// ArrowRect, paints a rectangle with an arrow inside pointing to the provided direction.
class ArrowRect : public pTK::Widget
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
    ArrowRect(Direction direction, pTK::Color bg, pTK::Color color, pTK::Color hover, pTK::Color click)
        : m_arrowColor{color},
          m_backgroundColor{bg},
          m_hoverColor{hover},
          m_clickColor{click},
          m_activeColor{0},
          m_activeArrowColor{pTK::Color::MakeRGB(120, 120, 120)},
          m_direction{direction}
    {
        setSizePolicy(pTK::SizePolicy::Type::Fixed);
    }
    ~ArrowRect() override = default;

    void enable(bool enabled)
    {
        m_enabled = enabled;
        m_activeColor = (enabled) ? m_backgroundColor : pTK::Color{0};
        m_activeArrowColor = (enabled) ? m_arrowColor : pTK::Color::MakeRGB(120, 120, 120);
        draw();
    }
    [[nodiscard]] bool isEnabled() const { return m_enabled; }

    void onEnterEvent(const pTK::EnterEvent&) override
    {
        m_isMouseOver = true;
        if (isEnabled())
        {
            m_activeColor = m_hoverColor;
            draw();
        }
    }

    void onLeaveEvent(const pTK::LeaveEvent&) override
    {
        m_isMouseOver = false;
        if (isEnabled())
        {
            if (!m_clicked)
            {
                m_activeColor = m_backgroundColor;
                draw();
            }
        }
    }

    void onClickEvent(const pTK::ClickEvent&) override
    {
        m_clicked = true;
        if (isEnabled())
        {
            m_activeColor = m_clickColor;
            draw();
        }
    }

    void onReleaseEvent(const pTK::ReleaseEvent&) override
    {
        m_clicked = false;
        if (isEnabled())
        {
            m_activeColor = (m_isMouseOver) ? m_hoverColor : m_backgroundColor;
            draw();
        }
    }

    void onDraw(pTK::Canvas* canvas) override
    {
        // Draw background if needed.
        if (m_activeColor.a > 0)
            canvas->drawRoundRect(getPosition(), getSize(), m_activeColor, 4.0f);

        const pTK::Vec2f arrowSize{static_cast<float>(getSize().width) * 0.6f,
                                   static_cast<float>(getSize().height) * 0.6f};
        const pTK::Vec2f arrowSizeOffset{static_cast<float>(getSize().width) * 0.2f,
                                         static_cast<float>(getSize().height) * 0.2f};
        const pTK::Vec2f topCenter{static_cast<float>(getPosition().x), static_cast<float>(getPosition().y)};
        const auto thickness = arrowSize.y * 0.1f;
        const auto halfThickness = thickness * 0.5f;
        const auto angledHeight = arrowSize.y * 0.6f;

        canvas->save();

        // Set top point (where all rectangles meet) as offset.
        const SkScalar degreesOffset{directionDegreesOffset(m_direction)};
        const pTK::Vec2f offset = directionOffset(m_direction, arrowSize) + arrowSizeOffset;
        canvas->translate(offset.x, offset.y);

        // Center rect.
        {
            canvas->save();

            const auto cHeight = arrowSize.y * 0.9f;

            pTK::Vec2f pos = topCenter;
            pos.x -= halfThickness;
            pos.y += (arrowSize.y - cHeight) / 2.0f;

            const pTK::Vec2f size{thickness, cHeight};

            canvas->rotate(degreesOffset, topCenter.x, topCenter.y);
            canvas->drawRoundRect(pos, size, m_activeArrowColor, 2.0f);

            canvas->restore();
        }

        // Angled rect (45).
        {
            canvas->save();

            const pTK::Vec2f pos = topCenter;
            const pTK::Vec2f size{thickness, angledHeight};

            canvas->rotate(45 + degreesOffset, topCenter.x, topCenter.y);
            canvas->drawRoundRect(pos, size, m_activeArrowColor, 2.0f);

            canvas->restore();
        }

        // Angled rect (-45).
        {
            canvas->save();

            const pTK::Vec2f pos{topCenter.x - thickness, topCenter.y};
            const pTK::Vec2f size{thickness, angledHeight};

            canvas->rotate(-45 + degreesOffset, topCenter.x, topCenter.y);
            canvas->drawRoundRect(pos, size, m_activeArrowColor, 2.0f);

            canvas->restore();
        }

        canvas->restore();
    }

private:
    [[nodiscard]] static float directionDegreesOffset(Direction direction) noexcept
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

    [[nodiscard]] static pTK::Vec2f directionOffset(Direction direction, const pTK::Vec2f size) noexcept
    {
        switch (direction)
        {
            case Direction::Up:
                return {size.x / 2.0f, 0.0f};
            case Direction::Down:
                return {size.x / 2.0f, size.y};
            case Direction::Left:
                return {0, size.y / 2.0f};
            case Direction::Right:
                return {size.x, size.y / 2.0f};
        }

        return {0.0f, 0.0f};
    }

private:
    pTK::Color m_arrowColor{};
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

// Converts the Direction to its string version.
constexpr std::string_view DirectionToStr(pTK::BoxLayout::Direction direction) noexcept
{
    constexpr auto lookup = std::array{"LeftToRight", "RightToLeft", "TopToBottom", "BottomToTop"};

    using dir_utype = std::underlying_type_t<pTK::BoxLayout::Direction>;
    return lookup[static_cast<decltype(lookup)::size_type>(static_cast<dir_utype>(direction))];
}

// Applies the layout direction to layout and enables/disables the appropriate arrows.
void ApplyDirection(std::shared_ptr<pTK::BoxLayout>& layout, pTK::BoxLayout::Direction direction,
                    std::shared_ptr<ArrowRect>& left, std::shared_ptr<ArrowRect>& right,
                    std::shared_ptr<pTK::Label>& label)
{
    std::cout << "Changing layout direction to \"" << DirectionToStr(direction) << "\"." << std::endl;
    label->setText(std::string{DirectionToStr(direction)});
    layout->setDirection(direction);

    if (direction == pTK::BoxLayout::Direction::LeftToRight)
    {
        // LeftToRight is the lowest possible index.
        left->enable(false); // Decrement is not allowed here.
        right->enable(true); // Increment is allowed here.
    }
    else if (direction == pTK::BoxLayout::Direction::BottomToTop)
    {
        // BottomToTop is the highest possible index.
        left->enable(true);   // Decrement is allowed here.
        right->enable(false); // Increment is not allowed here.
    }
    else
    {
        // Both inc and dec is allowed here.
        left->enable(true);
        right->enable(true);
    }
}

int main(int argc, char* argv[])
{
    pTK::Application app("BoxLayout Example Application", argc, argv);
    pTK::Window window("BoxLayout Example Window", {960, 540});
    window.setBackground(pTK::Color::MakeHex(0x1F1F1F));

    // TopBar is for changing layout direction and displaying current direction.
    auto topBar = std::make_shared<pTK::HBox>();
    topBar->setMaxSize({pTK::Size::Limits::Max, 20});
    topBar->setBackground(window.getBackground());
    // topBar->setBackground(pTK::Color::MakeRGB(123, 21, 132));

    // Arrow Colors.
    constexpr auto ArrowBackground = pTK::Color::MakeHex(0x3F3F3F);
    constexpr auto ArrowColor = pTK::Color::MakeHex(0xF1F2F3);
    constexpr auto ArrowHover = pTK::Color::MakeHex(0x5F5F5F);
    constexpr auto ArrowClick = pTK::Color::MakeHex(0x2F2F2F);

    // Left arrow (decrements the layout direction).
    auto left =
        std::make_shared<ArrowRect>(ArrowRect::Direction::Left, ArrowBackground, ArrowColor, ArrowHover, ArrowClick);
    left->setSize({28, 28});
    left->setMarginLeftRight(200, 50);
    left->setAlign(pTK::Align::Left, pTK::Align::VCenter);
    topBar->add(left);

    // Center label displays the current layout direction setting.
    auto label = std::make_shared<pTK::Label>();
    label->setText("LeftToRight");
    label->setFontSize(24);
    label->setAlign(pTK::Align::Center);
    label->setMarginTopBottom(10, 10);
    topBar->add(label);

    // Right arrow (increments the layout direction).
    auto right =
        std::make_shared<ArrowRect>(ArrowRect::Direction::Right, ArrowBackground, ArrowColor, ArrowHover, ArrowClick);
    right->setSize({28, 28});
    right->setMarginLeftRight(50, 200);
    right->setAlign(pTK::Align::Right, pTK::Align::VCenter);
    right->enable(true);
    topBar->add(right);

    // TopBar content has been added, now add TopBar to window.
    window.add(topBar);

    // Layout container contains 3 simple widgets TestSquare with default layout LeftToRight.
    auto layout = std::make_shared<pTK::BoxLayout>(pTK::BoxLayout::Direction::LeftToRight);
    layout->setBackground(window.getBackground());
    layout->add(std::make_shared<NumberRect>("1", pTK::Color::MakeRGB(200, 100, 100), pTK::Size{50, 50}));
    layout->add(std::make_shared<NumberRect>("2", pTK::Color::MakeRGB(100, 200, 100), pTK::Size{50, 50}));
    layout->add(std::make_shared<NumberRect>("3", pTK::Color::MakeRGB(100, 100, 200), pTK::Size{50, 50}));
    window.add(layout);

    // Changing layout direction is done by inc/dec the current index.
    using BTU = pTK::BoxLayout::Direction;
    auto CurrentDirectionIndex = static_cast<std::underlying_type_t<BTU>>(BTU::LeftToRight);

    // Left arrow clicked! Decrement the index if possible and set new value to layout.
    left->onClick([&](const pTK::ClickEvent&) {
        if (left->isEnabled())
        {
            if (CurrentDirectionIndex > 0)
            {
                CurrentDirectionIndex--;
                const auto dir{static_cast<pTK::BoxLayout::Direction>(CurrentDirectionIndex)};
                ApplyDirection(layout, dir, left, right, label);
            }
        }
        return false;
    });

    // Right arrow clicked! Decrement the index if possible and set new value to layout.
    right->onClick([&](const pTK::ClickEvent&) {
        if (right->isEnabled())
        {
            if (CurrentDirectionIndex < 3)
            {
                CurrentDirectionIndex++;
                const auto dir{static_cast<pTK::BoxLayout::Direction>(CurrentDirectionIndex)};
                ApplyDirection(layout, dir, left, right, label);
            }
        }
        return false;
    });

    // Keys [1, 4] can be used to switch layout direction.
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

    // Initialization is done! Run the application.
    window.show();
    return app.exec(&window);
}
