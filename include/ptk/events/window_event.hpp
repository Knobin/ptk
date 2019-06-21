//
//  events/windowevent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_WINDOWEVENT_HPP
#define PTK_EVENTS_WINDOWEVENT_HPP

// Local Headers
#include "ptk/core/event.hpp"
#include "ptk/util/vec2.hpp"

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
        ResizeEvent(uint32_t width, uint32_t height)
            : Event(EventCategory::Window, EventType::WindowResize), m_size{width, height}
        {
        }
        
        /** Constructs ResizeEvent with default values with size.
         
         @param size    associated width and height
         @return        default initialized ResizeEvent
         */
        ResizeEvent(const Vec2<uint32_t>& size)
            : Event(EventCategory::Window, EventType::WindowResize), m_size{size}
        {
        }
        ~ResizeEvent() final = default;

        /** Function for retrieving the associated width.
         
         @return    width
         */
        uint32_t get_width() const { return m_size.x; }
        
        /** Function for retrieving the associated height.
         
         @return    height
         */
        uint32_t get_height() const { return m_size.y; }
        
        /** Function for retrieving the associated size.
         
         @return    size
         */
        const Vec2<uint32_t>& get_size() const { return m_size; }

    private:
        Vec2<uint32_t> m_size;
    };
}

#endif // PTK_EVENTS_WINDOWEVENT_HPP
