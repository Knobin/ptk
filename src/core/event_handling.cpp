//
//  core/event_handling.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/core/event_handling.hpp"

namespace pTK
{

    EventHandling::EventHandling()
        : EventCallbacks(), EventFunctions()
    {
    }
    
    void EventHandling::handle_key_event(EventType type, int keycode)
    {
        on_key_event(type, keycode);
        if (m_key_callback != nullptr)
            m_key_callback(type, keycode);
    }
    
    void EventHandling::handle_hover_event()
    {
        on_hover_event();
        if (m_hover_callback != nullptr)
            m_hover_callback();
    }
    
    void EventHandling::handle_leave_event()
    {
        on_leave_event();
        if (m_leave_callback != nullptr)
            m_leave_callback();
    }
    
    void EventHandling::handle_scroll_event(const Vec2<int>& offset)
    {
        on_scroll_event(offset);
        if (m_scroll_callback != nullptr)
            m_scroll_callback(offset);
    }
    
    void EventHandling::handle_click_event(MouseButton button, const Vec2<int>& position)
    {
        on_click_event(button, position);
        if (m_click_callback != nullptr)
            m_click_callback(button, position);
    }
    
    void EventHandling::handle_release_event(MouseButton button, const Vec2<int>& position)
    {
        on_release_event(button, position);
        if (m_release_callback != nullptr)
            m_release_callback(button, position);
    }
}
