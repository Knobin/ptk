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

namespace pTK
{
    TextField::TextField()
        : Rectangle(), Text(), m_cursor{}
    {
        m_cursor.setColor(Color{0x151515FF});

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

        Point textPos{getPosition()};
        const Point::value_type startPos = getPosition().x + ((rectSize.height - m_cursor.getSize().height) / 2);
        textPos.x = startPos;
        textPos.y += (rectSize.height - skFont()->getSpacing()) / 2;
        
        float advance = drawText(canvas, Color{0x151515FF}, textPos);

        if (m_drawCursor)
        {
            Point cursorPos{};
            cursorPos.x = static_cast<Point::value_type>(startPos + static_cast<Point::value_type>(std::ceil(advance)) + m_offset);
            cursorPos.y = getPosition().y + ((rectSize.height - m_cursor.getSize().height) / 2);

            m_cursor.setPosHint(cursorPos);
            m_cursor.onDraw(canvas);
        }
    }

    void TextField::onTextUpdate()
    {
        SkRect bounds{};
        skFont()->measureText("|", 1, SkTextEncoding::kUTF8, &bounds);

        Size cursorSize{1, 0};
        int bHeight = static_cast<int>(std::ceil(bounds.width()));
        cursorSize.height = bHeight + ((getSize().height - bHeight) / 2);
        m_cursor.setSize(cursorSize);
        m_offset = static_cast<int>(std::ceil(bounds.width()) / 2);
        
        Size minSize{getBounds()};
        minSize.width += m_offset + static_cast<Point::value_type>(cursorSize.width);
        minSize.height = (minSize.height > cursorSize.height) ? minSize.height : cursorSize.height;
        
        setMinSize(minSize);
    }
}
