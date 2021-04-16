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
                if (IsKeyCodeAlpha(keycode) || IsKeyCodeDigit(keycode) || IsKeyCodeSpace(keycode))
                {
                    bool shift = IsKeyEventModifierSet(modifier, KeyEvent::Modifier::Shift);
                    bool capsLock = IsKeyEventModifierSet(modifier, KeyEvent::Modifier::CapsLock);

                    char toInsert{0};
                    if (IsKeyCodeAlpha(keycode))
                    {
                        char alpha = KeyCodeToAlpha(keycode);
                        toInsert = ((capsLock && !shift) || (!capsLock && shift)) ? alpha : static_cast<char>(std::tolower(alpha));
                    }
                    else if (IsKeyCodeDigit(keycode))
                        toInsert = KeyCodeToGraph(keycode);
                    else if (IsKeyCodeSpace(keycode))
                        toInsert = ' ';

                    if (toInsert != 0)
                    {
                        std::string str{getText()};
                        str.insert(m_cursorLocation , 1, toInsert);
                        ++m_cursorLocation;
                        setText(str);
                    }
                }
                else
                {
                    switch (keycode) {
                        case Key::Backspace:
                        {
                            if (m_cursorLocation > 0)
                            {
                                std::string str{getText()};
                                str.erase(m_cursorLocation - 1, 1);
                                setText(str);
                                --m_cursorLocation;
                            }

                            break;
                        }
                        case Key::Delete:
                        {
                            if (m_cursorLocation < getText().size())
                            {
                                std::string str{getText()};
                                str.erase(m_cursorLocation, 1);
                                setText(str);
                            }

                            break;
                        }
                        case Key::Left:
                        {
                            if (m_cursorLocation > 0)
                            {
                                --m_cursorLocation;
                                draw();
                            }
                            break;
                        }
                        case Key::Right:
                        {
                            if ((!getText().empty()) && (m_cursorLocation < getText().size()))
                            {
                                ++m_cursorLocation;
                                draw();
                            }
                            break;
                        }
                        case Key::Home:
                        {
                            if (m_cursorLocation != 0)
                            {
                                m_cursorLocation = 0;
                                draw();
                            }
                            break;
                        }
                        case Key::End:
                        {
                            std::size_t size{getText().size()};
                            if (m_cursorLocation != size)
                            {
                                m_cursorLocation = size;
                                draw();
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
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
        PTK_INFO("CURSOR LOCATION: {}", m_cursorLocation);
        const Size rectSize{getSize()};
        
        float advance = (!getText().empty()) ? drawTextLine(canvas, getText(), m_textColor, m_textPos) : 0.0f;
        
        if (getText().empty())
            drawTextLine(canvas, m_placeholderText, m_placeholderColor, m_textPos);

        if (m_drawCursor)
        {
            SkPaint paint{GetSkPaintFromColor(m_textColor)};
            paint.setStrokeWidth(1.0f);

            if (m_cursorLocation <= getText().size())
            {
                SkRect tmp{};
                advance = skFont()->measureText(getText().c_str(), m_cursorLocation, SkTextEncoding::kUTF8, &tmp);
            }

            float posX = m_textPos.x + advance - ((m_cursorLocation == 0) ? 2.0f : 0.0f);

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
