//
//  core/EventCallbacks.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// Local Headers
#include "ptk/core/EventCallbacks.hpp"

namespace pTK
{
    EventCallbacks::EventCallbacks()
        : m_keyCallback{nullptr}, m_hoverCallback{nullptr}, m_leaveCallback{nullptr},
            m_scrollCallback{nullptr}, m_clickCallback{nullptr}, m_releaseCallback{nullptr}
    {
    }
    
    void EventCallbacks::onKey(const std::function<bool(EventType, int)>& callback)
    {
        m_keyCallback = callback;
    }
    
    void EventCallbacks::onHover(const std::function<bool()>& callback)
    {
        m_hoverCallback = callback;
    }
    
    void EventCallbacks::onLeave(const std::function<bool()>& callback)
    {
        m_leaveCallback = callback;
    }
    
    void EventCallbacks::onScroll(const std::function<bool(const Vec2f&)>& callback)
    {
        m_scrollCallback = callback;
    }
    
    void EventCallbacks::onClick(const std::function<bool(MouseButton, const Position&)>& callback)
    {
        m_clickCallback = callback;
    }
    
    void EventCallbacks::onRelease(const std::function<bool(MouseButton, const Position&)>& callback)
    {
        m_releaseCallback = callback;
    }
}
