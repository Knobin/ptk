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
        : m_font{}
    {
        m_font.setEdging(SkFont::Edging::kAntiAlias);
        //m_font.setEdging(SkFont::Edging::kAlias);
        //m_font.setEdging(SkFont::Edging::kSubpixelAntiAlias);
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

    float Text::ascentToDescent() const
    {
        return m_ascentToDescent;
    }

    float Text::capHeight() const
    {
        return m_capHeight;
    }

    Vec2f Text::getBoundsFromStr(const std::string& str) const
    {
        SkRect bounds{};
        /*float advance =*/ m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        // return { advance, bounds.height() };
        return { bounds.width(), bounds.height() };
    }


    const SkFont& Text::skFont() const
    {
        return m_font;
    }

    static constexpr std::size_t EncodingByteSize(Text::Encoding encoding)
    {
        if (encoding == Text::Encoding::UTF16)
            return 2;
        if (encoding == Text::Encoding::UTF32)
            return 4;

        return 1;
    }

    static std::size_t SpaceCount(const Text::StrData& data)
    {
        std::size_t count{0};

        const void* ptr = data.text;

        for (std::size_t i{ 0 }; i < data.size; ++i)
        {
            uint32 ch{ 0 };

            if (data.encoding == Text::Encoding::UTF8)
            {
                const uint8 *c = static_cast<const uint8*>(ptr);
                ch = static_cast<uint32>(c[i]);
            }

            if (data.encoding == Text::Encoding::UTF16)
            {
                const uint16* c = static_cast<const uint16*>(ptr);
                ch = static_cast<uint32>(c[i]);
            }

            if (data.encoding == Text::Encoding::UTF32)
            {
                const uint32* c = static_cast<const uint32*>(ptr);
                ch = static_cast<uint32>(c[i]);
            }

            if (ch == 0x20)
                ++count;
            else
                break;
        }
            

        
        return count;
    }

    static SkTextEncoding EncodingToSkTextEncoding(Text::Encoding encoding)
    {
        if (encoding == Text::Encoding::UTF16)
            return SkTextEncoding::kUTF16;
        if (encoding == Text::Encoding::UTF32)
            return SkTextEncoding::kUTF32;

        return SkTextEncoding::kUTF8;
    }

    static float StartSpaceOffset(const SkFont& font, const Text::StrData& data)
    {
        // This function only handles " " for now.
        // TODO: Support for no-break space should be added.

        float offset{0.0f};
        std::size_t spaceCount{SpaceCount(data)};
        
        if (spaceCount > 0)
        {
            SkRect tmp{};
            offset = font.measureText(data.text, spaceCount, EncodingToSkTextEncoding(data.encoding), &tmp);
        }
        
        return offset;
    }

    

    float Text::drawTextLineWithPaint(SkCanvas* canvas, const StrData& data, const Vec2f& pos, const SkPaint& paint)
    {
        SkRect bounds{};

        std::size_t byteLength = data.size * EncodingByteSize(data.encoding);
        float advance = m_font.measureText(data.text, byteLength, EncodingToSkTextEncoding(data.encoding), &bounds);

#ifdef PTK_DRAW_TEXT_RECT
        Rectangle rect{};
        rect.setPosHint({static_cast<int>(pos.x), static_cast<int>(pos.y)});
        rect.setSize({static_cast<int>(std::ceil(advance)), static_cast<int>(std::ceil(m_capHeight))});
        rect.setColor(Color{0xFF1212AA});
        rect.onDraw(canvas);
#endif

        SkScalar x = pos.x + StartSpaceOffset(m_font, data) + (-1 * bounds.x());
        SkScalar y = pos.y + m_capHeight;

        canvas->drawSimpleText(data.text, byteLength, EncodingToSkTextEncoding(data.encoding), x, y, m_font, paint);

        return advance;
    }

    float Text::drawTextLine(SkCanvas* canvas, const StrData& data, const Color& color, const Vec2f& pos)
    {
        SkPaint paint = GetSkPaintFromColor(color);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);

        return drawTextLineWithPaint(canvas, data, pos, paint);
    }

    float Text::drawTextLine(SkCanvas* canvas, const StrData& data, const Color& color, const Vec2f& pos, float outlineSize, const Color& outColor)
    {
        if (!(outlineSize > 0.0f))
            return drawTextLine(canvas, data, color, pos);
        
        SkPaint paint = GetSkPaintFromColor(color);
        paint.setStrokeWidth(outlineSize);
        paint.setStyle(SkPaint::kFill_Style);

        float advance = drawTextLineWithPaint(canvas, data, pos, paint);

        paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
        paint.setStyle(SkPaint::kStroke_Style);

        drawTextLineWithPaint(canvas, data, pos, paint);

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
