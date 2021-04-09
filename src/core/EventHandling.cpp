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

    void EventHandling::handleKeyEvent(Event::Type type, KeyCode keycode)
    {
        onKeyEvent(type, keycode);

        auto& container = getKeyCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(type, keycode))
                it = container.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleHoverEvent(const Point& pos)
    {
        onHoverEvent(pos);

        auto& container = getHoverCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(pos))
                it = container.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleEnterEvent()
    {
        onEnterEvent();

        auto& container = getEnterCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)())
                it = container.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleLeaveEvent()
    {
        onLeaveEvent();

        auto& container = getLeaveCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)())
                it = container.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleScrollEvent(const Vec2f& offset)
    {
        onScrollEvent(offset);

        auto& container = getScrollCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(offset))
                it = container.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleClickEvent(Mouse::Button button, const Point& position)
    {
        onClickEvent(button, position);

        auto& container = getClickCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(button, position))
                it = container.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleReleaseEvent(Mouse::Button button, const Point& position)
    {
        onReleaseEvent(button, position);

        auto& container = getReleaseCallbacks();
        auto it = container.begin();
        while (it != container.end())
        {
            if ((*it)(button, position))
                it = container.erase(it);
            else
                ++it;
        }
    }
}
