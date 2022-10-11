//
//  core/EventFunctions.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-21.
//

#ifndef PTK_CORE_EVENTFUNCTIONS_HPP
#define PTK_CORE_EVENTFUNCTIONS_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WidgetEvents.hpp"

namespace pTK
{
    /** EventFunctions class implementation.

        This class contains functions that are called when some common events
        are triggered / handled.

        Override any of the functions if you want to handle that event.
    */
    class PTK_API EventFunctions
    {
    public:
        /** Constructs EventFunctions with default values.

        */
        EventFunctions() = default;

        /** Destructor for EventFunctions.

        */
        virtual ~EventFunctions() = default;

        /** Function callback for KeyEvent.

            Override this function if you want to handle KeyEvent.

            @param evt     KeyEvent
        */
        virtual void onKeyEvent(const KeyEvent& UNUSED(evt)) {}

        /** Function callback for InputEvent.

            Override this function if you want to handle InputEvent.

            @param evt     InputEvent
        */
        virtual void onKeyInput(const InputEvent&) {}

        /** Function callback for MotionEvent.

            Override this function if you want to handle MotionEvent.

            @param evt     MotionEvent
        */
        virtual void onHoverEvent(const MotionEvent&) {}

        /** Function for handling when mouse is entering.

        */
        virtual void onEnterEvent(const EnterEvent&) {}

        /** Function callback for LeaveEvent.

            Override this function if you want to handle LeaveEvent.

            @param evt     LeaveEvent
        */
        virtual void onLeaveEvent(const LeaveEvent&) {}

        /** Function callback for LeaveClickEvent.

            Override this function if you want to handle LeaveClickEvent.

            @param evt     LeaveClickEvent
        */
        virtual void onLeaveClickEvent(const LeaveClickEvent&) {}

        /** Function callback for ScrollEvent.

            Override this function if you want to handle ScrollEvent.

            @param evt     ScrollEvent
        */
        virtual void onScrollEvent(const ScrollEvent&) {}

        /** Function callback for ClickEvent.

            Override this function if you want to handle ClickEvent.

            @param evt     ClickEvent
        */
        virtual void onClickEvent(const ClickEvent&) {}

        /** Function callback for ReleaseEvent.

            Override this function if you want to handle ReleaseEvent.

            @param evt     ReleaseEvent
        */
        virtual void onReleaseEvent(const ReleaseEvent&) {}
    };
} // namespace pTK

#endif // PTK_CORE_EVENTFUNCTIONS_HPP
