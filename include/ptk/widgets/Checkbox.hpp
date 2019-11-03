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
        
        /** Function for handling when the checkbox is toggled.
         
         @param status    status on toggle.
         */
        virtual void onToggleEvent(bool status);
        
        /** Function for handling when the checkbox is toggled.
         
         @param callback    function to call on toggle.
         */
        void onToggle(const std::function<bool(bool status)>& callback);
        
    private:
        bool m_checked = false;
        bool m_hover = false;
        bool m_click = false;
        int m_state = 0;
        Color m_checkColor;
        std::function<bool(bool status)> m_toggleCallback;
        
        void drawChecked(SkCanvas* canvas);
        void drawStates(SkCanvas* canvas);
        
        void internalToggle();
    };
    
}

#endif // PTK_WIDGETS_CHECKBOX_HPP
