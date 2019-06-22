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
        void handle_key_event(EventType type, int keycode);
        
        /** Function for handling when mouse is entering.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         */
        void handle_hover_event();
        
        /** Function for handling when mouse is leaving.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         */
        void handle_leave_event();
        
        /** Function for handling when mouse is scrolling.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param offset     x and y offset
         */
        void handle_scroll_event(const Vec2<int>& offset);
        
        
        /** Function for handling when mouse is clicking.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        void handle_click_event(MouseButton button, const Vec2<int>& position);
        
        /** Function for handling when mouse is released.
         
         This function will call two other functions to handle the event.
         One for the user to bind to and one for derived classes to handle
         the event internally.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        void handle_release_event(MouseButton button, const Vec2<int>& position);
        
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
