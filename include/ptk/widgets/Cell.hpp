//
//  core/Cell.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

#ifndef PTK_CORE_CELL_HPP
#define PTK_CORE_CELL_HPP

// Local Headers
#include "ptk/core/Widget.hpp"

// C++ Headers
#include <memory>

namespace pTK
{
    class Cell : public Widget
    {
    public:
        Cell();
        Cell(const std::shared_ptr<Widget>& widget);
        ~Cell() = default;
        
        void setWidget(const std::shared_ptr<Widget>& widget);
        std::shared_ptr<Widget> getWidget() const;
        
        bool drawChild(Widget* widget) override;
        
        void setPosHint(const Position& pos) override;
        
        void setSize(const Size& size) override;
        
        // Event handling
        
        /** Function for handling when a key is pressed or released.
         
         Derived from EventFunctions.
         
         @param type    Key event (press or release)
         @param int     Keycode
         */
        bool onKeyEvent(EventType type, int32 keycode) override;
        
        /** Function for handling when mouse is hovering.
         
         Derived from EventFunctions.
         
         @param pos     position
         */
        bool onHoverEvent(const Position& pos) override;
        
        /** Function for handling when mouse is entering.
         
         Derived from EventFunctions.
         */
        bool onEnterEvent() override;
        
        /** Function for handling when mouse is leaving.
         
         Derived from EventFunctions.
         */
        bool onLeaveEvent() override;
        
        /** Function for handling when mouse is scrolling.
         
         Derived from EventFunctions.
         
         @param offset     x and y offset
         */
        bool onScrollEvent(const Vec2f& offset) override;
        
        /** Function for handling when mouse is clicking.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onClickEvent(MouseButton btn, const Position& pos) override;
        
        /** Function for handling when mouse is released.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onReleaseEvent(MouseButton btn, const Position& pos) override;
    private:
        void calculatePosition();
        
        std::shared_ptr<Widget> m_widget;
        Size m_widgetSize;
        Position m_widgetPos;
        bool m_clicked;
        bool m_hover;
    };
}


#endif // PTK_CORE_CELL_HPP
