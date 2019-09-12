//
//  widgets/Checkbox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-11.
//

#ifndef PTK_WIDGETS_CHECKBOX_HPP
#define PTK_WIDGETS_CHECKBOX_HPP

// Local Headers
#include "ptk/widgets/Rectangle.hpp"

namespace pTK
{
    class Checkbox : public Rectangle
    {
    public:
        Checkbox();
        virtual ~Checkbox() = default;
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         */
        void onDraw(SkCanvas* canvas) override;
        
        bool status() const;
        void set(bool status);
        bool toggle();
        
        /** Function for handling when mouse is entering.
         
         Derived from EventFunctions.
         */
        bool onEnterEvent() override;
        
        /** Function for handling when mouse is leaving.
         
         Derived from EventFunctions.
         */
        bool onLeaveEvent() override;
        
        /** Function for handling when mouse is clicking.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onClickEvent(MouseButton, const Point&) override;
        
        /** Function for handling when mouse is released.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onReleaseEvent(MouseButton, const Point&) override;
        
    private:
        bool m_checked = false;
        bool m_hover = false;
        bool m_click = false;
        int m_state = 0;
        Color m_checkColor;
        
        void drawChecked(SkCanvas* canvas);
        void drawStates(SkCanvas* canvas);
        
        void internalToggle();
    };
    
}

#endif // PTK_WIDGETS_CHECKBOX_HPP
