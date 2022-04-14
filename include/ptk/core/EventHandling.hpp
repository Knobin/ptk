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

        This class is to handle the incoming events.

        Depending on the event category and type, one of
        the defined functions below will be called.
    */
    class EventHandling : public EventCallbacks, public EventFunctions
    {
    public:
        /** Constructs EventHandling with default values.

        */
        EventHandling() = default;

        /** Destructor for EventHandling.

        */
        virtual ~EventHandling() = default;

        template<typename Event>
        void handleEvent(const Event& evt);

    private:
        // void registerMemberCallbacks();
    };

    template<typename Event>
    void EventHandling::handleEvent(const Event& evt)
    {
        triggerEvent<Event>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const KeyEvent& evt)
    {
        onKeyEvent(evt);
        triggerEvent<KeyEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const InputEvent& evt)
    {
        onKeyInput(evt);
        triggerEvent<InputEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const MotionEvent& evt)
    {
        onHoverEvent(evt);
        triggerEvent<MotionEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const EnterEvent& evt)
    {
        onEnterEvent(evt);
        triggerEvent<EnterEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const LeaveEvent& evt)
    {
        onLeaveEvent(evt);
        triggerEvent<LeaveEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const LeaveClickEvent& evt)
    {
        onLeaveClickEvent(evt);
        triggerEvent<LeaveClickEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const ScrollEvent& evt)
    {
        onScrollEvent(evt);
        triggerEvent<ScrollEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const ClickEvent& evt)
    {
        onClickEvent(evt);
        triggerEvent<ClickEvent>(evt);
    }

    template<>
    inline void EventHandling::handleEvent(const ReleaseEvent& evt)
    {
        onReleaseEvent(evt);
        triggerEvent<ReleaseEvent>(evt);
    }
}

#endif // PTK_CORE_EVENTHANDLING_HPP
