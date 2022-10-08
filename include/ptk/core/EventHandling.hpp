//
//  core/EventHandling.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_CORE_EVENTHANDLING_HPP
#define PTK_CORE_EVENTHANDLING_HPP

// pTK Headers
#include "ptk/core/EventCallbacks.hpp"
#include "ptk/core/EventFunctions.hpp"
#include "ptk/events/KeyCodes.hpp"

namespace pTK
{
    /** EventHandling class implementation.

        This class is to handle all the incoming events.

        To send an event use the template function handleEvent.

        Depending if a template specialization exists for the event, it will
        also call the corresponing event function in EventFunctions.
    */
    class PTK_API EventHandling : public EventCallbacks, public EventFunctions
    {
    public:
        /** Constructs EventHandling with default values.

        */
        EventHandling() = default;

        /** Move Constructor for EventHandling.

            @return    initialized EventHandling from value
        */
        EventHandling(EventHandling&&) = default;

        /** Move Assignment operator for EventHandling.

            @return    EventHandling with value
        */
        EventHandling& operator=(EventHandling&&) = default;

        /** Destructor for EventHandling.

        */
        virtual ~EventHandling() = default;

        /** Templated function for handling all events.

            @param evt    event of type Event
        */
        template<typename Event>
        void handleEvent(const Event& evt);
    };

    /** Standard version of the function

        @param evt    event of type Event
    */
    template<typename Event>
    void EventHandling::handleEvent(const Event& evt)
    {
        triggerEvent<Event>(evt);
    }

    /** handleEvent specialization for KeyEvent.

        @param evt    KeyEvent
    */
    template<>
    inline void EventHandling::handleEvent(const KeyEvent& evt)
    {
        onKeyEvent(evt);
        triggerEvent<KeyEvent>(evt);
    }

    /** handleEvent specialization for InputEvent.

        @param evt    InputEvent
    */
    template<>
    inline void EventHandling::handleEvent(const InputEvent& evt)
    {
        onKeyInput(evt);
        triggerEvent<InputEvent>(evt);
    }

    /** handleEvent specialization for MotionEvent.

        @param evt    MotionEvent
    */
    template<>
    inline void EventHandling::handleEvent(const MotionEvent& evt)
    {
        onHoverEvent(evt);
        triggerEvent<MotionEvent>(evt);
    }

    /** handleEvent specialization for EnterEvent.

        @param evt    EnterEvent
    */
    template<>
    inline void EventHandling::handleEvent(const EnterEvent& evt)
    {
        onEnterEvent(evt);
        triggerEvent<EnterEvent>(evt);
    }

    /** handleEvent specialization for LeaveEvent.

        @param evt    LeaveEvent
    */
    template<>
    inline void EventHandling::handleEvent(const LeaveEvent& evt)
    {
        onLeaveEvent(evt);
        triggerEvent<LeaveEvent>(evt);
    }

    /** handleEvent specialization for LeaveClickEvent.

        @param evt    LeaveClickEvent
    */
    template<>
    inline void EventHandling::handleEvent(const LeaveClickEvent& evt)
    {
        onLeaveClickEvent(evt);
        triggerEvent<LeaveClickEvent>(evt);
    }

    /** handleEvent specialization for ScrollEvent.

        @param evt    ScrollEvent
    */
    template<>
    inline void EventHandling::handleEvent(const ScrollEvent& evt)
    {
        onScrollEvent(evt);
        triggerEvent<ScrollEvent>(evt);
    }

    /** handleEvent specialization for ClickEvent.

        @param evt    ClickEvent
    */
    template<>
    inline void EventHandling::handleEvent(const ClickEvent& evt)
    {
        onClickEvent(evt);
        triggerEvent<ClickEvent>(evt);
    }

    /** handleEvent specialization for ReleaseEvent.

        @param evt    ReleaseEvent
    */
    template<>
    inline void EventHandling::handleEvent(const ReleaseEvent& evt)
    {
        onReleaseEvent(evt);
        triggerEvent<ReleaseEvent>(evt);
    }
}

#endif // PTK_CORE_EVENTHANDLING_HPP
