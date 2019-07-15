//
//  core/Font.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-14.
//

#ifndef PTK_CORE_FONT_HPP
#define PTK_CORE_FONT_HPP

// Local Headers
#include "ptk/core/Types.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    /** Font class implementation.
     
     This class is for handling font information.
     */
    class Font
    {
    public:
        /** Constructs Font with default values.
         
         @return    default initialized Font
         */
        Font();
        virtual ~Font() = default;
        
        /** Function for setting the font family of the Label.
         
         @param font    font family for Label to use
         */
        virtual void setFontFamily(const std::string& fontFamily);
        
        /** Function for retrieving the current font family used by Label.
         
         @return  current font family
         */
        const std::string& getFontFamily() const;
        
        /** Function for setting the font size of the Label
         
         @param fontSize    size of the font
         */
        virtual void setFontSize(uint fontSize);
        
        /** Function for retrieving the current font size used by Label.
         
         @return  current font size
         */
        uint getFontSize() const;

    private:
        std::string m_fontFamily;
        uint m_fontSize;
    };
}


#endif // PTK_CORE_FONT_HPP


