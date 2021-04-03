//
//  core/EventFunctions.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-21.
//

#ifndef PTK_CORE_EVENTFUNCTIONS_HPP
#define PTK_CORE_EVENTFUNCTIONS_HPP

namespace pTK
{
    /** EventFunctions class implementation.
     
        This class is to handle the incoming events.
     
        Depending on the event category and type, one of
        the defined functions below will be called.
    */
    class EventFunctions
    {
    public:
        /** Constructs EventFunctions with default values.

        */
        EventFunctions() = default;

        /** Destructor for EventFunctions.

        */
        virtual ~EventFunctions() = default;
        
        /** Function for handling when a key is pressed or released.
         
            @param type     Key event (press or release)
            @param KeyCode  Keycode
        */
        virtual bool onKeyEvent(Event::Type, KeyCode) { return false; }
        
        /** Function for handling when mouse is hovering.
         
        */
        virtual bool onHoverEvent(const Point&) { return false; }
        
        /** Function for handling when mouse is entering.
         
        */
        virtual bool onEnterEvent() { return false; }
        
        /** Function for handling when mouse is leaving.
         
        */
        virtual bool onLeaveEvent() { return false; }
        
        /** Function for handling when mouse is scrolling.
         
         @param offset     x and y offset
        */
        virtual bool onScrollEvent(const Vec2f&) { return false; }
        
        /** Function for handling when mouse is clicking.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        virtual bool onClickEvent(Mouse::Button, const Point&) { return false; }
        
        /** Function for handling when mouse is released.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        virtual bool onReleaseEvent(Mouse::Button, const Point&) { return false; }
    };
}

#endif // PTK_CORE_EVENTFUNCTIONS_HPP

