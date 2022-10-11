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
    uint64_t EventCallbacks::onKey(const std::function<bool(const KeyEvent&)>& callback)
    {
        return addListener<KeyEvent>(callback);
    }

    uint64_t EventCallbacks::onInput(const std::function<bool(const InputEvent&)>& callback)
    {
        return addListener<InputEvent>(callback);
    }

    uint64_t EventCallbacks::onHover(const std::function<bool(const MotionEvent&)>& callback)
    {
        return addListener<MotionEvent>(callback);
    }

    uint64_t EventCallbacks::onEnter(const std::function<bool(const EnterEvent&)>& callback)
    {
        return addListener<EnterEvent>(callback);
    }

    uint64_t EventCallbacks::onLeave(const std::function<bool(const LeaveEvent&)>& callback)
    {
        return addListener<LeaveEvent>(callback);
    }

    uint64_t EventCallbacks::onLeaveClick(const std::function<bool(const LeaveClickEvent&)>& callback)
    {
        return addListener<LeaveClickEvent>(callback);
    }

    uint64_t EventCallbacks::onScroll(const std::function<bool(const ScrollEvent&)>& callback)
    {
        return addListener<ScrollEvent>(callback);
    }

    uint64_t EventCallbacks::onClick(const std::function<bool(const ClickEvent&)>& callback)
    {
        return addListener<ClickEvent>(callback);
    }

    uint64_t EventCallbacks::onRelease(const std::function<bool(const ReleaseEvent&)>& callback)
    {
        return addListener<ReleaseEvent>(callback);
    }
} // namespace pTK
