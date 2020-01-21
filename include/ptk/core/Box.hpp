//
//  core/Box.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

#ifndef PTK_CORE_BOX_HPP
#define PTK_CORE_BOX_HPP

// Local Headers
#include "ptk/core/Container.hpp"
#include "ptk/core/Widget.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** Box class implementation.
     
        Derived from Container, this class for holding Widgets in
        order of displaying them in a vertical style.

        Be sure to override these functions:
            - virtual void setPosHint(const Point& pos);
            - virtual void setSize(const Size& newSize);
            - virtual void drawContent(SkCanvas *canvas);
    */
    class Box : public Container<Ref<Widget>>, public Widget
    {
    public:
        /** Constructs Box with default values.
         
            @return    default initialized Box
         */
        Box();
        virtual ~Box();

        /** Function for adding a Widget
         
            Derived from Container.
         
            @param widget  Widget to add
            @return        Status (true if added)
        */
        bool add(const Ref<Widget>& widget) override;
        
        /** Function for removing a Widget
         
            Derived from Container.
            
            @param widget  Widget to remove
        */
        void remove(const Ref<Widget>& widget) override;
        
        /** Draw function.
            Function is called when it is time to draw.
        
            Derived from Drawable.
            @param canvas  canvas to draw to
        */
        void onDraw(SkCanvas* canvas) override;

        /** Function for a child Widget to call the Parent when it
            need to be drawn.
         
            Derived from Widget.
         
            @param widget  Child Widget
            @return        Status (true if handled)
        */
        bool updateChild(Widget* widget) override;
        
        /** Function for a child Widget to call the Parent when it
            need to be drawn.
         
             Derived from Widget.
         
            @param widget  Child Widget
            @return        Status (true if handled)
        */
        bool drawChild(Widget* widget) override;

        /** Funtion for finding a widget in the Container.
            Function will look for a widget with the specified name and it
            will grab the first with the matching name.
         
            Meaning that if two Widgets or more have the same name, only the
            first added will be found.
         
            @param name    name to find
            @return        const_iterator to widget or end const_iterator
        */
        const_iterator find(const std::string& name);
        
        /** Funtion for finding a widget in the Container.
            Function can be used to check if a pointer to Widget exists.
         
            @param widget  widget to find
            @return        const_iterator to widget or end const_iterator
        */
        const_iterator findRaw(const Widget* widget);
        
        /** Funtion for finding a widget in the Container.
         
            @param pos     pos to find
            @return        const_iterator to widget or end const_iterator
        */
        const_iterator find(const Point& pos);
        
        /** Funtion for finding a widget in the Container.
            In reverse order.
         
            @param pos     pos to find
            @return        const_iterator to widget or end const_iterator
        */
        reverse_iterator rfind(const Point& pos);
        
        /** Function for handling when a key is pressed or released.
         
            Derived from EventFunctions.
         
            @param type    Key event (press or release)
            @param int     Keycode
        */
        bool onKeyEvent(Event::Type type, int32 keycode) override;
        
        /** Function for handling when mouse is hovering.
         
            Derived from EventFunctions.
         
            @param pos     position
        */
        bool onHoverEvent(const Point& pos) override;
        
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
        bool onClickEvent(Mouse::Button btn, const Point& pos) override;
        
        /** Function for handling when mouse is released.
         
            Derived from EventFunctions.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        bool onReleaseEvent(Mouse::Button btn, const Point& pos) override;
        
        /** Funtion for setting the background of the Container.
         
            @param color   background color
        */
        void setBackground(const Color& color);
        
        /** Funtion for getting the background of the Container.
         
            @return    current background color
        */
        const Color& getBackground() const;
        
    private:
        virtual void onAdd(const Ref<Widget>&) {}
        virtual void onRemove(const Ref<Widget>&) {}
        virtual void onChildUpdate(size_type) {}
        virtual void onChildDraw(size_type) {}
        
    private:
        // Variables
        Color m_background;
        Widget* m_lastClickedWidget;
        Widget* m_currentHoverWidget;
        bool m_busy;
    };
}

#endif // PTK_CORE_BOX_HPP
