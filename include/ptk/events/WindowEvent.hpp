//
//  events/WindowEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_WINDOWEVENT_HPP
#define PTK_EVENTS_WINDOWEVENT_HPP

// Local Headers
#include "ptk/core/Event.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Point.hpp"

namespace pTK
{
    /** ResizeEvent class implementation.

        Signal a window resize.
    */
    class ResizeEvent final : public Event
    {
    public:
        /** Constructs ResizeEvent with default values with size.

            @param t_size   associated width and height
            @return         default initialized ResizeEvent
        */
        explicit ResizeEvent(const Size& t_size)
            : Event(Event::Category::Window, Event::Type::WindowResize), size{t_size}
        {

        }

        // Resize values.
        Size size;
    };

    /** MoveEvent class implementation.

        Signal a window movement.
    */
    class MoveEvent final : public Event
    {
    public:
        /** Constructs MoveEvent with default values with size.

            @param t_pos    associated x and y
            @return         default initialized MoveEvent
        */
        explicit MoveEvent(const Point& t_pos)
            : Event(Event::Category::Window, Event::Type::WindowMove), pos{t_pos}
        {
        }

        // Movement coordinates.
        const Point pos;
    };

    /** ScaleEvent class implementation.

        Signal a window scale change.
    */
    class ScaleEvent final : public Event
    {
    public:
        /** ScaleEvent with default values with scale.

            @param t_scale  associated x and y scale
            @return         default initialized ScaleEvent
        */
        explicit ScaleEvent(const Vec2f& t_scale)
            : Event(Event::Category::Window, Event::Type::WindowScale), scale{t_scale}
        {
        }

        // Movement coordinates.
        const Vec2f scale;
    };

    /** PaintEvent class implementation.

        Signal to paint to the window.
    */
    class PaintEvent final : public Event
    {
    public:
        /** PaintEvent with default values with pos and size.

            @param t_pos    coordinates
            @param t_size   area
            @return         default initialized PaintEvent
        */
        PaintEvent(const Point& t_pos, const Size& t_size)
            : Event(Event::Category::Window, Event::Type::WindowPaint),
                pos{t_pos}, size{t_size}
        {
        }

        // Coordinates.
        const Point pos;

        // Area.
        const Size size;
    };
}

#endif // PTK_EVENTS_WINDOWEVENT_HPP
