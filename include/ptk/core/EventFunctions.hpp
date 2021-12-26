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
#include "ptk/core/Text.hpp"
#include "ptk/events/WidgetEvents.hpp"

namespace pTK
{
    /** EventFunctions class implementation.

        This class is to handle the incoming events.

        Depending on the event category and type, one of
        the defined functions below will be called.
    */
    class EventFunctions
    {
    public:
        /** Constructs EventFunctions with default values.

        */
        EventFunctions() = default;

        /** Destructor for EventFunctions.

        */
        virtual ~EventFunctions() = default;

        /** Function for handling when a key is pressed or released.

            @param type     Key event (press or release)
            @param KeyCode  Keycode
        */
        virtual void onKeyEvent(const KeyEvent&) {}

        /** Function for handling key input.

            @param data         array of display characters
            @param size         amount of characters
            @param encoding     encoding of the characer data
        */
        virtual void onKeyInput(const InputEvent&) {}

        /** Function for handling when mouse is hovering.

        */
        virtual void onHoverEvent(const MotionEvent&) {}

        /** Function for handling when mouse is entering.

        */
        virtual void onEnterEvent(const EnterEvent&) {}

        /** Function for handling when mouse is leaving.

        */
        virtual void onLeaveEvent(const LeaveEvent&) {}

        // TODO: Add documentation.
        virtual void onLeaveClickEvent(const LeaveClickEvent&) {}

        /** Function for handling when mouse is scrolling.

         @param offset     x and y offset
        */
        virtual void onScrollEvent(const ScrollEvent&) {}

        /** Function for handling when mouse is clicking.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        virtual void onClickEvent(const ClickEvent&) {}

        /** Function for handling when mouse is released.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        virtual void onReleaseEvent(const ReleaseEvent&) {}
    };
}

#endif // PTK_CORE_EVENTFUNCTIONS_HPP
