//
//  core/Text.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-10-09.
//

#ifndef PTK_CORE_TEXT_HPP
#define PTK_CORE_TEXT_HPP

// pTK Headers
#include "ptk/Core.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/core/Drawable.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"
PTK_DISABLE_WARN_END()

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

        /** Destructor for Text.

        */
        virtual ~Text() = default;
        
        /** Function for setting the font from a given file.
         
            @param path    font file to load
            @return        status
        */
        bool setFontFromFile(const std::string& path);

        /** Function for setting the font family.
         
            @param font    font family to use.
        */
        bool setFontFamily(const std::string& fontFamily);
        
        /** Function for retrieving the font family.
         
            @return  current font family
        */
        std::string getFontFamily() const;
        
        /** Function for setting the font size.
         
            @param fontSize    size of the font
        */
        void setFontSize(uint fontSize);
        
        /** Function for retrieving the font size.
         
            @return  current font size
        */
        uint getFontSize() const;
        
        /** Function for setting the text.
         
            @param text    text for Label
        */
        void setText(const std::string& text);
        
        /** Function for retrieving the text.
         
            @return  current text
        */
        const std::string& getText() const;
        
        /** Function for retrieving the bounds of the text.
         
            @return  bounds of the text
        */
        Size getBounds() const;
        
        /** Function for retrieving the raw SkFont.
         
         @return  raw SkFont
         */
        const SkFont* skFont() const;
        
        void drawText(SkCanvas* canvas, const Color& color, const Point& pos);
        void drawText(SkCanvas* canvas, const Color& color, const Point& pos, float outlineSize, const Color& outColor);
        
    private:
        virtual void onTextUpdate() {}
        
    private:
        std::string m_text;
        SkFont m_font;
    };
}

#endif // PTK_CORE_TEXT_HPP

