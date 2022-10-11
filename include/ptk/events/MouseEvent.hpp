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
    } // namespace Mouse

    /** MotionEvent class implementation.

        Signal a mouse movement.
    */
    class PTK_API MotionEvent : public Event
    {
    public:
        /** Constructs MotionEvent with default values with pos.

            @param t_pos    associated x and y position
            @return         default initialized MotionEvent
        */
        explicit constexpr MotionEvent(const Point& t_pos) noexcept
            : Event(Event::Category::Mouse, Event::Type::MouseMoved),
              pos{t_pos}
        {}

        // Location.
        Point pos;
    };

    /** ScrollEvent class implementation.

        Signal a mouse scroll.
    */
    class PTK_API ScrollEvent : public Event
    {
    public:
        /** Constructs ScrollEvent with default values with offset.

            @param t_offset     associated x and y offset
            @return             default initialized ScrollEvent
        */
        explicit constexpr ScrollEvent(const Vec2f& t_offset) noexcept
            : Event(Event::Category::Mouse, Event::Type::MouseScrolled),
              offset{t_offset}
        {}

        // Scroll offset.
        Vec2f offset;
    };

    /** ButtonEvent class implementation.

        Signal a mouse press or release event.
    */
    class PTK_API ButtonEvent : public Event
    {
    public:
        /** Constructs ButtonEvent with default values with type,
            type, t_button and t_pos.

            @param type         press or release
            @param t_button     which button
            @param t_pos        associated x and y
            @return             default initialized ButtonEvent
        */
        constexpr ButtonEvent(Event::Type t_type, Mouse::Button t_button, const Point& t_pos) noexcept
            : Event(Event::Category::Mouse, t_type),
              button{t_button},
              pos{t_pos}
        {}

        // Button pressed or released.
        Mouse::Button button;

        // Location.
        Point pos;
    };

    class PTK_API ClickEvent : public ButtonEvent
    {
    public:
        /** Constructs ClickEvent.

            @param t_button     which button
            @param t_pos        associated x and y
            @return             default initialized ButtonEvent
        */
        constexpr ClickEvent(Mouse::Button t_button, const Point& t_pos) noexcept
            : ButtonEvent(Event::Type::MouseButtonPressed, t_button, t_pos)
        {}
    };

    class PTK_API ReleaseEvent : public ButtonEvent
    {
    public:
        /** Constructs ClickEvent.

            @param t_button     which button
            @param t_pos        associated x and y
            @return             default initialized ButtonEvent
        */
        constexpr ReleaseEvent(Mouse::Button t_button, const Point& t_pos) noexcept
            : ButtonEvent(Event::Type::MouseButtonReleased, t_button, t_pos)
        {}
    };
} // namespace pTK

#endif // PTK_EVENTS_MOUSEEVENT_HPP
