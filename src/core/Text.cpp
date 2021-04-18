//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

// pTK Headers
#include "ptk/core/Text.hpp"

#include "ptk/widgets/Rectangle.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkFontMetrics.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Text::Text()
        : m_text{}, m_font{}
    {
        m_font.setEdging(SkFont::Edging::kAntiAlias);
        updateFontInfo();
    }

    bool Text::setFontFromFile(const std::string& path)
    {
        if (!path.empty())
        {
            sk_sp<SkTypeface> tf{SkTypeface::MakeFromFile(path.c_str())};
            if (tf)
            {
                m_font.setTypeface(tf);
                PTK_INFO("Loaded font family \"{0}\" from file \"{1}\"", getFontFamily(), path);
                updateFontInfo();
                onTextUpdate();
                return true;
            } else
            {
                m_font.setTypeface(SkTypeface::MakeDefault());
                PTK_WARN("Failed to load \"{0}\", fell back to \"{1}\"", path, getFontFamily());
            }
        }
        
        return false;
    }

    bool Text::setFontFamily(const std::string& fontFamily)
    {
        if (fontFamily.empty())
        {
            m_font.setTypeface(SkTypeface::MakeDefault());
            PTK_INFO("Loaded default font \"{0}\"", getFontFamily());
        } else
        {
            m_font.setTypeface(SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal()));
            if (fontFamily == getFontFamily())
            {
                PTK_INFO("Loaded \"{0}\" successfully.", getFontFamily());
                updateFontInfo();
                onTextUpdate();
                return true;
            }
#ifdef PTK_DEBUG
            else
                PTK_WARN("Failed to load \"{0}\", fell back to \"{1}\"", fontFamily, getFontFamily());
#endif
        }

        return false;
    }
    
    std::string Text::getFontFamily() const
    {
        SkString str{};
        m_font.getTypefaceOrDefault()->getFamilyName(&str);
        
        return std::string(str.c_str());
    }
    
    void Text::setFontSize(uint fontSize)
    {
        m_font.setSize(static_cast<SkScalar>(fontSize));
        updateFontInfo();
        onTextUpdate();
    }
    
    uint Text::getFontSize() const
    {
        return static_cast<uint>(m_font.getSize());
    }
    
    void Text::setText(const std::string& text)
    {
        m_text = text;
        onTextUpdate();
    }
    
    const std::string& Text::getText() const
    {
        return m_text;
    }

    float Text::ascentToDescent() const
    {
        return m_ascentToDescent;
    }

    float Text::capHeight() const
    {
        return m_capHeight;
    }
    
    Size Text::getBounds() const
    {
        SkRect bounds{};
        float advance = m_font.measureText(m_text.c_str(), m_text.size(), SkTextEncoding::kUTF8, &bounds);
        return {static_cast<int>(std::ceil(advance)), static_cast<int>(std::ceil(bounds.height()))};
    }

    const SkFont& Text::skFont() const
    {
        return m_font;
    }

    static std::size_t SpaceCount(const std::string& str)
    {
        std::size_t count{0};
        for (char it : str)
        {
            if (it == ' ')
                ++count;
            else
                break;
        }
        return count;
    }

    static float StartSpaceOffset(const SkFont& font, const std::string& str)
    {
        float offset{0.0f};
        std::size_t spaceCount{SpaceCount(str)};
        
        if (spaceCount > 0)
        {
            SkRect tmp{};
            offset = font.measureText(str.c_str(), spaceCount, SkTextEncoding::kUTF8, &tmp);
        }
        
        return offset;
    }

    float Text::drawTextLineWithPaint(SkCanvas* canvas, const std::string& str, const Vec2f& pos, const SkPaint& paint)
    {
        SkRect bounds{};
        float advance = m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);

#ifdef PTK_DRAW_TEXT_RECT
        Rectangle rect{};
        rect.setPosHint({static_cast<int>(pos.x), static_cast<int>(pos.y)});
        rect.setSize({static_cast<int>(std::ceil(advance)), static_cast<int>(std::ceil(m_capHeight))});
        rect.setColor(Color{0xFF1212AA});
        rect.onDraw(canvas);
#endif

        canvas->drawString(str.c_str(), pos.x + StartSpaceOffset(m_font, str) + (-1*bounds.x()), pos.y + m_capHeight, m_font, paint);

        return advance;
    }

    float Text::drawTextLine(SkCanvas* canvas, const std::string& str, const Color& color, const Vec2f& pos)
    {
        SkPaint paint = GetSkPaintFromColor(color);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);

        return drawTextLineWithPaint(canvas, str, pos, paint);
    }

    float Text::drawTextLine(SkCanvas* canvas, const std::string& str, const Color& color, const Vec2f& pos, float outlineSize, const Color& outColor)
    {
        if (!(outlineSize > 0.0f))
            return drawTextLine(canvas, str, color, pos);
        
        SkPaint paint = GetSkPaintFromColor(color);
        paint.setStrokeWidth(outlineSize);
        paint.setStyle(SkPaint::kFill_Style);

        float advance = drawTextLineWithPaint(canvas, str, pos, paint);

        paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
        paint.setStyle(SkPaint::kStroke_Style);

        drawTextLineWithPaint(canvas, str, pos, paint);

        return advance;
    }

    void Text::updateFontInfo()
    {
        SkFontMetrics metrics{};
        m_font.getMetrics(&metrics);
        m_capHeight = std::abs(metrics.fCapHeight);
        m_ascentToDescent = metrics.fDescent - metrics.fAscent;
    }

}
