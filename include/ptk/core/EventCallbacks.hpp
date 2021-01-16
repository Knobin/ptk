//
//  events/EventCallbacks.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENTCALLBACKS_HPP
#define PTK_CORE_EVENTCALLBACKS_HPP

// pTK Headers
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <functional>

namespace pTK
{
    class EventCallbacks
    {
    public:
        /** Constructs EventCallbacks with default values.

        */
        EventCallbacks();

        /** Copy Constructor for EventCallbacks.

            Note: Event callbacks will not be copied!

            @param other    to copy from
            @return         EventCallbacks copy of other
        */
        EventCallbacks(const EventCallbacks& other);

        /** Copy Constructor for EventCallbacks.

            @param other    to move from
            @return         EventCallbacks with values moved from other
        */
        EventCallbacks(EventCallbacks&& other);

        /** Deconstructor for EventCallbacks.

        */
        virtual ~EventCallbacks() = default;

        /** Copy Assignment for EventCallbacks.

            Note: Event callbacks will not be copied!

            @param other    to copy from
            @return         EventCallbacks copy of other
        */
        EventCallbacks& operator=(const EventCallbacks& other);

        /** Move Assignment for EventCallbacks.

            @param other    to move from
            @return         EventCallbacks with values moved from other
        */
        EventCallbacks& operator=(EventCallbacks&& other);
        
        /** Function for handling when a key is pressed or released.
         
            @param callback    function to call on key event
        */
        void onKey(const std::function<bool(Event::Type, KeyCode)>& callback);
        
        /** Function for handling when mouse is hovering.
         
            @param callback    function to call on hover event
        */
        void onHover(const std::function<bool(const Point&)>& callback);
        
        /** Function for handling when mouse is entering.
         
            @param callback    function to call on hover event
        */
        void onEnter(const std::function<bool()>& callback);
        
        /** Function for handling when mouse is leaving.
         
            @param callback    function to call on leaving event
        */
        void onLeave(const std::function<bool()>& callback);
        
        /** Function for handling when mouse is scrolling.
         
            @param offset     x and y offset
        */
        void onScroll(const std::function<bool(const Vec2f&)>& callback);
        
        /** Function for handling when mouse is clicking.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onClick(const std::function<bool(Mouse::Button, const Point&)>& callback);
        
        /** Function for handling when mouse is released.
         
            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onRelease(const std::function<bool(Mouse::Button, const Point&)>& callback);
        
    protected:
        std::function<bool(Event::Type, KeyCode)> m_keyCallback;
        std::function<bool(const Point& pos)> m_hoverCallback;
        std::function<bool()> m_enterCallback;
        std::function<bool()> m_leaveCallback;
        std::function<bool(const Vec2f&)> m_scrollCallback;
        std::function<bool(Mouse::Button, const Point&)> m_clickCallback;
        std::function<bool(Mouse::Button, const Point&)> m_releaseCallback;
    };
}

#endif // PTK_CORE_EVENTCALLBACKS_HPP
