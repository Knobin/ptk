//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

// pTK Headers
#include "ptk/core/Text.hpp"

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

    float Text::drawText(SkCanvas* canvas, const std::string& str, const Color& color, const Vec2f& pos)
    {
        SkPaint paint = GetSkPaintFromColor(color);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        SkRect bounds{};
        float advance = m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        const SkPoint skPos{pos.x, pos.y};

        std::size_t startSpaces{0};
        const std::string& text{getText()};
        for (auto it = text.cbegin(); it != text.cend(); ++it)
        {
            if ((*it) == ' ')
                ++startSpaces;
            else
                break;
        }

        SkScalar startSpaceOffset{0};
        if (startSpaces > 0)
        {
            SkRect tmp{};
            float spaceAdvance = m_font.measureText(str.c_str(), startSpaces, SkTextEncoding::kUTF8, &tmp);
            startSpaceOffset = spaceAdvance;
        }
        
        SkGlyphID id;
        int count = m_font.textToGlyphs("a", 1, SkTextEncoding::kUTF8, &id, 1);
        SkPoint point;
        m_font.getPos(&id, 1, &point);
        PTK_INFO("{} Glyph pos: {}x{}, {}x{}", count, point.x(), point.y(), point.fX, point.fY);

        canvas->drawString(str.c_str(), skPos.x() + startSpaceOffset + (-1*bounds.x()), skPos.y() + (-1*bounds.y()), m_font, paint);

        return advance;
    }

    float Text::drawText(SkCanvas* canvas, const std::string& str, const Color& color, const Vec2f& pos, float outlineSize, const Color& outColor)
    {
        SkPaint paint = GetSkPaintFromColor(color);
        
        SkRect bounds{};
        float advance = m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        const SkPoint skPos{pos.x, pos.y};
        
        // Outline
        paint.setStrokeWidth(outlineSize);
        if (outlineSize > 0.0f)
            paint.setStyle(SkPaint::kFill_Style);
        else
            paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        canvas->drawString(str.c_str(), skPos.fX + (-1*bounds.x()), skPos.fY + (-1*bounds.y()), m_font, paint);
        
        if (outlineSize > 0.0f)
        {
            // Draw Outline
            paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            canvas->drawString(str.c_str(), skPos.fX + (-1*bounds.x()), skPos.fY + (-1*bounds.y()), m_font, paint);
        }

        return advance;
    }

}
