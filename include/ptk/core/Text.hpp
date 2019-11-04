//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

#ifndef PTK_CORE_TEXT_HPP
#define PTK_CORE_TEXT_HPP

// Local Headers
#include "ptk/core/Font.hpp"

namespace pTK
{
    /** Text class implementation.
     
     */
    class Text
    {
    public:
        /** Constructs Text with default values.
         
         @return    default initialized Text
         */
        Text();
        virtual ~Text() = default;

        virtual bool setFontFromFile(const std::string& path);

        /** Function for setting the font family.
         
         @param font    font family to use.
         */
        virtual bool setFontFamily(const std::string& fontFamily);
        
        /** Function for retrieving the font family.
         
         @return  current font family
         */
        std::string getFontFamily() const;
        
        /** Function for setting the font size.
         
         @param fontSize    size of the font
         */
        virtual void setFontSize(uint fontSize);
        
        /** Function for retrieving the font size.
         
         @return  current font size
         */
        uint getFontSize() const;
        
        /** Function for setting the text.
         
         @param text    text for Label
         */
        virtual void setText(const std::string& text);
        
        /** Function for retrieving the text.
         
         @return  current text
         */
        const std::string& getText() const;
        
        /** Function for retrieving the Font.
         
         @return  Font
         */
        const Font* getFont() const;
        
        /** Function for retrieving the bounds of the text.
         
         @return  bounds of the text
         */
        Size getBounds() const;
        
    private:
        std::string m_text;
        Font m_font;
    };
}

#endif // PTK_CORE_TEXT_HPP

