//
//  events/event_callbacks.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENT_CALLBACKS_HPP
#define PTK_CORE_EVENT_CALLBACKS_HPP

// Local Headers
#include "ptk/events/key_event.hpp"
#include "ptk/events/mouse_event.hpp"
#include "ptk/util/vec2.hpp"

// C++ Headers
#include <functional>

namespace pTK
{
    class EventCallbacks
    {
    public:
        EventCallbacks();
        virtual ~EventCallbacks() = default;
        
        /** Function for handling when a key is pressed or released.
         
         @param callback    function to call on key event
         */
        void onKey(const std::function<void(EventType, int)>& callback);
        
        /** Function for handling when mouse is entering.
         
         @param callback    function to call on hover event
         */
        void onHover(const std::function<void()>& callback);
        
        /** Function for handling when mouse is leaving.
         
         @param callback    function to call on leaving event
         */
        void onLeave(const std::function<void()>& callback);
        
        /** Function for handling when mouse is scrolling.
         
         @param offset     x and y offset
         */
        void onScroll(const std::function<void(const Vec2<int>&)>& callback);
        
        /** Function for handling when mouse is clicking.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        void onClick(const std::function<void(MouseButton, const Vec2<int>&)>& callback);
        
        /** Function for handling when mouse is released.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        void onRelease(const std::function<void(MouseButton, const Vec2<int>&)>& callback);
        
    protected:
        std::function<void(EventType, int)> m_keyCallback;
        std::function<void()> m_hoverCallback;
        std::function<void()> m_leaveCallback;
        std::function<void(const Vec2<int>&)> m_scrollCallback;
        std::function<void(MouseButton, const Vec2<int>&)> m_clickCallback;
        std::function<void(MouseButton, const Vec2<int>&)> m_releaseCallback;
    };
}

#endif // PTK_CORE_EVENT_CALLBACKS_HPP
