//
//  widgets/TextField.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

// pTK Headers
#include "ptk/widgets/TextField.hpp"
#include "ptk/util/Math.hpp"

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
                handleKeyPress(keycode, modifier);
            return false;
        });

        onInput([this](const std::unique_ptr<uint32[]>& data, std::size_t size, Text::Encoding encoding) {
            handleInput(data, size, encoding);
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

    void TextField::handleKeyPress(KeyCode keycode, byte)
    {
        switch (keycode) {
            case Key::Backspace:
            case Key::Delete:
            {
                removeFromText(((keycode == Key::Delete) ? 1 : -1));
                break;
            }
            case Key::Left:
            case Key::Right:
            {
                moveCursor(((keycode == Key::Left) ? -1 : 1), getText().size(), true);
                break;
            }
            case Key::Home:
            case Key::End:
            {
                moveCursorToPos(((keycode == Key::Home) ? 0 : getText().size()), getText().size(), true);
                break;
            }
            default:
                break;
        }
    }

    void TextField::removeFromText(int direction)
    {
        if (direction > 0)
        {
            if (m_cursorLocation < getText().size())
            {
                std::string str{getText()};
                str.erase(m_cursorLocation, 1);
                setText(str);
            }
        }
        else
        {
            if (m_cursorLocation > 0)
            {
                std::string str{getText()};
                str.erase(m_cursorLocation - 1, 1);
                moveCursor(-1, str.size());
                setText(str);
            }
        }
    }

    void TextField::handleInput(const std::unique_ptr<uint32[]>& data, std::size_t size, Text::Encoding)
    {
        // This currently ignores the encoding.
        // TODO: Fix encoding for the data.

        std::string toAdd{};

        for (std::size_t i{0}; i < size; ++i)
            toAdd += static_cast<char>(data[i]);

        m_text.insert(m_cursorLocation, toAdd);
        moveCursor(static_cast<int>(size), m_text.size());
        onTextUpdate();
    }

    void TextField::moveCursor(int direction, std::size_t strSize, bool shouldDraw)
    {
        bool sDraw{false};

        if (direction > 0)
        {
            if ((m_cursorLocation + static_cast<std::size_t>(direction)) <= strSize)
            {
                m_cursorLocation += static_cast<std::size_t>(direction);
                sDraw = true;
            }
        }
        else if (direction < 0)
        {
            int cursorLoc = static_cast<int>(m_cursorLocation);
            if (cursorLoc + direction > 0)
                m_cursorLocation = static_cast<std::size_t>(cursorLoc + direction);
            else
                m_cursorLocation = 0;

            sDraw = true;
        }

        if (sDraw && shouldDraw)
            draw();
    }

    void TextField::moveCursorToPos(std::size_t pos, std::size_t strSize, bool shouldDraw)
    {
        if ((m_cursorLocation != pos) && (pos <= strSize))
        {
            m_cursorLocation = pos;
            if (shouldDraw)
                draw();
        }
    }

    void TextField::onDraw(SkCanvas* canvas)
    {
        Rectangle::onDraw(canvas);
        const Size rectSize{getSize()};

        const Text::StrData textData{ getText().c_str(), getText().size(), Text::Encoding::UTF8 };
        float advance = (!getText().empty()) ? drawTextLine(canvas, textData, m_textColor, m_textPos) : 0.0f;

        if (getText().empty())
        {
            const Text::StrData placeholderStrData{ m_placeholderText.c_str(), m_placeholderText.size(), Text::Encoding::UTF8 };
            drawTextLine(canvas, placeholderStrData, m_placeholderColor, m_textPos);
        }

        if (m_drawCursor)
        {
            SkPaint paint{GetSkPaintFromColor(m_textColor)};
            paint.setStrokeWidth(1.0f);

            if (m_cursorLocation <= getText().size())
            {
                SkRect tmp{};
                advance = skFont().measureText(getText().c_str(), m_cursorLocation, SkTextEncoding::kUTF8, &tmp);
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

    void TextField::setText(const std::string& text)
    {
        m_text = text;
        onTextUpdate();
    }

    const std::string& TextField::getText() const
    {
        return m_text;
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

        Size minSize{Vec2ToSize(getBoundsFromStr(getText()), Math::ceilf)};
        minSize.width += 1;
        auto ceilCursorHeight = static_cast<Size::value_type>(std::ceil(m_cursorHeight));
        minSize.height = (minSize.height > ceilCursorHeight) ? minSize.height : ceilCursorHeight;

        Vec2f placeholderBounds{ getBoundsFromStr(m_placeholderText) };
        Size placeholderSize{ Vec2ToSize(placeholderBounds, Math::ceilf) };

        minSize.width = (minSize.width > placeholderSize.width) ? minSize.width : placeholderSize.width;
        minSize.height = (minSize.height > placeholderSize.height) ? minSize.height : placeholderSize.height;

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
        update();
    }

    const Color& TextField::getTextColor() const
    {
        return m_textColor;
    }
}
