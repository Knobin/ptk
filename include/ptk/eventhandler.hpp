//
//  eventshandler.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_EVENTHANDLER_HPP
#define PTK_EVENTHANDLER_HPP

// Local Headers
#include "ptk/events/event.hpp"

// C++ Headers
#include <queue>

namespace pTK
{
    class EventHandler
    {
    public:
        EventHandler() = default;
        virtual ~EventHandler()
        {
            while (!m_event_queue.empty())
            {
                Event* event = m_event_queue.front();
                m_event_queue.pop();
                delete event;
            }
        }

        virtual void push_event(Event* t_event) { m_event_queue.push(t_event); }
        virtual Event* pop_event()
        {
            Event* event = m_event_queue.front();
            m_event_queue.pop();
            return event;
        }

        // Event processing
        virtual void key_event(Event*) {}
        virtual void mouse_event(Event*) {}
        virtual void window_event(Event*) {}

    protected:
        std::queue<Event*> m_event_queue;
    };
}

#endif // PTK_EVENTHANDLER_HPP