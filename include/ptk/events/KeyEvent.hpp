//
//  events/KeyEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_KEYEVENT_HPP
#define PTK_EVENTS_KEYEVENT_HPP

// pTK Headers
#include "ptk/core/Event.hpp"
#include "ptk/core/Types.hpp"
#include "ptk/events/KeyCodes.hpp"

namespace pTK
{
    /** KeyEvent class implementation.
     
        Derived from Event, this class if for creating
        events from the keyboard. Such as Key presses or
        releases.
    */
    class KeyEvent final : public Event
    {
    public:
        // Objects for easier type management.
        inline static const Event::Type Pressed = Event::Type::KeyPressed;
        inline static const Event::Type Released = Event::Type::KeyReleased;

    public:
        /** Constructs KeyEvent with default values with type and code.
         
            @param type     press or release
            @param code     associated keycode
            @return         default initialized KeyEvent
        */
        KeyEvent(Event::Type type, KeyCode code)
            : Event(Event::Category::Key, type), keycode{code}
        {

        }

        // Key code.
        const KeyCode keycode;
    };
}

#endif // PTK_EVENTS_KEYEVENT_HPP
