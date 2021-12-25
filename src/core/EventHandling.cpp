//
//  core/EventHandling.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// pTK Headers
#include "ptk/core/EventHandling.hpp"
#include "ptk/core/EventManager.hpp"

namespace pTK
{
    EventHandling::EventHandling()
        : EventCallbacks(), EventFunctions()
    {}

    void EventHandling::registerMemberCallbacks()
    {
        addListener<KeyEvent>([&](const KeyEvent& evt) { onKeyEvent(evt); return false; });
        addListener<InputEvent>([&](const InputEvent& evt) { onKeyInput(evt); return false; });
        addListener<MotionEvent>([&](const MotionEvent& evt) { onHoverEvent(evt); return false; });
        addListener<EnterEvent>([&](const EnterEvent& evt) { onEnterEvent(evt); return false; });
        addListener<LeaveEvent>([&](const LeaveEvent& evt) { onLeaveEvent(evt); return false; });
        addListener<LeaveClickEvent>([&](const LeaveClickEvent& evt) { onLeaveClickEvent(evt); return false; });
        addListener<ScrollEvent>([&](const ScrollEvent& evt) { onScrollEvent(evt); return false; });
        addListener<ClickEvent>([&](const ClickEvent& evt) { onClickEvent(evt); return false; });
        addListener<ReleaseEvent>([&](const ReleaseEvent& evt) { onReleaseEvent(evt); return false; });
    }
}
