//
//  widgets/Label.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-30.
//

#ifndef PTK_WIDGETS_LABEL_HPP
#define PTK_WIDGETS_LABEL_HPP

// pTK Headers
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
        
        /** Draw function.
            Function is called when it is time to draw.
         
            Derived from Drawable.
        */
        void onDraw(SkCanvas* canvas) override;
        
    private:
        // Label should only change size on setFontSize!
        using Widget::setMinSize;
        using Widget::setSize;
        using Widget::setMaxSize;
        
        void onTextUpdate() override;
    };
}

#endif // PTK_WIDGETS_LABEL_HPP
