//
//  core/EventHandling.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_CORE_EVENTHANDLING_HPP
#define PTK_CORE_EVENTHANDLING_HPP

// Local Headers
#include "ptk/core/EventCallbacks.hpp"
#include "ptk/core/EventFunctions.hpp"

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
        EventHandling();
        virtual ~EventHandling() = default;
        
        /** Function for handling when a key is pressed or released.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param type    Key event (press or release)
         @param int     Keycode
         */
        bool handleKeyEvent(EventType type, int32 keycode);
        
        /** Function for handling when mouse is entering.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         */
        bool handleHoverEvent();
        
        /** Function for handling when mouse is leaving.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         */
        bool handleLeaveEvent();
        
        /** Function for handling when mouse is scrolling.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param offset     x and y offset
         */
        bool handleScrollEvent(const Vec2f& offset);
        
        
        /** Function for handling when mouse is clicking.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool handleClickEvent(MouseButton button, const Position& position);
        
        /** Function for handling when mouse is released.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool handleReleaseEvent(MouseButton button, const Position& position);
        
    private:
        using EventCallbacks::m_keyCallback;
        using EventCallbacks::m_hoverCallback;
        using EventCallbacks::m_leaveCallback;
        using EventCallbacks::m_scrollCallback;
        using EventCallbacks::m_clickCallback;
        using EventCallbacks::m_releaseCallback;
    };
}

#endif // PTK_CORE_EVENTHANDLING_HPP
