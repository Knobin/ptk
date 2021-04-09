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
    {}
    
    bool EventHandling::handleKeyEvent(Event::Type type, KeyCode keycode)
    {
        bool status{onKeyEvent(type, keycode)};

        auto& container = getKeyCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(type, keycode))
                it = container.erase(it);
            else
                ++it;
        }

        return status;
    }
    
    bool EventHandling::handleHoverEvent(const Point& pos)
    {
        bool status{onHoverEvent(pos)};

        auto& container = getHoverCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(pos))
                it = container.erase(it);
            else
                ++it;
        }
        
        return status;
    }
    
    bool EventHandling::handleEnterEvent()
    {
        bool status{onEnterEvent()};

        auto& container = getEnterCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)())
                it = container.erase(it);
            else
                ++it;
        }
        
        return status;
    }
    
    bool EventHandling::handleLeaveEvent()
    {
        bool status{onLeaveEvent()};

        auto& container = getLeaveCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)())
                it = container.erase(it);
            else
                ++it;
        }

        return status;
    }
    
    bool EventHandling::handleScrollEvent(const Vec2f& offset)
    {
        bool status{onScrollEvent(offset)};

        auto& container = getScrollCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(offset))
                it = container.erase(it);
            else
                ++it;
        }
        
        return status;
    }
    
    bool EventHandling::handleClickEvent(Mouse::Button button, const Point& position)
    {
        bool status{onClickEvent(button, position)};

        auto& container = getClickCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(button, position))
                it = container.erase(it);
            else
                ++it;
        }
        
        return status;
    }
    
    bool EventHandling::handleReleaseEvent(Mouse::Button button, const Point& position)
    {
        bool status{onReleaseEvent(button, position)};

        auto& container = getReleaseCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(button, position))
                it = container.erase(it);
            else
                ++it;
        }
        
        return status;
    }
}
