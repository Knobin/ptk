//
//  events/mouseevent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_MOUSEEVENT_HPP
#define PTK_EVENTS_MOUSEEVENT_HPP

// Local Headers
#include "ptk/events/event.hpp"

namespace pTK
{
    enum class MouseButton
    {
        NONE = 0,
        Left,
        Middle,
        Right
    };

    class MotionEvent : public Event
    {
    public:
        MotionEvent(int t_posx, int t_posy)
            : Event(EventCategory::Mouse, EventType::MouseMoved), m_posx{t_posx}, m_posy{t_posy}
        {
        }
        ~MotionEvent() final = default;

        int get_posx() const { return m_posx; }
        int get_posy() const { return m_posy; }
    private:
        int m_posx;
        int m_posy;
    };

    class ScrollEvent : public Event
    {
    public:
        ScrollEvent(int t_xoffset, int t_yoffset)
            : Event(EventCategory::Mouse, EventType::MouseScrolled), m_xoffset{t_xoffset}, m_yoffset{t_yoffset}
        {
        }
        ~ScrollEvent() final = default;

        int get_x_offset() const { return m_xoffset; }
        int get_y_offset() const { return m_yoffset; }
    private:
        int m_xoffset;
        int m_yoffset;
    };

    class ButtonEvent : public Event
    {
    public:
        ButtonEvent(EventType type, MouseButton t_button)
            : Event(EventCategory::Mouse, type), m_button{t_button}
        {
        }
        ~ButtonEvent() final = default;

        MouseButton get_button() const { return m_button; }
    private:
        MouseButton m_button;
    };
}

#endif // PTK_EVENTS_MOUSEEVENT_HPP