//
//  events/WindowEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_WINDOWEVENT_HPP
#define PTK_EVENTS_WINDOWEVENT_HPP

// pTK Headers
#include "ptk/core/Event.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/Size.hpp"

namespace pTK
{
    /** ResizeEvent class implementation.

        Signal a window resize.
    */
    class PTK_API ResizeEvent final : public Event
    {
    public:
        /** Constructs ResizeEvent with default values with size.

            @param t_size   associated width and height
            @return         default initialized ResizeEvent
        */
        explicit constexpr ResizeEvent(const Size& t_size) noexcept
            : Event(Event::Category::Window, Event::Type::WindowResize),
              size{t_size}
        {}

        // Resize values.
        Size size;
    };

    /** MoveEvent class implementation.

        Signal a window movement.
    */
    class PTK_API MoveEvent final : public Event
    {
    public:
        /** Constructs MoveEvent with default values with size.

            @param t_pos    associated x and y
            @return         default initialized MoveEvent
        */
        explicit constexpr MoveEvent(const Point& t_pos) noexcept
            : Event(Event::Category::Window, Event::Type::WindowMove),
              pos{t_pos}
        {}

        // Movement coordinates.
        Point pos;
    };

    /** ScaleEvent class implementation.

        Signal a window scale change.
    */
    class PTK_API ScaleEvent final : public Event
    {
    public:
        /** ScaleEvent with default values with scale.

            @param t_scale  associated x and y scale
            @return         default initialized ScaleEvent
        */
        explicit constexpr ScaleEvent(const Vec2f& t_scale) noexcept
            : Event(Event::Category::Window, Event::Type::WindowScale),
              scale{t_scale}
        {}

        // Movement coordinates.
        Vec2f scale;
    };

    /** PaintEvent class implementation.

        Signal to paint to the window.
    */
    class PTK_API PaintEvent final : public Event
    {
    public:
        /** PaintEvent with default values with pos and size.

            @param t_pos    coordinates
            @param t_size   area
            @return         default initialized PaintEvent
        */
        constexpr PaintEvent(const Point& t_pos, const Size& t_size) noexcept
            : Event(Event::Category::Window, Event::Type::WindowPaint),
              pos{t_pos},
              size{t_size}
        {}

        // Coordinates.
        Point pos;

        // Area.
        Size size;
    };

    /** CloseEvent class implementation.

        Signal to that window should close.
    */
    class PTK_API CloseEvent final : public Event
    {
    public:
        constexpr CloseEvent() noexcept
            : Event(Event::Category::Window, Event::Type::WindowClose)
        {}
    };

    /** MinimizeEvent class implementation.

        Signal to that window has been minimized.
    */
    class PTK_API MinimizeEvent final : public Event
    {
    public:
        constexpr MinimizeEvent() noexcept
            : Event(Event::Category::Window, Event::Type::WindowMinimize)
        {}
    };

    /** RestoreEvent class implementation.

        Signal to that window has been restored
        from a minimized state.
    */
    class PTK_API RestoreEvent final : public Event
    {
    public:
        constexpr RestoreEvent() noexcept
            : Event(Event::Category::Window, Event::Type::WindowRestore)
        {}
    };

    /** FocusEvent class implementation.

        Signal to that window is in focus.
    */
    class PTK_API FocusEvent final : public Event
    {
    public:
        constexpr FocusEvent() noexcept
            : Event(Event::Category::Window, Event::Type::WindowFocus)
        {}
    };

    /** LostFocusEvent class implementation.

        Signal to that window is out of focus.
    */
    class PTK_API LostFocusEvent final : public Event
    {
    public:
        constexpr LostFocusEvent() noexcept
            : Event(Event::Category::Window, Event::Type::WindowLostFocus)
        {}
    };
} // namespace pTK

#endif // PTK_EVENTS_WINDOWEVENT_HPP
