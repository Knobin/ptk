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

    EventCallbacks::EventCallbacks(const EventCallbacks&)
        : m_keyCallback{nullptr}, m_hoverCallback{nullptr}, m_leaveCallback{nullptr},
            m_scrollCallback{nullptr}, m_clickCallback{nullptr}, m_releaseCallback{nullptr}
    {
        // Callbacks should be copied!
    }

    EventCallbacks::EventCallbacks(EventCallbacks&& other)
    {
        std::swap(m_keyCallback, other.m_keyCallback);
        std::swap(m_hoverCallback, other.m_hoverCallback);
        std::swap(m_enterCallback, other.m_enterCallback);
        std::swap(m_leaveCallback, other.m_leaveCallback);
        std::swap(m_scrollCallback, other.m_scrollCallback);
        std::swap(m_clickCallback, other.m_clickCallback);
        std::swap(m_releaseCallback, other.m_releaseCallback);
    }

    EventCallbacks& EventCallbacks::operator=(const EventCallbacks& other)
    {
        if (this == &other)
            return *this;

        // Do not copy the event callbacks!
        return *this;
    }

    EventCallbacks& EventCallbacks::operator=(EventCallbacks&& other)
    {
        if (this == &other)
            return *this;

        m_keyCallback       = std::move(other.m_keyCallback);
        m_hoverCallback     = std::move(other.m_hoverCallback);
        m_enterCallback     = std::move(other.m_enterCallback);
        m_leaveCallback     = std::move(other.m_leaveCallback);
        m_scrollCallback    = std::move(other.m_scrollCallback);
        m_clickCallback     = std::move(other.m_clickCallback);
        m_releaseCallback   = std::move(other.m_releaseCallback);
        return *this;
    }
    
    void EventCallbacks::onKey(const std::function<bool(Event::Type, int32)>& callback)
    {
        m_keyCallback = callback;
    }
    
    void EventCallbacks::onHover(const std::function<bool(const Point&)>& callback)
    {
        m_hoverCallback = callback;
    }
    
    void EventCallbacks::onEnter(const std::function<bool()>& callback)
    {
        m_enterCallback = callback;
    }
    
    void EventCallbacks::onLeave(const std::function<bool()>& callback)
    {
        m_leaveCallback = callback;
    }
    
    void EventCallbacks::onScroll(const std::function<bool(const Vec2f&)>& callback)
    {
        m_scrollCallback = callback;
    }
    
    void EventCallbacks::onClick(const std::function<bool(Mouse::Button, const Point&)>& callback)
    {
        m_clickCallback = callback;
    }
    
    void EventCallbacks::onRelease(const std::function<bool(Mouse::Button, const Point&)>& callback)
    {
        m_releaseCallback = callback;
    }
}
