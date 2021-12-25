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
    void EventCallbacks::onKey(const std::function<bool(const KeyEvent&)>& callback)
    {
        addListener<KeyEvent>(callback);
    }

    void EventCallbacks::onInput(const std::function<bool(const InputEvent&)>& callback)
    {
        addListener<InputEvent>(callback);
    }

    void EventCallbacks::onHover(const std::function<bool(const MotionEvent&)>& callback)
    {
        addListener<MotionEvent>(callback);
    }

    void EventCallbacks::onEnter(const std::function<bool(const EnterEvent&)>& callback)
    {
        addListener<EnterEvent>(callback);
    }

    void EventCallbacks::onLeave(const std::function<bool(const LeaveEvent&)>& callback)
    {
        addListener<LeaveEvent>(callback);
    }

    void EventCallbacks::onLeaveClick(const std::function<bool(const LeaveClickEvent&)>& callback)
    {
        addListener<LeaveClickEvent>(callback);
    }

    void EventCallbacks::onScroll(const std::function<bool(const ScrollEvent&)>& callback)
    {
        addListener<ScrollEvent>(callback);
    }

    void EventCallbacks::onClick(const std::function<bool(const ClickEvent&)>& callback)
    {
        addListener<ClickEvent>(callback);
    }

    void EventCallbacks::onRelease(const std::function<bool(const ReleaseEvent&)>& callback)
    {
        addListener<ReleaseEvent>(callback);
    }
}
