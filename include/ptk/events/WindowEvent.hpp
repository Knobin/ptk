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
            : Event(Event::Category::Window, Event::Type::WindowMoved), pos{t_pos}
        {
        }

        // Movement coordinates.
        const Point pos;
    };
}

#endif // PTK_EVENTS_WINDOWEVENT_HPP
