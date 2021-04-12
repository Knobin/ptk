//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

// pTK Headers
#include "ptk/core/Text.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkFontMetrics.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Text::Text()
        : m_text{""}, m_font{}
    {
        m_font.setEdging(SkFont::Edging::kAntiAlias);
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
        m_font.getTypeface()->getFamilyName(&str);
        
        return std::string(str.c_str());
    }
    
    void Text::setFontSize(uint fontSize)
    {
        m_font.setSize(static_cast<SkScalar>(fontSize));
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
    
    Size Text::getBounds() const
    {
        SkRect bounds{};
        float advance = m_font.measureText(m_text.c_str(), m_text.size(), SkTextEncoding::kUTF8, &bounds);
        return Size(static_cast<int>(std::ceil(advance)), static_cast<int>(std::ceil(bounds.height())));
    }

    const SkFont* Text::skFont() const
    {
        return &m_font;
    }

    static std::size_t SpaceCount(const std::string& str)
    {
        std::size_t count{0};
        for (auto it = str.cbegin(); it != str.cend(); ++it)
        {
            if ((*it) == ' ')
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

    float Text::drawText(SkCanvas* canvas, const std::string& str, const Color& color, const Vec2f& pos)
    {
        SkPaint paint = GetSkPaintFromColor(color);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        SkRect bounds{};
        float advance = m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        const SkPoint skPos{pos.x, pos.y};
        
        SkFontMetrics metrics{};
        m_font.getMetrics(&metrics);

        canvas->drawString(str.c_str(), skPos.x() + StartSpaceOffset(m_font, str) + (-1*bounds.x()), skPos.y() + (-1*metrics.fAscent), m_font, paint);

        return advance;
    }

    float Text::drawText(SkCanvas* canvas, const std::string& str, const Color& color, const Vec2f& pos, float outlineSize, const Color& outColor)
    {
        SkPaint paint = GetSkPaintFromColor(color);
        
        SkRect bounds{};
        float advance = m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        const SkPoint skPos{pos.x, pos.y};
        
        SkFontMetrics metrics{};
        m_font.getMetrics(&metrics);
        
        // Outline
        paint.setStrokeWidth(outlineSize);
        if (outlineSize > 0.0f)
            paint.setStyle(SkPaint::kFill_Style);
        else
            paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        canvas->drawString(str.c_str(), skPos.x() + StartSpaceOffset(m_font, str) + (-1*bounds.x()), skPos.y() + (-1*metrics.fAscent), m_font, paint);
        
        if (outlineSize > 0.0f)
        {
            // Draw Outline
            paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            canvas->drawString(str.c_str(), skPos.x() + StartSpaceOffset(m_font, str) + (-1*bounds.x()), skPos.y() + (-1*metrics.fAscent), m_font, paint);
        }

        return advance;
    }

}
