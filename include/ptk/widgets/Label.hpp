//
//  widgets/Label.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

#ifndef PTK_WIDGETS_LABEL_HPP
#define PTK_WIDGETS_LABEL_HPP

// Local Headers
#include "ptk/core/Font.hpp"
#include "ptk/core/Shape.hpp"
#include "ptk/core/Types.hpp"

namespace pTK
{
    /** Label class implementation.
     
     This class is for drawing a Label.
     Deriving from Shape instead of directly of Widget,
     is for the unique properties that Shape has that Label
     can also use. Such as Color and outline.
     */
    class Label : public Shape, public Font
    {
    public:
        /** Constructs Label with default values.
         
         @return    default initialized Label
         */
        Label();
        ~Label() = default;
        
        /** Function for setting the font family of the Label.
         
         Derived from Font.
         
         @param font    font family for Label to use
         */
        void setFontFamily(const std::string& fontFamily) override;
        
        /** Function for setting the font size of the Label.
         
         Derived from Font.
         
         @param fontSize    size of the font
         */
        void setFontSize(uint fontSize) override;
        
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
        std::string m_text;
        
        void calculateBounds();
        
        // Keep these private, for internal use.
        using Widget::setSize;
    };
}

#endif // PTK_WIDGETS_LABEL_HPP
