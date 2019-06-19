//
//  events/event.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_EVENT_HPP
#define PTK_EVENTS_EVENT_HPP

namespace pTK
{
    enum class EventType
    {
        NONE = 0,
        WindowClose, WindowLostFocus, WindowFocus, WindowDraw, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum class EventCategory
    {
        NONE = 0,
        Window,
        Key,
        Mouse
    };

    class Event
    {
    public:
        Event(EventCategory t_category, EventType t_type)
            : m_type{t_type}, m_category{t_category}
        {
        }
        virtual ~Event() = default;

        EventType get_type() const { return m_type; }
        EventCategory get_category() const { return m_category; }

    protected:
        EventType m_type;
        EventCategory m_category;
    };
}

#endif // PTK_EVENTS_EVENT_HPP