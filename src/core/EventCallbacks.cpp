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
    uint64 EventCallbacks::onKey(const std::function<bool(const KeyEvent&)>& callback)
    {
        return addListener<KeyEvent>(callback);
    }

    uint64 EventCallbacks::onInput(const std::function<bool(const InputEvent&)>& callback)
    {
        return addListener<InputEvent>(callback);
    }

    uint64 EventCallbacks::onHover(const std::function<bool(const MotionEvent&)>& callback)
    {
        return addListener<MotionEvent>(callback);
    }

    uint64 EventCallbacks::onEnter(const std::function<bool(const EnterEvent&)>& callback)
    {
        return addListener<EnterEvent>(callback);
    }

    uint64 EventCallbacks::onLeave(const std::function<bool(const LeaveEvent&)>& callback)
    {
        return addListener<LeaveEvent>(callback);
    }

    uint64 EventCallbacks::onLeaveClick(const std::function<bool(const LeaveClickEvent&)>& callback)
    {
        return addListener<LeaveClickEvent>(callback);
    }

    uint64 EventCallbacks::onScroll(const std::function<bool(const ScrollEvent&)>& callback)
    {
        return addListener<ScrollEvent>(callback);
    }

    uint64 EventCallbacks::onClick(const std::function<bool(const ClickEvent&)>& callback)
    {
        return addListener<ClickEvent>(callback);
    }

    uint64 EventCallbacks::onRelease(const std::function<bool(const ReleaseEvent&)>& callback)
    {
        return addListener<ReleaseEvent>(callback);
    }
}
