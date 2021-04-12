//
//  widgets/TextField.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

// pTK Headers
#include "ptk/widgets/TextField.hpp"

// C++ Headers
#include <cctype>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkFontMetrics.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    TextField::TextField()
        : Rectangle(), Text(), m_cursor{}
    {
        m_cursor.setColor(Color{0x495057FF});

        onKey([this](Event::Type type, KeyCode keycode, byte modifier){
            if (type == Event::Type::KeyPressed)
            {
                if (IsKeyCodeAlpha(keycode))
                {
                    bool shift = IsKeyEventModifierSet(modifier, KeyEvent::Modifier::Shift);
                    bool capsLock = IsKeyEventModifierSet(modifier, KeyEvent::Modifier::CapsLock);
                    char alpha = KeyCodeToAlpha(keycode);
              
                    if ((capsLock && !shift) || (!capsLock && shift))
                        setText(getText() + alpha);
                    else
                        setText(getText() + static_cast<char>(std::tolower(alpha)));
                }
                else if (IsKeyCodeDigit(keycode))
                    setText(getText() + KeyCodeToGraph(keycode));
                else if (IsKeyCodeSpace(keycode))
                    setText(getText() + ' ');
                else if (keycode == Key::Backspace)
                    setText(getText().substr(0, getText().size() - 1));
            }

                
            return false;
        });

        onClick([this](Mouse::Button, const Point&){
            m_drawCursor = true;
            return false;
        });

        onLeaveClick([this](){
            m_drawCursor = false;
            return false;
        });
    }

    void TextField::onDraw(SkCanvas* canvas)
    {
        Rectangle::onDraw(canvas);
        
        const Size rectSize{getSize()};
        
        float advance = (!getText().empty()) ? drawText(canvas, getText(), Color{0x495057FF}, m_textPos) : 0;
        
        PTK_INFO("DRAW POS: {}x{}", m_textPos.x, m_textPos.y);
        
        if (getText().empty())
            drawText(canvas, m_placeholderText, Color{0x6D757DFF}, m_textPos);

        if (m_drawCursor)
        {
            Point cursorPos{};
            cursorPos.x = static_cast<Point::value_type>(m_textPos.x + static_cast<Point::value_type>(std::ceil(advance)));
            cursorPos.y = getPosition().y + ((rectSize.height - m_cursor.getSize().height) / 2);

            m_cursor.setPosHint(cursorPos);
            m_cursor.onDraw(canvas);
        }
    }

    void TextField::onTextUpdate()
    {
        updateBounds();
    }

    void TextField::setPosHint(const Point& pos)
    {
        Widget::setPosHint(pos);
        updateBounds();
    }

    void TextField::updateBounds()
    {
        SkFontMetrics metrics{};
        skFont()->getMetrics(&metrics);
        m_totalTextHeight = std::abs(metrics.fAscent - metrics.fDescent);
        m_baseToAscent = m_totalTextHeight - std::abs(metrics.fDescent);
        PTK_INFO("m_totalTextHeight: {}, m_baseToAscent: {}", m_totalTextHeight, m_baseToAscent);
        
        const Size rectSize{getSize()};
        m_textPos.x = getPosition().x + ((rectSize.height - m_baseToAscent) / 2) - 2;
        m_textPos.y = getPosition().y + ((rectSize.height - m_baseToAscent) / 2);
        PTK_INFO("UPDATE POS: {}x{}", m_textPos.x, m_textPos.y);
        
        Size cursorSize{1, static_cast<Size::value_type>(m_totalTextHeight)};
        m_cursor.setSize(cursorSize);
        
        Size minSize{getBounds()};
        minSize.width += static_cast<Point::value_type>(cursorSize.width);
        minSize.height = (minSize.height > cursorSize.height) ? minSize.height : cursorSize.height;
        
        SkRect placeholderBounds{};
        skFont()->measureText(m_placeholderText.c_str(), m_placeholderText.size(), SkTextEncoding::kUTF8, &placeholderBounds);
        Size::value_type pWidth = static_cast<Size::value_type>(std::ceil(placeholderBounds.width()));
        Size::value_type pHeight = static_cast<Size::value_type>(std::ceil(placeholderBounds.height()));
        
        minSize.width = (minSize.width > pWidth) ? minSize.width : pWidth;
        minSize.height = (minSize.height > pHeight) ? minSize.height : pHeight;
        
        setMinSize(minSize);
        update();
    }

    void TextField::setPlaceholderText(const std::string& text)
    {
        m_placeholderText = text;
        updateBounds();
    }

    const std::string& TextField::getPlaceholderText() const
    {
        return m_placeholderText;
    }
}
