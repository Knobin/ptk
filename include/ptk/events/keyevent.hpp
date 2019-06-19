//
//  events/keyevent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_KEYEVENT_HPP
#define PTK_EVENTS_KEYEVENT_HPP

// Local Headers
#include "ptk/core/event.hpp"

namespace pTK
{
    /** KeyEvent class implementation.
     
     Derived from Event, this class if for creating
     events from the keyboard. Such as Key presses or
     releases.
     */
    class KeyEvent : public Event
    {
    public:
        /** Constructs KeyEvent with default values with t_type and t_keycode.
         
         @return    default initialized KeyEvent
         */
        KeyEvent(EventType t_type, int t_keycode)
            : Event(EventCategory::Key, t_type), m_keycode{t_keycode}
        {
        }
        ~KeyEvent() final = default;

        /** Function for retrieving the associated keycode.
         
         @return    Pressed or Released keycode
         */
        int get_keycode() const { return m_keycode; }

    private:
        int m_keycode;
    };
}

#endif // PTK_EVENTS_KEYEVENT_HPP
