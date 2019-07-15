//
//  core/Font.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-14.
//

// Local Headers
#include "ptk/core/Font.hpp"

namespace pTK
{
    Font::Font()
        : m_fontFamily{}, m_fontSize{12}
    {
    }
    
    void Font::setFontFamily(const std::string& fontFamily)
    {
        m_fontFamily = fontFamily;
    }
    
    const std::string& Font::getFontFamily() const
    {
        return m_fontFamily;
    }
    
    void Font::setFontSize(uint fontSize)
    {
        m_fontSize = fontSize;
    }
    
    uint Font::getFontSize() const
    {
        return m_fontSize;
    }
}
