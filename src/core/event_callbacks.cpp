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
        : m_keyCallback{nullptr}, m_hoverCallback{nullptr}, m_leaveCallback{nullptr},
            m_scrollCallback{nullptr}, m_clickCallback{nullptr}, m_releaseCallback{nullptr}
    {
    }
    
    void EventCallbacks::onKey(const std::function<void(EventType, int)>& callback)
    {
        m_keyCallback = callback;
    }
    
    void EventCallbacks::onHover(const std::function<void()>& callback)
    {
        m_hoverCallback = callback;
    }
    
    void EventCallbacks::onLeave(const std::function<void()>& callback)
    {
        m_leaveCallback = callback;
    }
    
    void EventCallbacks::onScroll(const std::function<void(const Vec2<int>&)>& callback)
    {
        m_scrollCallback = callback;
    }
    
    void EventCallbacks::onClick(const std::function<void(MouseButton, const Vec2<int>&)>& callback)
    {
        m_clickCallback = callback;
    }
    
    void EventCallbacks::onRelease(const std::function<void(MouseButton, const Vec2<int>&)>& callback)
    {
        m_releaseCallback = callback;
    }
}
