//
//  core/Font.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-14.
//

// Local Headers
#include "ptk/core/Font.hpp"

// C++ Headers
#include <cmath>

namespace pTK
{
    Font::Font()
        : m_fontSize{12}
    {
    }
    
    void Font::setFamily(const std::string& fontFamily)
    {
        if (fontFamily == "")
            m_typeface.reset(SkTypeface::MakeDefault().get());
        else
            m_typeface.reset(SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal()).get());
        
        m_font = SkFont(m_typeface, m_fontSize);
    }
    
    std::string Font::getFamily() const
    {
        SkString str;
        m_typeface->getFamilyName(&str);
        
        return std::string(str.c_str());
    }
    
    void Font::setSize(uint fontSize)
    {
        m_fontSize = fontSize;
        m_font = SkFont(m_typeface, m_fontSize);
    }
    
    uint Font::getSize() const
    {
        return m_fontSize;
    }
    
    Size Font::getBounds(const std::string& str) const
    {
        SkRect bounds;
        m_font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds);
        return Size(static_cast<int>(std::ceil(bounds.width())), static_cast<int>(std::ceil(bounds.height())));
    }
    
    const SkFont* Font::skFont() const
    {
        return &m_font;
    }
    
    sk_sp<SkTypeface> Font::skTypeface() const
    {
        return m_typeface;
    }
}
