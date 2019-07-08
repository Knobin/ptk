//
//  widgets/Label.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

#ifndef PTK_WIDGETS_LABEL_HPP
#define PTK_WIDGETS_LABEL_HPP

// Local Headers
#include "ptk/core/Shape.hpp"
#include "ptk/core/Types.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    /** Label class implementation.
     
     This class is for drawing a Label.
     Deriving from Shape instead of directly of Widget,
     is for the unique properties that Shape has that Label
     can also use. Such as Color and outline.
     */
    class Label : public Shape
    {
    public:
        /** Constructs Label with default values.
         
         @return    default initialized Label
         */
        Label();
        ~Label() = default;
        
        /** Function for setting the font family of the Label.
         
         @param font    font family for Label to use
         */
        void setFont(const std::string& font);
        
        /** Function for retrieving the current font family used by Label.
         
         @return  current font family
         */
        const std::string& getFont() const;
        
        /** Function for setting the font size of the Label
         
         @param fontSize    size of the font
         */
        void setFontSize(uint fontSize);
        
        /** Function for retrieving the current font size used by Label.
         
         @return  current font size
         */
        uint getFontSize() const;
        
        /** Function for setting the text of the Label
         
         @param text    text for Label
         */
        void setText(const std::string& text);
        
        /** Function for retrieving the current text used by Label.
         
         @return  current text
         */
        const std::string& getText() const;
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         */
        void onDraw(SkCanvas* canvas) override;
        
    private:
        std::string m_fontName;
        uint m_fontSize;
        std::string m_text;
        
        void calculateBounds();
        
        // Keep these private, for internal use.
        using Widget::setSizeHint;
    };
}

#endif // PTK_WIDGETS_LABEL_HPP
