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
        virtual void onKeyEvent(Event::Type, KeyCode, byte) {}

        /** Function for handling key input.

            @param keyCode  Keycode
            @param char     display character
            @param mod      modifier keys
        */
        virtual void onKeyInput(KeyCode, uint32, Text::Encoding, byte) {}
        
        /** Function for handling when mouse is hovering.
         
        */
        virtual void onHoverEvent(const Point&) {}
        
        /** Function for handling when mouse is entering.
         
        */
        virtual void onEnterEvent() {}
        
        /** Function for handling when mouse is leaving.
         
        */
        virtual void onLeaveEvent() {}

        // TODO: Add documentation.
        virtual void onLeaveClickEvent() {}
        
        /** Function for handling when mouse is scrolling.
         
         @param offset     x and y offset
        */
        virtual void onScrollEvent(const Vec2f&) {}
        
        /** Function for handling when mouse is clicking.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        virtual void onClickEvent(Mouse::Button, const Point&) {}
        
        /** Function for handling when mouse is released.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        virtual void onReleaseEvent(Mouse::Button, const Point&) {}
    };
}

#endif // PTK_CORE_EVENTFUNCTIONS_HPP

