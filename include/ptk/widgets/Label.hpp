//
//  widgets/Label.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

#ifndef PTK_WIDGETS_LABEL_HPP
#define PTK_WIDGETS_LABEL_HPP

// Local Headers
#include "ptk/core/Text.hpp"
#include "ptk/core/Shape.hpp"

namespace pTK
{
    /** Label class implementation.
     
     This class is for drawing a Label.
     Deriving from Shape instead of directly of Widget,
     is for the unique properties that Shape has that Label
     can also use. Such as Color and outline.
     */
    class Label : public Shape, public Text
    {
    public:
        /** Constructs Label with default values.
         
         @return    default initialized Label
         */
        Label();
        virtual ~Label() = default;
        
        /** Function for setting the font from a given file.
         
         Derived from Text.
         
         @param path    font file to load
         @return        status
         */
        bool setFontFromFile(const std::string& path) override;
        
        /** Function for setting the font family of the Label.
         
         Derived from Text.
         
         @param font    font family for Label to use
         @return        status
         */
        bool setFontFamily(const std::string& fontFamily) override;
        
        /** Function for setting the font size of the Label.
         
         Derived from Text.
         
         @param fontSize    size of the font
         */
        void setFontSize(uint fontSize) override;
        
        /** Function for setting the text of the Label
         
         @param text    text for Label
         */
        void setText(const std::string& text) override;
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         */
        void onDraw(SkCanvas* canvas) override;
        
    private:
        
        // Keep these private, for internal use.
        using Widget::setSize;
    };
}

#endif // PTK_WIDGETS_LABEL_HPP
