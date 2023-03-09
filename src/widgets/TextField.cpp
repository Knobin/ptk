//
//  widgets/TextField.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

// pTK Headers
#include "ptk/widgets/TextField.hpp"
#include "ptk/core/ContextBase.hpp"
#include "ptk/util/Math.hpp"

// C++ Headers
#include <cctype>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkFontMetrics.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    TextField::TextField()
        : Widget(),
          Text()
    {
        onKey([this](const KeyEvent& evt) {
            if (evt.type == Event::Type::KeyPressed)
                handleKeyPress(evt.keycode, evt.modifier);
            return false;
        });

        onInput([this](const InputEvent& evt) {
            handleInput(evt.data, evt.size, evt.encoding);
            return false;
        });

        onClick([this](const ClickEvent&) {
            m_drawCursor = true;
            return false;
        });

        onLeaveClick([this](const LeaveClickEvent&) {
            m_drawCursor = false;
            return false;
        });
    }

    void TextField::handleKeyPress(KeyCode keycode, uint8_t)
    {
        switch (keycode)
        {
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

    void TextField::handleInput(const std::unique_ptr<uint32_t[]>& data, std::size_t size, Text::Encoding)
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

    void TextField::onDraw(Canvas* canvas)
    {
        canvas->drawRoundRect(getPosition(), getSize(), getColor(), getCornerRadius(), getOutlineColor(),
                              getOutlineThickness());

        const Size rectSize{getSize()};
        const SkFont* font = &skFont();

        const Text::StrData textData{getText().c_str(), getText().size(), Text::Encoding::UTF8};
        float advance = (!getText().empty()) ? canvas->drawTextLine(textData, m_textColor, m_textPos, font) : 0.0f;

        if (getText().empty())
        {
            const Text::StrData placeholderStrData{m_placeholderText.c_str(), m_placeholderText.size(),
                                                   Text::Encoding::UTF8};
            canvas->drawTextLine(placeholderStrData, m_placeholderColor, m_textPos, font);
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

            float startY =
                static_cast<float>(getPosition().y) + ((static_cast<float>(rectSize.height) - m_cursorHeight) / 2);
            float endY = startY + m_cursorHeight;
            canvas->skCanvas->drawLine({posX, startY}, {posX, endY}, paint);
        }
    }

    void TextField::onTextUpdate()
    {
        updateBounds();

        if (m_text.size() < m_cursorLocation)
            m_cursorLocation = m_text.size();
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
        draw();
    }

    const std::string& TextField::getText() const
    {
        return m_text;
    }

    void TextField::setCornerRadius(float radius)
    {
        if (radius >= 0)
            m_cornerRadius = radius;
    }

    float TextField::getCornerRadius() const
    {
        return m_cornerRadius;
    }

    const Color& TextField::getColor() const
    {
        return m_color;
    }

    void TextField::setColor(const Color& color)
    {
        m_color = color;
        draw();
    }

    const Color& TextField::getOutlineColor() const
    {
        return m_outlineColor;
    }

    void TextField::setOutlineColor(const Color& outline_color)
    {
        m_outlineColor = outline_color;
        draw();
    }

    float TextField::getOutlineThickness() const
    {
        return m_outlineThickness;
    }

    void TextField::setOutlineThickness(float outlineThickness)
    {
        m_outlineThickness = outlineThickness;
        draw();
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

        Vec2f bounds{getBoundsFromStr(getText())};
        bounds.x = Math::ceilf(bounds.x);
        bounds.y = Math::ceilf(bounds.y);
        Size minSize{Size::MakeNarrow(bounds.x, bounds.y)};
        minSize.width += 1;
        auto ceilCursorHeight = static_cast<Size::value_type>(std::ceil(m_cursorHeight));
        minSize.height = (minSize.height > ceilCursorHeight) ? minSize.height : ceilCursorHeight;

        Vec2f placeholderBounds{getBoundsFromStr(m_placeholderText)};
        placeholderBounds.x = Math::ceilf(placeholderBounds.x);
        placeholderBounds.y = Math::ceilf(placeholderBounds.y);
        Size placeholderSize{Size::MakeNarrow(placeholderBounds.x, placeholderBounds.y)};

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
} // namespace pTK
