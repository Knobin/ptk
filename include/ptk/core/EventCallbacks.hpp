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
#include "ptk/core/Defines.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WidgetEvents.hpp"

namespace pTK
{
    /** EventCallbackInterface class implementation.

        IMPORTANT: Be careful when implementing move semantics!
        Since this class is intended to be implemented with virtual inheritance,
        the move operation may be called multiple times and result in a loss of
        all the data. Therefore, make sure to only move this class ONCE, or let
        a "super" or main class inherit the EventCallbackMainInterface and have
        that class provide the necessary move operation. This will also mean that
        all classes that inherits the EventCallbackInterface class will have to
        implement the move constructor and assignment operator if move semantics
        is desired (defaulting those will result in a "defaulted move assignment"
        warning from the compiler, or GCC at least).

        This class contains functions that are used to add callbacks events
        that are triggered / handled.

        Main purpose is to store callbacks for Widgets.
        And therefore has been limited in scope to only use functions
        pointers of type bool() or bool(const T&) since the return value
        serves a purpose for deletion.

        The addListener function can be used to add a callback to any type.
        When a callback has been added a unique identifier will be returned.

        If you want to remove that callback in the future you need to save
        that identifier and use it when calling removeListener.

        Callbacks can also be removed by returning true when the function is
        called (hence the bool return value). If you do not want to remove the
        function after it has been called, simply return false in the callback.
    */
    class PTK_API EventCallbackInterface
    {
    public:
        /** Constructs EventCallbackInterface with default values.

            @return    initialized EventCallbackInterface
        */
        EventCallbackInterface() = default;

        /** Deleted Copy Constructor.

            Copying of callbacks can return undesired results, especially implicit copying.
            To get a deep copy of CallbackStorage, use the getter and then use the clone() function.
        */
        EventCallbackInterface(const EventCallbackInterface&) = delete;

        /** Deleted Copy Assignment operator.

            Copying of callbacks can return undesired results, especially implicit copying.
            To get a deep copy of CallbackStorage, use the getter and then use the clone() function.
        */
        EventCallbackInterface& operator=(const EventCallbackInterface&) = delete;

        /** Destructor for EventCallbackInterface.

        */
        virtual ~EventCallbackInterface() = default;

        /** Function to add callback.

            @param callback     function to call on event
            @return             callback id
        */
        template <typename T>
        uint64_t addListener(const std::function<bool(const T&)>& callback)
        {
            return m_callbackStorage.addCallback<T, bool(const T&)>(callback);
        }

        /** Function to add callback.

            @param callback     function to call on event
            @return             callback id
        */
        template <typename T>
        uint64_t addListener(const std::function<bool()>& callback)
        {
            auto helper_func = [callback](const T&) { return callback(); };
            return m_callbackStorage.addCallback<T, bool(const T&)>(helper_func);
        }

        /** Function to remove callback.

            @param id   callback id to remove
        */
        template <typename T>
        bool removeListener(uint64_t id)
        {
            return m_callbackStorage.removeCallback<T, bool(const T&)>(id);
        }

        /** Function to trigger an event.

            @param event    triggered event
        */
        template <typename T, typename... Args>
        void triggerEvent(Args&&... args)
        {
            const auto predicate = [&](typename CallbackContainer<bool(const T&)>::Node& entry) {
                return entry.callback(std::forward<Args>(args)...);
            };
            m_callbackStorage.removeCallbackIf<T, bool(const T&)>(predicate);
        }

        /** Function for retrieving the CallbackStorage.

            @return     callback storage
        */
        [[nodiscard]] const CallbackStorage& callbackStorage() const noexcept { return m_callbackStorage; }

        /** Function for retrieving the CallbackStorage.

            @return     callback storage
        */
        [[nodiscard]] CallbackStorage& callbackStorage() noexcept { return m_callbackStorage; }

    protected:
        /** Move Constructor for EventCallbackInterface.

            @return    initialized EventCallbackInterface from value
        */
        EventCallbackInterface(EventCallbackInterface&&) = default;

        /** Move Assignment operator for EventCallbackInterface.

            @return    EventCallbackInterface with value
        */
        EventCallbackInterface& operator=(EventCallbackInterface&&) = default;

    private:
        CallbackStorage m_callbackStorage{};
    };

    /** EventCallbackMainInterface class implementation.

        Implements the proper move semantics for EventCallbackInterface.

        Refer to the explanation in EventCallbackInterface for the usage of this
        class and why it exists.
    */
    class PTK_API EventCallbackMainInterface : public virtual EventCallbackInterface
    {
    public:
        /** Constructs EventCallbackMainInterface with default values.

            @return    initialized EventCallbackMainInterface
        */
        EventCallbackMainInterface() = default;

        /** Destructor for EventCallbackMainInterface.

        */
        virtual ~EventCallbackMainInterface() = default;

        /** Move Constructor for EventCallbackMainInterface.

            @return    initialized EventCallbackMainInterface from value
        */
        EventCallbackMainInterface(EventCallbackMainInterface&& other) noexcept
        {
            callbackStorage() = std::move(other.callbackStorage());
        }

        /** Move Assignment operator for EventCallbackMainInterface.

            @return    EventCallbackMainInterface with value
        */
        EventCallbackMainInterface& operator=(EventCallbackMainInterface&& other) noexcept
        {
            callbackStorage() = std::move(other.callbackStorage());
            return *this;
        }
    };

    /** EventCallbacks class implementation.

        This class contains helper functions for adding callbacks into
        storage for common events.
    */
    class PTK_API EventCallbacks : public EventCallbackMainInterface
    {
    public:
        /** Constructs EventCallbacks with default values.

        */
        EventCallbacks() = default;

        EventCallbacks(EventCallbacks&&) = default;
        EventCallbacks& operator=(EventCallbacks&&) = default;

        /** Destructor for EventCallbacks.

        */
        virtual ~EventCallbacks() = default;

        /** Function for handling when a key is pressed or released.

            @param callback    function to call on key event
            @return            callback id
        */
        uint64_t onKey(const std::function<bool(const KeyEvent&)>& callback);

        /** Function for handling when key input

            @param callback    function to call on key input
            @return            callback id
        */
        uint64_t onInput(const std::function<bool(const InputEvent&)>& callback);

        /** Function for handling when mouse is hovering.

            @param callback    function to call on hover event
            @return            callback id
        */
        uint64_t onHover(const std::function<bool(const MotionEvent&)>& callback);

        /** Function for handling when mouse is entering.

            @param callback    function to call on hover event
            @return            callback id
        */
        uint64_t onEnter(const std::function<bool(const EnterEvent&)>& callback);

        /** Function for handling when mouse is leaving.

            @param callback    function to call on leaving event
            @return            callback id
        */
        uint64_t onLeave(const std::function<bool(const LeaveEvent&)>& callback);

        /** Function for handling when mouse has left and a previous click has happened.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onLeaveClick(const std::function<bool(const LeaveClickEvent&)>& callback);

        /** Function for handling when mouse is scrolling.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onScroll(const std::function<bool(const ScrollEvent&)>& callback);

        /** Function for handling when mouse is clicking.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onClick(const std::function<bool(const ClickEvent&)>& callback);

        /** Function for handling when mouse is released.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onRelease(const std::function<bool(const ReleaseEvent&)>& callback);
    };
} // namespace pTK

#endif // PTK_CORE_EVENTCALLBACKS_HPP
