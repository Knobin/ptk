//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

// Local Headers
#include "../include/Log.hpp"

// pTK Headers
#include "ptk/core/ContextBase.hpp"
#include "ptk/core/Text.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkFontMetrics.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Text::Text()
        : m_font{}
    {
        m_font.setEdging(SkFont::Edging::kAntiAlias);
        // m_font.setEdging(SkFont::Edging::kAlias);
        // m_font.setEdging(SkFont::Edging::kSubpixelAntiAlias);
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
            }
            else
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
        }
        else
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

    void Text::setFontSize(uint32_t fontSize)
    {
        m_font.setSize(static_cast<SkScalar>(fontSize));
        updateFontInfo();
        onTextUpdate();
    }

    uint32_t Text::getFontSize() const
    {
        return static_cast<uint32_t>(m_font.getSize());
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
        /*float advance =*/m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        // return { advance, bounds.height() };
        return {bounds.width(), bounds.height()};
    }

    const SkFont& Text::skFont() const
    {
        return m_font;
    }

    void Text::updateFontInfo()
    {
        SkFontMetrics metrics{};
        m_font.getMetrics(&metrics);
        m_capHeight = std::abs(metrics.fCapHeight);
        m_ascentToDescent = metrics.fDescent - metrics.fAscent;
    }

} // namespace pTK
