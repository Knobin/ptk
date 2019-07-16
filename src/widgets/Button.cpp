//
//  widgets/Button.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-15.
//

// Local Headers
#include "ptk/widgets/Button.hpp"
#include "ptk/Log.hpp"

namespace pTK
{
    const Button::Style Button::Style::Default{Color(0x007BFFFF), Color(0x0071E6FF), Color(0x0068D9FF), Color(0xFFFFFFFF), 5};
    const Button::Style Button::Style::Success{Color(0x28A745FF), Color(0x24963CFF), Color(0x218838FF), Color(0xFFFFFFFF), 5};
    const Button::Style Button::Style::Danger{Color(0xDC3545FF), Color(0xce2331FF), Color(0xBC202DFF), Color(0xFFFFFFFF), 5};
    
    Button::Button()
        : Rectangle(), m_text{std::make_shared<Label>()}, m_labelPos{}, m_margin{15},
            m_hoverColor{}, m_clickColor{}, m_colorCopy{}, m_hover{false}, m_click{false}
    {
        setStyle(Style::Default);
        m_text->setParent(this);
        m_text->setFontSize(16);
    }
    
    Button::Button(const Style& style)
        : Rectangle(), m_text{std::make_shared<Label>()}, m_labelPos{}, m_margin{15},
            m_hoverColor{}, m_clickColor{}, m_colorCopy{}, m_hover{false}, m_click{false}
    {
        setStyle(style);
        m_text->setParent(this);
        m_text->setFontSize(16);
    }
    
    void Button::setPosHint(const Position& pos)
    {
        // TODO: Possibly wrong
        Size textSize = m_text->getSize();
        Size rectSize = getSize();
        Position textPos = pos;
        textPos.x += ((rectSize.width - textSize.width) / 2);
        textPos.y += ((rectSize.height - textSize.height) / 2);
        m_labelPos = textPos;
        Widget::setPosHint(pos);
        m_text->setPosHint(textPos);
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
            Size bSize = getSize();
            Size wSize = widget->getSize();
            Position textPos = widget->getPosition();
            if ((wSize.width > bSize.width) || (wSize.height > bSize.height))
            {
                // Apparently, this looks good.
                uint dMargin = (m_margin*2);
                uint hMargin = ((bSize.height - wSize.height) > dMargin) ? 0 : dMargin;
                uint wMargin = ((bSize.width - wSize.width) > dMargin) ? 0 : dMargin;
                wSize.width = ((wSize.width > bSize.width) ? wSize.width : bSize.width) + (wMargin);
                wSize.height = ((wSize.height > bSize.height) ? wSize.height : bSize.height) + (hMargin);
                Widget::setSize(wSize);
            }
            
            if (m_labelPos != textPos)
            {
                Position pos = getPosition();
                Size textSize = m_text->getSize();
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
    }
    
    const std::string& Button::getFontFamily() const
    {
        return m_text->getFontFamily();
    }
    
    void Button::setFontSize(uint fontSize)
    {
        // This will call draw in Label.
        // So we handle it in drawChild.
        m_text->setFontSize(fontSize);
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
    
    void Button::setLabel(const std::shared_ptr<Label> label)
    {
        PTK_ASSERT(label, "Label is nullptr");
        m_text->setParent(nullptr);
        m_text = label;
        m_text->setParent(this);
        draw();
    }
    
    std::shared_ptr<Label> Button::getLabel() const
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
    
    void Button::setMargin(uint margin)
    {
        m_margin = margin;
    }
    
    uint Button::getMargin() const
    {
        return m_margin;
    }
    
    void Button::setStyle(const Style& style)
    {
        m_hoverColor = style.hoverColor;
        m_clickColor = style.clickColor;
        m_text->setColor(style.textColor);
        setCornerRadius(style.cornerRadius);
        setColor(style.color); // Will call draw.
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
    
    bool Button::onClickEvent(MouseButton, const Position&)
    {
        setColor(m_clickColor);
        m_click = true;
        return true;
    }
    
    bool Button::onReleaseEvent(MouseButton, const Position&)
    {
        if (m_hover)
            setColor(m_hoverColor);
        else
            setColor(m_colorCopy);
        
        m_click = false;
        
        
        return true;
    }
}
