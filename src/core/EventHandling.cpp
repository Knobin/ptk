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
    
    bool EventHandling::handleKeyEvent(Event::Type type, int32 keycode)
    {
        bool status = true;
        if (m_keyCallback != nullptr)
            status = m_keyCallback(type, keycode);
        
        if (status)
            status = onKeyEvent(type, keycode);
        
        return status;
    }
    
    bool EventHandling::handleHoverEvent(const Point& pos)
    {
        bool status = true;
        
        if (m_hoverCallback != nullptr)
            status = m_hoverCallback(pos);
        
        if (status)
            status = onHoverEvent(pos);
        
        return status;
    }
    
    bool EventHandling::handleEnterEvent()
    {
        bool status = true;
        
        if (m_enterCallback != nullptr)
            status = m_enterCallback();
        
        if (status)
            status = onEnterEvent();
        
        return status;
    }
    
    bool EventHandling::handleLeaveEvent()
    {
        bool status = true;
        
        if (m_leaveCallback != nullptr)
            status = m_leaveCallback();
        
        if (status)
            status = onLeaveEvent();
        
        return status;
    }
    
    bool EventHandling::handleScrollEvent(const Vec2f& offset)
    {
        bool status = true;
        
        if (m_scrollCallback != nullptr)
            status = m_scrollCallback(offset);
        
        if (status)
            status = onScrollEvent(offset);
        
        return status;
    }
    
    bool EventHandling::handleClickEvent(Mouse::Button button, const Point& position)
    {
        bool status = true;
        
        if (m_clickCallback != nullptr)
            status = m_clickCallback(button, position);
        
        if (status)
            status = onClickEvent(button, position);
        
        return status;
    }
    
    bool EventHandling::handleReleaseEvent(Mouse::Button button, const Point& position)
    {
        bool status = true;
        
        if (m_releaseCallback != nullptr)
            status = m_releaseCallback(button, position);
        
        if (status)
            status = onReleaseEvent(button, position);
        
        return status;
    }
}
