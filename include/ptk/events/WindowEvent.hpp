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

namespace pTK
{
    /** ResizeEvent class implementation.

        Derived from Event, this class if for creating
        events from the resizing of the window.
    */
    class ResizeEvent : public Event
    {
    public:
        /** Constructs ResizeEvent with default values with width and height.

            @param width   associated width
            @param height  associated height
            @return        default initialized ResizeEvent
        */
        ResizeEvent(Size::value_type width, Size::value_type height)
            : Event(Event::Category::Window, Event::Type::WindowResize), m_size{width, height}
        {
        }

        /** Constructs ResizeEvent with default values with size.

            @param size    associated width and height
            @return        default initialized ResizeEvent
        */
        ResizeEvent(const Size& size)
            : Event(Event::Category::Window, Event::Type::WindowResize), m_size{size}
        {
        }
        ~ResizeEvent() final = default;

        /** Function for retrieving the associated size.

            @return    size
        */
        const Size& getSize() const { return m_size; }

        /** Function for retrieving the associated width.

            @return    width
        */
        Size::value_type width() const { return m_size.width; }

        /** Function for retrieving the associated height.

            @return    height
        */
        Size::value_type height() const { return m_size.height; }

    private:
        Size m_size;
    };
}

#endif // PTK_EVENTS_WINDOWEVENT_HPP
