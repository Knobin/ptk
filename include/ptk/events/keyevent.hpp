//
//  events/keyevent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_KEYEVENT_HPP
#define PTK_EVENTS_KEYEVENT_HPP

// Local Headers
#include "ptk/events/event.hpp"

namespace pTK
{
    class KeyEvent : public Event
    {
    public:
        KeyEvent(EventType t_type, int t_keycode)
            : Event(EventCategory::Key, t_type), m_keycode{t_keycode}
        {
        }
        ~KeyEvent() final = default;

        int get_keycode() const { return m_keycode; }

    private:
        int m_keycode;
    };
}

#endif // PTK_EVENTS_KEYEVENT_HPP