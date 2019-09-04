//
//  core/Cell.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

#ifndef PTK_CORE_CELL_HPP
#define PTK_CORE_CELL_HPP

// Local Headers
#include "ptk/widgets/Rectangle.hpp"

// C++ Headers
#include <memory>

namespace pTK
{
    /** Cell class implementation.
     
     Derived from Widget, this class for a Widget for controlling
     padding and layout style inside of the Cell.
     */
    class Cell : public Rectangle
    {
    public:
        /** Constructs Cell with default values.
         
         @return    default initialized Cell
         */
        Cell();
        
        /** Constructs Cell with default values with widget.
         
         @return    default initialized Cell
         */
        Cell(const Ref<Widget>& widget);
        ~Cell() = default;
        
        /** Function for setting the Widget of the Cell.

         @param widget  Widget to set
         */
        void setWidget(const Ref<Widget>& widget);
        
        /** Function for setting the Widget of the Cell.
         
         @return    current set Widget
         */
        Ref<Widget> getWidget() const;
        
        /** Function for a child Widget to call the Parent when it
         need to be drawn.
         
         Derived from Widget.
         
         @param widget  Child Widget
         @return        Status (true if handled)
         */
        bool drawChild(Widget* widget) override;
        
        /** Function for setting the position of the VBox and its
         children.
         
         Derived from Widget.
         
         @param pos     Position to set
         */
        void setPosHint(const Position& pos) override;
        
        /** Function for setting the size of the VBox and its
         children.
         
         Derived from Widget.
         
         @param newSize     Size to set
         */
        void setSize(const Size& size) override;
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         @param canvas  canvas to draw to
         */
        void onDraw(SkCanvas* canvas) override;
        
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
        
        Ref<Widget> m_widget;
        Size m_widgetSize;
        Position m_widgetPos;
        bool m_clicked;
        bool m_hover;
    };
}


#endif // PTK_CORE_CELL_HPP
