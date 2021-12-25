//
//  events/MouseEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_MOUSEEVENT_HPP
#define PTK_EVENTS_MOUSEEVENT_HPP

// pTK Headers
#include "ptk/core/Event.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/Vec2.hpp"

namespace pTK
{
    namespace Mouse
    {
        /** Mouse::Button enum class implementation.

            This enum class is to specify which button on the mouse
            if pressed or released.
        */
        enum class Button
        {
            NONE = 0,
            Left,
            Middle,
            Right
        };
    }

    /** MotionEvent class implementation.

        Signal a mouse movement.
    */
    class MotionEvent : public Event
    {
    public:
        /** Constructs MotionEvent with default values with pos.

            @param t_pos    associated x and y position
            @return         default initialized MotionEvent
        */
        MotionEvent(const Point& t_pos)
            : Event(Event::Category::Mouse, Event::Type::MouseMoved),
                pos{t_pos}
        {
        }

        // Location.
        const Point pos;
    };

    /** ScrollEvent class implementation.

        Signal a mouse scroll.
    */
    class ScrollEvent : public Event
    {
    public:
        /** Constructs ScrollEvent with default values with offset.

            @param t_offset     associated x and y offset
            @return             default initialized ScrollEvent
        */
        ScrollEvent(const Vec2f& t_offset)
            : Event(Event::Category::Mouse, Event::Type::MouseScrolled),
                offset{t_offset}
        {
        }

        // Scroll offset.
        const Vec2f offset;
    };

    /** ButtonEvent class implementation.

        Signal a mouse press or release event.
    */
    class ButtonEvent : public Event
    {
    public:
        /** Constructs ButtonEvent with default values with type,
            type, t_button and t_pos.

            @param type         press or release
            @param t_button     which button
            @param t_pos        associated x and y
            @return             default initialized ButtonEvent
        */
        ButtonEvent(Event::Type type, Mouse::Button t_button, const Point& t_pos)
            : Event(Event::Category::Mouse, type),
                button{t_button}, pos{t_pos}
        {

        }

        // Button pressed or released.
        const Mouse::Button button;

        // Location.
        const Point pos;
    };

    class ClickEvent : public ButtonEvent
    {
    public:
        /** Constructs ClickEvent.

            @param t_button     which button
            @param t_pos        associated x and y
            @return             default initialized ButtonEvent
        */
        ClickEvent(Mouse::Button t_button, const Point& t_pos)
            : ButtonEvent(Event::Type::MouseButtonPressed, t_button, t_pos)
        {

        }
    };


    class ReleaseEvent : public ButtonEvent
    {
    public:
        /** Constructs ClickEvent.

            @param t_button     which button
            @param t_pos        associated x and y
            @return             default initialized ButtonEvent
        */
        ReleaseEvent(Mouse::Button t_button, const Point& t_pos)
            : ButtonEvent(Event::Type::MouseButtonReleased, t_button, t_pos)
        {

        }
    };
}

#endif // PTK_EVENTS_MOUSEEVENT_HPP
