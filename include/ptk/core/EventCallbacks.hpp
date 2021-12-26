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

// C++ Headers
#include <functional>
#include <map>
#include <vector>

namespace pTK
{
    template <typename T>
    struct CallbackContainer
    {
        ~CallbackContainer()
        {
#ifdef PTK_DEBUG
            for (auto it{callbacks.cbegin()}; it != callbacks.cend(); ++it)
            {
                PTK_INFO("Callback {} removed (destruction)", it->first);
            }
#endif
        }

        void addCallback(uint64 id, const std::function<bool(const T&)>& callback)
        {
            callbacks.insert({id, callback});
            PTK_INFO("CallbackContainer: added [WIDGET] callback with id: {}", id);
        }

        void removeCallback(uint64 id)
        {
            auto found = callbacks.find(id);
            if (found != callbacks.end())
            {
                PTK_INFO("CallbackContainer: removed [WIDGET] callback with id: {}", id);
                callbacks.erase(found);
            }
        }

        void triggerCallbacks(const T& evt)
        {
            for (auto it = callbacks.begin(); it != callbacks.end();)
            {
                if (it->second(evt))
                {
                    PTK_INFO("CallbackContainer: auto-removed [WIDGET] callback with id: {}", it->first);
                    callbacks.erase(it++);
                }
                else
                    ++it;
            }
        }

        std::map<uint64, std::function<bool(const T&)>> callbacks;
    };



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
        uint64 addListener(const std::function<bool(const T&)>& callback)
        {
            CallbackContainer<T> *storage = getListeners<T>();
            PTK_ASSERT(storage, "getListeners<T>() not implementated");

            if (storage)
            {
                uint64 id = s_id++;
                storage->addCallback(id, callback);
                return id;
            }

            return 0;
        }

        /** Function to trigger an event.

            @param event    triggered event
        */
        template <typename T>
        void triggerEvent(const T& event)
        {
            CallbackContainer<T> *storage = getListeners<T>();
            PTK_ASSERT(storage, "getListeners<T>() not implementated");

            if (storage)
                storage->triggerCallbacks(event);
        }

    private:

        template <typename T>
        CallbackContainer<T> *getListeners()
        {
            return nullptr;
        }

    private:
        CallbackContainer<KeyEvent> m_keyEventStorage{};
        CallbackContainer<InputEvent> m_inputEventStorage{};
        CallbackContainer<MotionEvent> m_hoverEventStorage{};
        CallbackContainer<EnterEvent> m_enterEventStorage{};
        CallbackContainer<LeaveEvent> m_leaveEventStorage{};
        CallbackContainer<LeaveClickEvent> m_leaveClickEventStorage{};
        CallbackContainer<ScrollEvent> m_scrollEventStorage{};
        CallbackContainer<ClickEvent> m_clickEventStorage{};
        CallbackContainer<ReleaseEvent> m_releaseEventStorage{};

        static uint64 s_id;
    };

    template<>
    inline CallbackContainer<KeyEvent> *EventCallbacks::getListeners<KeyEvent>() {
        return &m_keyEventStorage;
    }

    template<>
    inline CallbackContainer<InputEvent> *EventCallbacks::getListeners<InputEvent>() {
        return &m_inputEventStorage;
    }

    template<>
    inline CallbackContainer<MotionEvent> *EventCallbacks::getListeners<MotionEvent>() {
        return &m_hoverEventStorage;
    }

    template<>
    inline CallbackContainer<EnterEvent> *EventCallbacks::getListeners<EnterEvent>() {
        return &m_enterEventStorage;
    }

    template<>
    inline CallbackContainer<LeaveEvent> *EventCallbacks::getListeners<LeaveEvent>() {
        return &m_leaveEventStorage;
    }

    template<>
    inline CallbackContainer<LeaveClickEvent> *EventCallbacks::getListeners<LeaveClickEvent>() {
        return &m_leaveClickEventStorage;
    }

    template<>
    inline CallbackContainer<ScrollEvent> *EventCallbacks::getListeners<ScrollEvent>() {
        return &m_scrollEventStorage;
    }

    template<>
    inline CallbackContainer<ClickEvent> *EventCallbacks::getListeners<ClickEvent>() {
        return &m_clickEventStorage;
    }

    template<>
    inline CallbackContainer<ReleaseEvent> *EventCallbacks::getListeners<ReleaseEvent>() {
        return &m_releaseEventStorage;
    }
}

#endif // PTK_CORE_EVENTCALLBACKS_HPP
