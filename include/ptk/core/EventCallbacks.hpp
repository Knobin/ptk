//
//  events/EventCallbacks.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENTCALLBACKS_HPP
#define PTK_CORE_EVENTCALLBACKS_HPP

// pTK Headers
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WidgetEvents.hpp"
#include "ptk/core/EventManager.hpp"

// C++ Headers
#include <functional>
#include <vector>

namespace pTK
{
    class Widget;

    class EventCallbacks
    {
    public:
        /** Constructs EventCallbacks with default values.

        */
        EventCallbacks() = default;

        /** Destructor for EventCallbacks.

        */
        virtual ~EventCallbacks() = default;

        /** Function for handling when a key is pressed or released.

            @param callback    function to call on key event
        */
        void onKey(const std::function<bool(const KeyEvent&)>& callback);

        /** Function for handling when key input

            @param callback    function to call on key input
        */
        void onInput(const std::function<bool(const InputEvent&)>& callback);

        /** Function for handling when mouse is hovering.

            @param callback    function to call on hover event
        */
        void onHover(const std::function<bool(const MotionEvent&)>& callback);

        /** Function for handling when mouse is entering.

            @param callback    function to call on hover event
        */
        void onEnter(const std::function<bool(const EnterEvent&)>& callback);

        /** Function for handling when mouse is leaving.

            @param callback    function to call on leaving event
        */
        void onLeave(const std::function<bool(const LeaveEvent&)>& callback);

        // TODO: Add documentation.
        void onLeaveClick(const std::function<bool(const LeaveClickEvent&)>& callback);

        /** Function for handling when mouse is scrolling.

            @param offset     x and y offset
        */
        void onScroll(const std::function<bool(const ScrollEvent&)>& callback);

        /** Function for handling when mouse is clicking.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onClick(const std::function<bool(const ClickEvent&)>& callback);

        /** Function for handling when mouse is released.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onRelease(const std::function<bool(const ReleaseEvent&)>& callback);

        /** Function to add a listener.

            @param callback     function to call on event
        */
        template <typename T>
        uint64 addListener(std::function<bool(const T&)> callback)
        {
            PTK_ASSERT(getWidgetPtr(), "Widget ptr is nullptr");
            uint64 id = EventManager::AddListener<T>(getWidgetPtr(), std::move(callback));
            return id;
        }

        /** Function to trigger an event.

            @param event    triggered event
        */
        template <typename T>
        void triggerEvent(const T& event)
        {
            PTK_ASSERT(getWidgetPtr(), "Widget ptr is nullptr");
            EventManager::TriggerWidgetEvent<T>(getWidgetPtr(), event);
        }

    private:
        virtual Widget *getWidgetPtr() { return nullptr; }
    };
}

#endif // PTK_CORE_EVENTCALLBACKS_HPP
