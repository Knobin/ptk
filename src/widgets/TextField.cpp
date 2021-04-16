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
        : Rectangle(), Text()
    {
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
        
        float advance = (!getText().empty()) ? drawTextLine(canvas, getText(), m_textColor, m_textPos) : 0.0f;
        
        if (getText().empty())
            drawTextLine(canvas, m_placeholderText, m_placeholderColor, m_textPos);

        if (m_drawCursor)
        {
            SkPaint paint{GetSkPaintFromColor(m_textColor)};
            paint.setStrokeWidth(1.0f);

            float posX = m_textPos.x + advance + ((getText().empty()) ? - 2.0f : 0.0f);
            float startY = static_cast<float>(getPosition().y)  + ((static_cast<float>(rectSize.height) - m_cursorHeight) / 2);
            float endY = startY + m_cursorHeight;
            canvas->drawLine({posX, startY}, {posX, endY}, paint);
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
        const float capitalHeight{capHeight()};
        const Size rectSize{getSize()};
        const Point pos{getPosition()};

        float fHeight{static_cast<float>(rectSize.height)};
        float center{(fHeight - capitalHeight) / 2};
        m_textPos.x = static_cast<float>(pos.x) + center;
        m_textPos.y = static_cast<float>(pos.y) + center;
        
        m_cursorHeight = ascentToDescent();
        
        Size minSize{getBounds()};
        minSize.width += 1;
        auto ceilCursorHeight = static_cast<Point::value_type>(std::ceil(m_cursorHeight));
        minSize.height = (minSize.height > ceilCursorHeight) ? minSize.height : ceilCursorHeight;
        
        SkRect placeholderBounds{};
        skFont()->measureText(m_placeholderText.c_str(), m_placeholderText.size(), SkTextEncoding::kUTF8, &placeholderBounds);
        auto pWidth = static_cast<Size::value_type>(std::ceil(placeholderBounds.width()));
        auto pHeight = static_cast<Size::value_type>(std::ceil(placeholderBounds.height()));
        
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

    void TextField::setPlaceholderColor(const Color& color)
    {
        m_placeholderColor = color;
        update();
    }

    const Color& TextField::getPlaceholderColor() const
    {
        return m_placeholderColor;
    }

    void TextField::setTextColor(const Color& color)
    {
        m_textColor = color;
        //m_cursor.setColor(color);
        update();
    }

    const Color& TextField::getTextColor() const
    {
        return m_textColor;
    }
}
