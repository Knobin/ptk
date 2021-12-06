//
//  core/EventCallbacks.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// pTK Headers
#include "ptk/core/EventCallbacks.hpp"

namespace pTK
{
    EventCallbacks::EventCallbacks()
        : m_noArgsCallbacks{}, m_clickCallbacks{}, m_scrollCallback{}, m_keyCallback{}, m_hoverCallback{}
    {
        initCallbacks();
    }

    EventCallbacks::EventCallbacks(const EventCallbacks&)
        : m_noArgsCallbacks{}, m_clickCallbacks{}, m_scrollCallback{}, m_keyCallback{}, m_hoverCallback{}
    {
        // Callbacks should not be copied!
        initCallbacks();
    }

    void EventCallbacks::initCallbacks()
    {
        m_noArgsCallbacks["Enter"]      = {};
        m_noArgsCallbacks["Leave"]      = {};
        m_clickCallbacks["Click"]       = {};
        m_clickCallbacks["Release"]     = {};
        m_noArgsCallbacks["LeaveClick"] = {};
    }

    EventCallbacks::EventCallbacks(EventCallbacks&& other)
    {
        std::swap(m_noArgsCallbacks, other.m_noArgsCallbacks);
        std::swap(m_clickCallbacks, other.m_clickCallbacks);
        std::swap(m_scrollCallback, other.m_scrollCallback);
        std::swap(m_keyCallback, other.m_keyCallback);
        std::swap(m_keyInputCallback, other.m_keyInputCallback);
        std::swap(m_hoverCallback, other.m_hoverCallback);
    }

    EventCallbacks& EventCallbacks::operator=(const EventCallbacks& other)
    {
        if (this == &other)
            return *this;

        // Do not copy the event callbacks!
        initCallbacks();

        return *this;
    }

    EventCallbacks& EventCallbacks::operator=(EventCallbacks&& other)
    {
        if (this == &other)
            return *this;

        m_noArgsCallbacks   = std::move(other.m_noArgsCallbacks);
        m_clickCallbacks    = std::move(other.m_clickCallbacks);
        m_scrollCallback    = std::move(other.m_scrollCallback);
        m_keyCallback       = std::move(other.m_keyCallback);
        m_keyInputCallback  = std::move(other.m_keyInputCallback);
        m_hoverCallback     = std::move(other.m_hoverCallback);

        return *this;
    }
    
    void EventCallbacks::onKey(const std::function<bool(Event::Type, KeyCode, byte)>& callback)
    {
        m_keyCallback.push_back(callback);
    }

    void EventCallbacks::onInput(const std::function<bool(KeyCode, uint32, Text::Encoding, byte)>& callback)
    {
        m_keyInputCallback.push_back(callback);
    }
    
    void EventCallbacks::onHover(const std::function<bool(const Point&)>& callback)
    {
        m_hoverCallback.push_back(callback);
    }
    
    void EventCallbacks::onEnter(const std::function<bool()>& callback)
    {
        m_noArgsCallbacks["Enter"].push_back(callback);
    }
    
    void EventCallbacks::onLeave(const std::function<bool()>& callback)
    {
        m_noArgsCallbacks["Leave"].push_back(callback);
    }

    void EventCallbacks::onLeaveClick(const std::function<bool()>& callback)
    {
        m_noArgsCallbacks["LeaveClick"].push_back(callback);
    }
    
    void EventCallbacks::onScroll(const std::function<bool(const Vec2f&)>& callback)
    {
        m_scrollCallback.push_back(callback);
    }
    
    void EventCallbacks::onClick(const std::function<bool(Mouse::Button, const Point&)>& callback)
    {
        m_clickCallbacks["Click"].push_back(callback);
    }
    
    void EventCallbacks::onRelease(const std::function<bool(Mouse::Button, const Point&)>& callback)
    {
        m_clickCallbacks["Release"].push_back(callback);
    }

    const std::vector<std::function<bool()>>& EventCallbacks::getEnterCallbacks() const
    {
        // Enter is guaranteed to be in the map.
        // If not, its a bug.
        return m_noArgsCallbacks.find("Enter")->second;
    }

    const std::vector<std::function<bool()>>& EventCallbacks::getLeaveCallbacks() const
    {
        // Leave is guaranteed to be in the map.
        // If not, its a bug.
        return m_noArgsCallbacks.find("Leave")->second;
    }

    const std::vector<std::function<bool(Mouse::Button, const Point&)>>& EventCallbacks::getClickCallbacks() const
    {
        // Click is guaranteed to be in the map.
        // If not, its a bug.
        return m_clickCallbacks.find("Click")->second;
    }

    const std::vector<std::function<bool(Mouse::Button, const Point&)>>& EventCallbacks::getReleaseCallbacks() const
    {
        // Release is guaranteed to be in the map.
        // If not, its a bug.
        return m_clickCallbacks.find("Release")->second;
    }

    const std::vector<std::function<bool(const Vec2f&)>>& EventCallbacks::getScrollCallbacks() const
    {
        return m_scrollCallback;
    }

    const std::vector<std::function<bool(Event::Type, KeyCode, byte)>>& EventCallbacks::getKeyCallbacks() const
    {
        return m_keyCallback;
    }

    const std::vector<std::function<bool(KeyCode, uint32, Text::Encoding, byte)>>& EventCallbacks::getKeyInputCallbacks() const
    {
        return m_keyInputCallback;
    }

    const std::vector<std::function<bool(const Point&)>>& EventCallbacks::getHoverCallbacks() const
    {
        return m_hoverCallback;
    }

    const std::vector<std::function<bool()>>& EventCallbacks::getLeaveClickCallbacks() const
    {
        return m_noArgsCallbacks.find("LeaveClick")->second;
    }

    std::vector<std::function<bool()>>& EventCallbacks::getEnterCallbacks()
    {
        // Enter is guaranteed to be in the map.
        // If not, its a bug.
        return m_noArgsCallbacks.find("Enter")->second;
    }

    std::vector<std::function<bool()>>& EventCallbacks::getLeaveCallbacks()
    {
        // Leave is guaranteed to be in the map.
        // If not, its a bug.
        return m_noArgsCallbacks.find("Leave")->second;
    }

    std::vector<std::function<bool(Mouse::Button, const Point&)>>& EventCallbacks::getClickCallbacks()
    {
        // Click is guaranteed to be in the map.
        // If not, its a bug.
        return m_clickCallbacks.find("Click")->second;
    }

    std::vector<std::function<bool(Mouse::Button, const Point&)>>& EventCallbacks::getReleaseCallbacks()
    {
        // Release is guaranteed to be in the map.
        // If not, its a bug.
        return m_clickCallbacks.find("Release")->second;
    }

    std::vector<std::function<bool(const Vec2f&)>>& EventCallbacks::getScrollCallbacks()
    {
        return m_scrollCallback;
    }

    std::vector<std::function<bool(Event::Type, KeyCode, byte)>>& EventCallbacks::getKeyCallbacks()
    {
        return m_keyCallback;
    }

    std::vector<std::function<bool(KeyCode, uint32, Text::Encoding, byte)>>& EventCallbacks::getKeyInputCallbacks()
    {
        return m_keyInputCallback;
    }

    std::vector<std::function<bool(const Point&)>>& EventCallbacks::getHoverCallbacks()
    {
        return m_hoverCallback;
    }

    std::vector<std::function<bool()>>& EventCallbacks::getLeaveClickCallbacks()
    {
        return m_noArgsCallbacks.find("LeaveClick")->second;
    }
}
