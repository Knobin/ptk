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

namespace pTK
{
    /** ResizeEvent class implementation.
     
     Derived from Event, this class if for creating
     events from the resizing of the window.
     */
    class ResizeEvent : public Event
    {
    public:
        /** Constructs ResizeEvent with default values with t_width and t_height.
         
         @return    default initialized ResizeEvent
         */
        ResizeEvent(unsigned int t_width, unsigned int t_height)
            : Event(EventCategory::Window, EventType::WindowResize), m_width{t_width}, m_height{t_height}
        {
        }
        ~ResizeEvent() final = default;

        /** Function for retrieving the associated width.
         
         @return    new width
         */
        unsigned int get_width() const { return m_width; }
        
        /** Function for retrieving the associated height.
         
         @return    new height
         */
        unsigned int get_height() const { return m_height; }

    private:
        unsigned int m_width;
        unsigned int m_height;
    };
}

#endif // PTK_EVENTS_WINDOWEVENT_HPP
