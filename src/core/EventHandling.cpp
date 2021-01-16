//
//  core/EventHandling.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// pTK Headers
#include "ptk/core/EventHandling.hpp"

namespace pTK
{

    EventHandling::EventHandling()
        : EventCallbacks(), EventFunctions()
    {
    }
    
    bool EventHandling::handleKeyEvent(Event::Type type, KeyCode keycode)
    {
        bool status = true;
        if (m_keyCallback != nullptr)
            status = m_keyCallback(type, keycode);
        
        return !status || onKeyEvent(type, keycode);
    }
    
    bool EventHandling::handleHoverEvent(const Point& pos)
    {
        bool status = true;
        
        if (m_hoverCallback != nullptr)
            status = m_hoverCallback(pos);
        
        return !status || onHoverEvent(pos);
    }
    
    bool EventHandling::handleEnterEvent()
    {
        bool status = true;
        
        if (m_enterCallback != nullptr)
            status = m_enterCallback();
        
        return !status || onEnterEvent();
    }
    
    bool EventHandling::handleLeaveEvent()
    {
        bool status = true;
        
        if (m_leaveCallback != nullptr)
            status = m_leaveCallback();

        return !status || onLeaveEvent();
    }
    
    bool EventHandling::handleScrollEvent(const Vec2f& offset)
    {
        bool status = true;
        
        if (m_scrollCallback != nullptr)
            status = m_scrollCallback(offset);
        
        return !status || onScrollEvent(offset);
    }
    
    bool EventHandling::handleClickEvent(Mouse::Button button, const Point& position)
    {
        bool status = true;
        
        if (m_clickCallback != nullptr)
            status = m_clickCallback(button, position);
        
        return !status || onClickEvent(button, position);
    }
    
    bool EventHandling::handleReleaseEvent(Mouse::Button button, const Point& position)
    {
        bool status = true;

        if (m_releaseCallback != nullptr)
            status = m_releaseCallback(button, position);
        
        return !status || onReleaseEvent(button, position);
    }
}
