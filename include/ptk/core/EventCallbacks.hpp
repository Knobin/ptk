//
//  events/EventCallbacks.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENTCALLBACKS_HPP
#define PTK_CORE_EVENTCALLBACKS_HPP

// pTK Headers
#include "ptk/core/CallbackStorage.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WidgetEvents.hpp"

namespace pTK
{
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
            @return            callback id
        */
        uint64 onKey(const std::function<bool(const KeyEvent&)>& callback);

        /** Function for handling when key input

            @param callback    function to call on key input
            @return            callback id
        */
        uint64 onInput(const std::function<bool(const InputEvent&)>& callback);

        /** Function for handling when mouse is hovering.

            @param callback    function to call on hover event
            @return            callback id
        */
        uint64 onHover(const std::function<bool(const MotionEvent&)>& callback);

        /** Function for handling when mouse is entering.

            @param callback    function to call on hover event
            @return            callback id
        */
        uint64 onEnter(const std::function<bool(const EnterEvent&)>& callback);

        /** Function for handling when mouse is leaving.

            @param callback    function to call on leaving event
            @return            callback id
        */
        uint64 onLeave(const std::function<bool(const LeaveEvent&)>& callback);

        // TODO: Add documentation.
        uint64 onLeaveClick(const std::function<bool(const LeaveClickEvent&)>& callback);

        /** Function for handling when mouse is scrolling.

            @param offset     x and y offset
            @return            callback id
        */
        uint64 onScroll(const std::function<bool(const ScrollEvent&)>& callback);

        /** Function for handling when mouse is clicking.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
            @return            callback id
        */
        uint64 onClick(const std::function<bool(const ClickEvent&)>& callback);

        /** Function for handling when mouse is released.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
            @return            callback id
        */
        uint64 onRelease(const std::function<bool(const ReleaseEvent&)>& callback);

        /** Function to add callback.

            @param callback     function to call on event
            @return             callback id
        */
        template <typename T>
        uint64 addListener(const std::function<bool(const T&)>& callback)
        {
            return m_callbackStorage.addCallback<T>(callback);
        }

        /** Function to remove callback.

            @param id   callback id to remove
        */
        template <typename T>
        void removeListener(uint64 id)
        {
            return m_callbackStorage.removeCallback<T>(id);
        }

        /** Function to trigger an event.

            @param event    triggered event
        */
        template <typename T>
        void triggerEvent(const T& event)
        {
            return m_callbackStorage.triggerCallbacks<T>(event);
        }

    private:
        CallbackStorage m_callbackStorage;
    };
}

#endif // PTK_CORE_EVENTCALLBACKS_HPP
