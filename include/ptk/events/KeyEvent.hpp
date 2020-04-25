//
//  events/KeyEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_KEYEVENT_HPP
#define PTK_EVENTS_KEYEVENT_HPP

// Local Headers
#include "ptk/core/Event.hpp"
#include "ptk/core/Types.hpp"

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
        // Ob jects for easier type management.
        inline static Event::Type Pressed = Event::Type::KeyPressed;
        inline static Event::Type Released = Event::Type::KeyReleased;

    public:
        /** Constructs KeyEvent with default values with type and code.
         
            @param type     press or release
            @param code     associated keycode
            @return         default initialized KeyEvent
        */
        KeyEvent(Event::Type type, int32 code)
            : Event(Event::Category::Key, type), keycode{code}
        {

        }

        // Key code.
        const int32 keycode;
    };
}

#endif // PTK_EVENTS_KEYEVENT_HPP
