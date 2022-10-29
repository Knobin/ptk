//
//  widgets/Button.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-15.
//

// pTK Headers
#include "ptk/widgets/Button.hpp"
#include "ptk/Core.hpp"
#include "ptk/util/Math.hpp"

namespace pTK
{
    const Button::Style Button::Style::Default{Color(0x007BFFFF), Color(0x0071E6FF), Color(0x0068D9FF),
                                               Color(0xFFFFFFFF), 5.0f};
    const Button::Style Button::Style::Success{Color(0x28A745FF), Color(0x24963CFF), Color(0x218838FF),
                                               Color(0xFFFFFFFF), 5.0f};
    const Button::Style Button::Style::Danger{Color(0xDC3545FF), Color(0xce2331FF), Color(0xBC202DFF),
                                              Color(0xFFFFFFFF), 5.0f};

    Button::Button()
        : Rectangle(),
          m_text{Create<Label>()},
          m_labelPos{},
          m_borderSize{14},
          m_hoverColor{},
          m_clickColor{},
          m_colorCopy{},
          m_hover{false},
          m_click{false}
    {
        initCallbacks();

        setStyle(Style::Default);
        m_text->setParent(this);
        m_text->setFontSize(14);
    }

    Button::Button(const Style& style)
        : Rectangle(),
          m_text{Create<Label>()},
          m_labelPos{},
          m_borderSize{14},
          m_hoverColor{},
          m_clickColor{},
          m_colorCopy{},
          m_hover{false},
          m_click{false}
    {
        initCallbacks();

        setStyle(style);
        m_text->setParent(this);
        m_text->setFontSize(14);
    }

    void Button::initCallbacks()
    {
        addListener<EnterEvent>([&](const EnterEvent& evt) {
            onEnterCallback(evt);
            return false;
        });
        addListener<LeaveEvent>([&](const LeaveEvent& evt) {
            onLeaveCallback(evt);
            return false;
        });
        addListener<ClickEvent>([&](const ClickEvent& evt) {
            onClickCallback(evt);
            return false;
        });
        addListener<ReleaseEvent>([&](const ReleaseEvent& evt) {
            onReleaseCallback(evt);
            return false;
        });
    }

    void Button::setPosHint(const Point& pos)
    {
        // TODO: Possibly wrong
        const Size textSize{m_text->getSize()};
        const Size rectSize{getSize()};
        Point textPos{pos};
        const float x = static_cast<float>(rectSize.width - textSize.width) / 2.0f;
        const float y = static_cast<float>(rectSize.height - textSize.height) / 2.0f;
        textPos.x += static_cast<Point::value_type>(x);
        textPos.y += static_cast<Point::value_type>(y);
        m_labelPos = textPos;
        Widget::setPosHint(pos);
        m_text->setPosHint(textPos);
    }

    void Button::onSizeChange(const Size&)
    {
        setBounds();
    }

    void Button::onDraw(SkCanvas* canvas)
    {
        Rectangle::onDraw(canvas);
        m_text->onDraw(canvas);
    }

    bool Button::drawChild(Widget* widget)
    {
        if (m_text.get() == widget)
        {
            const Size bSize = getSize();
            Size wSize = widget->getSize();
            const Point textPos = widget->getPosition();
            if ((wSize.width > bSize.width) || (wSize.height > bSize.height))
            {
                // Apparently, this looks good.
                const Size::value_type dMargin{static_cast<Size::value_type>(static_cast<float>(m_borderSize) * 2.0f)};
                const Size::value_type hMargin{((bSize.height - wSize.height) > dMargin) ? 0 : dMargin};
                const Size::value_type wMargin{((bSize.width - wSize.width) > dMargin) ? 0 : dMargin};
                wSize.width = ((wSize.width > bSize.width) ? wSize.width : bSize.width) + (wMargin);
                wSize.height = ((wSize.height > bSize.height) ? wSize.height : bSize.height) + (hMargin);
                setSize(wSize);
            }

            if (m_labelPos != textPos)
            {
                Point pos{getPosition()};
                const Size textSize{m_text->getSize()};

                const float x = static_cast<float>(bSize.width - textSize.width) / 2.0f;
                const float y = static_cast<float>(bSize.height - textSize.height) / 2.0f;
                pos.x += static_cast<Point::value_type>(x);
                pos.y += static_cast<Point::value_type>(y);

                m_labelPos = pos;
                m_text->setPosHint(pos);
            }

            draw();
            return true;
        }

        return false;
    }

    void Button::setText(const std::string& text)
    {
        // This will call draw in Label.
        // So we handle it in drawChild.
        m_text->setText(text);
        setBounds();
    }

    const std::string& Button::getText() const
    {
        return m_text->getText();
    }

    void Button::setFontFamily(const std::string& fontFamily)
    {
        // This will call draw in Label.
        // So we handle it in drawChild.
        m_text->setFontFamily(fontFamily);
        setBounds();
    }

    std::string Button::getFontFamily() const
    {
        return m_text->getFontFamily();
    }

    void Button::setFontSize(uint32_t fontSize)
    {
        // This will call draw in Label.
        // So we handle it in drawChild.
        m_text->setFontSize(fontSize);
        setBounds();
    }

    uint32_t Button::getFontSize() const
    {
        return m_text->getFontSize();
    }

    void Button::setTextColor(const Color& textColor)
    {
        // This will call draw in Label.
        // So we handle it in drawChild.
        m_text->setColor(textColor);
    }

    const Color& Button::getTextColor() const
    {
        return m_text->getColor();
    }

    void Button::setLabel(const Ref<Label>& label)
    {
        PTK_ASSERT(label, "Label is nullptr");
        m_text->setParent(nullptr);
        m_text = label;
        m_text->setParent(this);
        setBounds();
        draw();
    }

    Ref<Label> Button::getLabel() const
    {
        return m_text;
    }

    void Button::setHoverColor(const Color& hoverColor)
    {
        m_hoverColor = hoverColor;
    }

    const Color& Button::getHoverColor() const
    {
        return m_hoverColor;
    }

    void Button::setClickColor(const Color& clickColor)
    {
        m_clickColor = clickColor;
    }

    const Color& Button::getClickColor() const
    {
        return m_clickColor;
    }

    void Button::setBorderSize(uint32_t size)
    {
        m_borderSize = size;
        setBounds();
    }

    uint32_t Button::getBorderSize() const
    {
        return m_borderSize;
    }

    void Button::setStyle(const Style& style)
    {
        m_hoverColor = style.hoverColor;
        m_clickColor = style.clickColor;
        m_text->setColor(style.textColor);
        setCornerRadius(style.cornerRadius);
        setColor(style.color); // Will call draw.
        setBounds();
    }

    Button::Style Button::getStyle() const
    {
        Style style{};
        style.color = getColor();
        style.hoverColor = m_hoverColor;
        style.clickColor = m_clickColor;
        style.textColor = m_text->getColor();
        style.cornerRadius = getCornerRadius();
        return style;
    }

    void Button::onEnterCallback(const EnterEvent&)
    {
        m_colorCopy = getColor();
        setColor(m_hoverColor);
        m_hover = true;
    }

    void Button::onLeaveCallback(const LeaveEvent&)
    {
        if (!m_click)
            setColor(m_colorCopy);
        m_hover = false;
    }

    void Button::onClickCallback(const ClickEvent&)
    {
        setColor(m_clickColor);
        m_click = true;
    }

    void Button::onReleaseCallback(const ReleaseEvent&)
    {
        if (m_click)
        {
            if (m_hover)
                setColor(m_hoverColor);
            else
                setColor(m_colorCopy);
        }

        m_click = false;
    }

    void Button::setBounds()
    {
        Vec2f textBounds{m_text->getBoundsFromStr(getText())};
        textBounds.y += 2.0f * static_cast<float>(m_borderSize);
        textBounds.x += 2.0f * static_cast<float>(m_borderSize);

        const Size sizeBounds = Vec2ToSize(textBounds, Math::ceilf);
        setMinSize(sizeBounds);

        Size cSize{getSize()};
        if ((sizeBounds.height > cSize.height) || (sizeBounds.width > cSize.width))
            setSize(sizeBounds);
    }
} // namespace pTK
