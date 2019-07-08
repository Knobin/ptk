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
        EventFunctions() = default;
        virtual ~EventFunctions() = default;
        
        /** Function for handling when a key is pressed or released.
         
         @param type    Key event (press or release)
         @param int     Keycode
         */
        virtual bool onKeyEvent(EventType, int32) { return true; }
        
        /** Function for handling when mouse is hovering.
         
         */
        virtual bool onHoverEvent(const Position&) { return true; }
        
        /** Function for handling when mouse is entering.
         
         */
        virtual bool onEnterEvent() { return true; }
        
        /** Function for handling when mouse is leaving.
         
         */
        virtual bool onLeaveEvent() { return true; }
        
        /** Function for handling when mouse is scrolling.
         
         @param offset     x and y offset
         */
        virtual bool onScrollEvent(const Vec2f&) { return true; }
        
        /** Function for handling when mouse is clicking.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        virtual bool onClickEvent(MouseButton, const Position&) { return true; }
        
        /** Function for handling when mouse is released.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        virtual bool onReleaseEvent(MouseButton, const Position&) { return true; }
    };
}

#endif // PTK_CORE_EVENTFUNCTIONS_HPP

