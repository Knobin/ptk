//
//  core/Font.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-14.
//

// Local Headers
#include "ptk/core/Font.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <cmath>

namespace pTK
{
    Font::Font()
    {
        m_font.setEdging(SkFont::Edging::kAntiAlias);
    }


    bool Font::loadFromFile(const std::string& path)
    {
        bool status = false;
        if (path != "")
        {
            sk_sp<SkTypeface> tf = SkTypeface::MakeFromFile(path.c_str());
            if (tf)
            {
                m_font.setTypeface(tf);
                PTK_INFO("Loaded font family \"{0}\" from file \"{1}\"", getFamily(), path);
                status = true;
            } else
            {
                m_font.setTypeface(SkTypeface::MakeDefault());
                PTK_WARN("Failed to load \"{0}\", fell back to \"{1}\"", path, getFamily());            
            }
        }
        
        return status;
    }

    bool Font::setFamily(const std::string& fontFamily)
    {
        bool status = false;
        if (fontFamily == "")
        {
            m_font.setTypeface(SkTypeface::MakeDefault());
            PTK_INFO("Loaded default font \"{0}\"", getFamily());
        } else
        {
            m_font.setTypeface(SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal()));

            if (fontFamily == getFamily())
            {
                status = true;
                PTK_INFO("Loaded {0} successfully.", getFamily());
            }
#ifdef PTK_DEBUG
            else
                PTK_WARN("Failed to load \"{0}\", fell back to \"{1}\"", fontFamily, getFamily());            
#endif
        }

        return status;
    }
    
    std::string Font::getFamily() const
    {
        SkString str;
        m_font.getTypeface()->getFamilyName(&str);
        
        return std::string(str.c_str());
    }
    
    void Font::setSize(uint fontSize)
    {
        m_font.setSize(fontSize);
    }
    
    uint Font::getSize() const
    {
        return static_cast<uint>(m_font.getSize());
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
}
