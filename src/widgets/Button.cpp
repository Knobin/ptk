//
//  widgets/Button.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-15.
//

// Local Headers
#include "ptk/widgets/Button.hpp"
#include "ptk/Core.hpp"

namespace pTK
{
    const Button::Style Button::Style::Default{Color(0x007BFFFF), Color(0x0071E6FF), Color(0x0068D9FF), Color(0xFFFFFFFF), 5};
    const Button::Style Button::Style::Success{Color(0x28A745FF), Color(0x24963CFF), Color(0x218838FF), Color(0xFFFFFFFF), 5};
    const Button::Style Button::Style::Danger{Color(0xDC3545FF), Color(0xce2331FF), Color(0xBC202DFF), Color(0xFFFFFFFF), 5};
    
    Button::Button()
        : Rectangle(), m_text{create<Label>()}, m_labelPos{}, m_borderSize{14},
            m_hoverColor{}, m_clickColor{}, m_colorCopy{}, m_hover{false}, m_click{false}
    {
        setStyle(Style::Default);
        m_text->setParent(this);
        m_text->setFontSize(14);
        setConstSize({});
    }
    
    Button::Button(const Style& style)
        : Rectangle(), m_text{create<Label>()}, m_labelPos{}, m_borderSize{14},
            m_hoverColor{}, m_clickColor{}, m_colorCopy{}, m_hover{false}, m_click{false}
    {
        setStyle(style);
        m_text->setParent(this);
        m_text->setFontSize(14);
        setConstSize({});
    }
    
    void Button::setPosHint(const Point& pos)
    {
        // TODO: Possibly wrong
        const Size textSize{m_text->getSize()};
        const Size rectSize{getSize()};
        Point textPos{pos};
        textPos.x += ((rectSize.width - textSize.width) / 2);
        textPos.y += ((rectSize.height - textSize.height) / 2);
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
                const int dMargin{static_cast<int>(static_cast<float>(m_borderSize*2.0f))};
                const int hMargin{((bSize.height - wSize.height) > dMargin) ? 0 : dMargin};
                const int wMargin{((bSize.width - wSize.width) > dMargin) ? 0 : dMargin};
                wSize.width = ((wSize.width > bSize.width) ? wSize.width : bSize.width) + (wMargin);
                wSize.height = ((wSize.height > bSize.height) ? wSize.height : bSize.height) + (hMargin);
                if (isConstSize())
                    setConstSize(wSize);
                else
                    setSize(wSize);
            }
            
            if (m_labelPos != textPos)
            {
                Point pos{getPosition()};
                const Size textSize{m_text->getSize()};
                pos.x += ((bSize.width - textSize.width) / 2);
                pos.y += ((bSize.height - textSize.height) / 2);
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
    
    void Button::setFontSize(uint fontSize)
    {
        // This will call draw in Label.
        // So we handle it in drawChild.
        m_text->setFontSize(fontSize);
        setBounds();
    }
    
    uint Button::getFontSize() const
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
    
    void Button::setLabel(const Ref<Label> label)
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
    
    void Button::setBorderSize(uint size)
    {
        m_borderSize = size;
        setBounds();
    }
    
    uint Button::getBorderSize() const
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
    
    bool Button::onEnterEvent()
    {
        m_colorCopy = getColor();
        setColor(m_hoverColor);
        m_hover = true;
        
        return true;
    }
    
    bool Button::onLeaveEvent()
    {
        if (!m_click)
            setColor(m_colorCopy);
        m_hover = false;
        
        return true;
    }
    
    bool Button::onClickEvent(Mouse::Button, const Point&)
    {
        setColor(m_clickColor);
        m_click = true;
        return true;
    }
    
    bool Button::onReleaseEvent(Mouse::Button, const Point&)
    {
        if (m_click)
        {
            if (m_hover)
                setColor(m_hoverColor);
            else
                setColor(m_colorCopy);
        }

        m_click = false;
        
        return true;
    }
    
    void Button::setBounds()
    {
        Size textBounds{m_text->getBounds()};
        textBounds.height += 2*m_borderSize;
        textBounds.width += 2*m_borderSize;

        if (!isConstSize())
            setMinSize(textBounds);

        Size cSize{getSize()};
        if ((textBounds.height > cSize.height) || (textBounds.width > cSize.width))
        {
            if (isConstSize())
                setConstSize(textBounds);
            else
                setSize(textBounds);
        }
    }
}
