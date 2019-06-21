//
//  core/event_callbacks.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// Local Headers
#include "ptk/core/event_callbacks.hpp"

namespace pTK
{
    EventCallbacks::EventCallbacks()
        : m_key_callback{nullptr}, m_hover_callback{nullptr}, m_leave_callback{nullptr},
            m_scroll_callback{nullptr}, m_click_callback{nullptr}, m_release_callback{nullptr}
    {
    }
    
    void EventCallbacks::on_key(const std::function<void(EventType, int)>& callback)
    {
        m_key_callback = callback;
    }
    
    void EventCallbacks::on_hover(const std::function<void()>& callback)
    {
        m_hover_callback = callback;
    }
    
    void EventCallbacks::on_leave(const std::function<void()>& callback)
    {
        m_leave_callback = callback;
    }
    
    void EventCallbacks::on_scroll(const std::function<void(const Vec2<int>&)>& callback)
    {
        m_scroll_callback = callback;
    }
    
    void EventCallbacks::on_click(const std::function<void(MouseButton, const Vec2<int>&)>& callback)
    {
        m_click_callback = callback;
    }
    
    void EventCallbacks::on_release(const std::function<void(MouseButton, const Vec2<int>&)>& callback)
    {
        m_release_callback = callback;
    }
}
