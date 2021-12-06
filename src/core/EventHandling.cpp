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

    template<typename Container, typename... Args>
    static void LoopAndConditionRemove(Container& cont, Args&&... args)
    {
        auto it = cont.begin();
        while (it != cont.end())
        {
            if ((*it)(std::forward<Args>(args)...))
                it = cont.erase(it);
            else
                ++it;
        }
    }

    void EventHandling::handleKeyEvent(Event::Type type, KeyCode keycode, byte modifier)
    {
        onKeyEvent(type, keycode, modifier);
        LoopAndConditionRemove(getKeyCallbacks(), type, keycode, modifier);
    }

    void EventHandling::handleKeyInput(KeyCode keycode, uint32 data, byte modifier)
    {
        onKeyInput(keycode, data, modifier);
        LoopAndConditionRemove(getKeyInputCallbacks(), keycode, data, modifier);
    }

    void EventHandling::handleHoverEvent(const Point& pos)
    {
        onHoverEvent(pos);
        LoopAndConditionRemove(getHoverCallbacks(), pos);
    }

    void EventHandling::handleEnterEvent()
    {
        onEnterEvent();
        LoopAndConditionRemove(getEnterCallbacks());
    }

    void EventHandling::handleLeaveEvent()
    {
        onLeaveEvent();
        LoopAndConditionRemove(getLeaveCallbacks());
    }

    void EventHandling::handleLeaveClickEvent()
    {
        onLeaveClickEvent();
        LoopAndConditionRemove(getLeaveClickCallbacks());
    }

    void EventHandling::handleScrollEvent(const Vec2f& offset)
    {
        onScrollEvent(offset);
        LoopAndConditionRemove(getScrollCallbacks(), offset);
    }

    void EventHandling::handleClickEvent(Mouse::Button button, const Point& position)
    {
        onClickEvent(button, position);
        LoopAndConditionRemove(getClickCallbacks(), button, position);
    }

    void EventHandling::handleReleaseEvent(Mouse::Button button, const Point& position)
    {
        onReleaseEvent(button, position);
        LoopAndConditionRemove(getReleaseCallbacks(), button, position);
    }
}
