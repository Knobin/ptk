//
//  events/WidgetEvents.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-23.
//

// Idea of this file is to create events that are specific
// to widgets and are not triggered by the OS.
//
// For example, the onEnter & onLeave mouse events that the widget
// have. These originate from the MotionEvent, but it would be
// useful for the widget to know where outside position is.
//

#ifndef PTK_EVENTS_WIDGETEVENTS_HPP
#define PTK_EVENTS_WIDGETEVENTS_HPP

// pTK Headers
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WindowEvent.hpp"

namespace pTK
{

    struct EnterEvent
    {

    };


    struct LeaveEvent
    {

    };


    struct LeaveClickEvent
    {

    };

} // namespace pTK

#endif // PTK_EVENTS_WIDGETEVENTS_HPP
