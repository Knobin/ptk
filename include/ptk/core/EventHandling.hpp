//
//  core/EventHandling.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_CORE_EVENTHANDLING_HPP
#define PTK_CORE_EVENTHANDLING_HPP

// pTK Headers
#include "ptk/core/EventCallbacks.hpp"
#include "ptk/core/EventFunctions.hpp"
#include "ptk/events/KeyCodes.hpp"

namespace pTK
{
    /** EventHandling class implementation.
     
        This class is to handle the incoming events.
     
        Depending on the event category and type, one of
        the defined functions below will be called.
    */
    class EventHandling : public EventCallbacks, public EventFunctions
    {
    public:
        /** Constructs EventHandling with default values.

        */
        EventHandling();

        /** Destructor for EventHandling.

        */
        virtual ~EventHandling() = default;
        
        /** Function for handling when a key is pressed or released.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
         
            @param type    Key event (press or release)
            @param int     Keycode
        */
        void handleKeyEvent(Event::Type type, KeyCode keycode, byte modifier);
        
        /** Function for handling when mouse is hovering.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
        */
        void handleHoverEvent(const Point&);
        
        /** Function for handling when mouse is entering.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
         */
        void handleEnterEvent();
        
        /** Function for handling when mouse is leaving.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
        */
        void handleLeaveEvent();

        // TODO: Add documentation.
        void handleLeaveClickEvent();
        
        /** Function for handling when mouse is scrolling.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
         
            @param offset     x and y offset
        */
        void handleScrollEvent(const Vec2f& offset);
        
        
        /** Function for handling when mouse is clicking.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void handleClickEvent(Mouse::Button button, const Point& position);
        
        /** Function for handling when mouse is released.
         
            This function will call two other functions to handle the event.
            One for the user to bind to and one for derived classes to handle
            the event internally.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void handleReleaseEvent(Mouse::Button button, const Point& position);
    };
}

#endif // PTK_CORE_EVENTHANDLING_HPP
