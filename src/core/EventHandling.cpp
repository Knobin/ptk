//
//  core/EventHandling.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/core/EventHandling.hpp"

namespace pTK
{

    EventHandling::EventHandling()
        : EventCallbacks(), EventFunctions()
    {
    }
    
    void EventHandling::handle_key_event(EventType type, int keycode)
    {
        onKeyEvent(type, keycode);
        if (m_keyCallback != nullptr)
            m_keyCallback(type, keycode);
    }
    
    void EventHandling::handle_hover_event()
    {
        onHoverEvent();
        if (m_hoverCallback != nullptr)
            m_hoverCallback();
    }
    
    void EventHandling::handle_leave_event()
    {
        onLeaveEvent();
        if (m_leaveCallback != nullptr)
            m_leaveCallback();
    }
    
    void EventHandling::handle_scroll_event(const Vec2<int>& offset)
    {
        onScrollEvent(offset);
        if (m_scrollCallback != nullptr)
            m_scrollCallback(offset);
    }
    
    void EventHandling::handle_click_event(MouseButton button, const Vec2<int>& position)
    {
        onClickEvent(button, position);
        if (m_clickCallback != nullptr)
            m_clickCallback(button, position);
    }
    
    void EventHandling::handle_release_event(MouseButton button, const Vec2<int>& position)
    {
        onReleaseEvent(button, position);
        if (m_releaseCallback != nullptr)
            m_releaseCallback(button, position);
    }
}
