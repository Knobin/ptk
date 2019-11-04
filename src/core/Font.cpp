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
        : m_fontSize{12}
    {
    }


    bool Font::loadFromFile(const std::string& path)
    {
        bool status = false;
        if (path != "")
        {
            sk_sp<SkTypeface> tf = SkTypeface::MakeFromFile(path.c_str());
            if (tf)
            {
                m_typeface.reset(tf.get());
                PTK_INFO("loadFromFile: family \"{0}\"", getFamily());
                status = true;
            } else
            {
                m_typeface.reset(SkTypeface::MakeDefault().get());
                PTK_WARN("Failed to load \"{0}\", fell back to \"{1}\"", path, getFamily());            
            }
            m_font = SkFont(m_typeface, m_fontSize);
        }
        
        return status;
    }

    bool Font::setFamily(const std::string& fontFamily)
    {
        bool status = false;
        if (fontFamily == "")
        {
            m_typeface.reset(SkTypeface::MakeDefault().get());
            PTK_INFO("Loaded default font \"{0}\"", getFamily());
        } else
        {
            m_typeface.reset(SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal()).get());

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

        m_font = SkFont(m_typeface, m_fontSize);
        return status;
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
