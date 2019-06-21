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
        void on_key(const std::function<void(EventType, int)>& callback);
        
        /** Function for handling when mouse is entering.
         
         @param callback    function to call on hover event
         */
        void on_hover(const std::function<void()>& callback);
        
        /** Function for handling when mouse is leaving.
         
         @param callback    function to call on leaving event
         */
        void on_leave(const std::function<void()>& callback);
        
        /** Function for handling when mouse is scrolling.
         
         @param offset     x and y offset
         */
        void on_scroll(const std::function<void(const Vec2<int>&)>& callback);
        
        /** Function for handling when mouse is clicking.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        void on_click(const std::function<void(MouseButton, const Vec2<int>&)>& callback);
        
        /** Function for handling when mouse is released.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        void on_release(const std::function<void(MouseButton, const Vec2<int>&)>& callback);
        
    protected:
        std::function<void(EventType, int)> m_key_callback;
        std::function<void()> m_hover_callback;
        std::function<void()> m_leave_callback;
        std::function<void(const Vec2<int>&)> m_scroll_callback;
        std::function<void(MouseButton, const Vec2<int>&)> m_click_callback;
        std::function<void(MouseButton, const Vec2<int>&)> m_release_callback;
    };
}

#endif // PTK_CORE_EVENT_CALLBACKS_HPP
