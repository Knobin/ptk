//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

// Local Headers
#include "ptk/core/Text.hpp"

namespace pTK
{
    Text::Text()
        : m_text{""}, m_font{create<Font>()}
    {
    }
    
    Text::Text(const Ref<Font>& font)
        : m_text{""}, m_font{font}
    {
    }

    void Text::setFont(const Ref<Font>& font)
    {
        if (font)
            m_font = font;
    }
    
    Ref<Font> Text::getFont() const
    {
        return m_font;
    }

    bool Text::setFontFromFile(const std::string& path)
    {
        return m_font->loadFromFile(path);
    }

    bool Text::setFontFamily(const std::string& fontFamily)
    {
        return m_font->setFamily(fontFamily);
    }
    
    std::string Text::getFontFamily() const
    {
        return m_font->getFamily();
    }
    
    void Text::setFontSize(uint fontSize)
    {
        m_font->setSize(fontSize);
    }
    
    uint Text::getFontSize() const
    {
        return m_font->getSize();
    }
    
    void Text::setText(const std::string& text)
    {
        m_text = text;
    }
    
    const std::string& Text::getText() const
    {
        return m_text;
    }
    
    Size Text::getBounds() const
    {
        return m_font->getBounds(m_text);
    }
}
