//
//  widgets/TextField.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-10.
//

#ifndef PTK_WIDGETS_TEXTFIELD_HPP
#define PTK_WIDGETS_TEXTFIELD_HPP

// pTK Headers
#include "ptk/widgets/Rectangle.hpp"
#include "ptk/core/Text.hpp"

namespace pTK
{
    /** TextField class implementation.
     
    */
    class TextField : public Rectangle, public Text
    {
    public:
        /** Constructs Label with default values.
         
            @return    default initialized Label
        */
        TextField();
        virtual ~TextField() = default;
        
        /** Draw function.
            Function is called when it is time to draw.
         
            Derived from Drawable.
        */
        void onDraw(SkCanvas* canvas) override;
        
    private:
        void onTextUpdate() override;

    private:
        Rectangle m_cursor;
        int m_offset{0};
        bool m_drawCursor{false};
    };
}

#endif // PTK_WIDGETS_TEXTFIELD_HPP

