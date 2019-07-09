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
#include "ptk/util/Vec2.hpp"

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
        ResizeEvent(uint width, uint height, uint contentWidth, uint contentHeight)
            : Event(EventCategory::Window, EventType::WindowResize), m_size{width, height},
                m_contentSize{contentWidth, contentHeight}
        {
        }
        
        /** Constructs ResizeEvent with default values with size.
         
         @param size    associated width and height
         @return        default initialized ResizeEvent
         */
        ResizeEvent(const Vec2u& size, const Vec2u& contentSize)
            : Event(EventCategory::Window, EventType::WindowResize), m_size{size},
                m_contentSize{contentSize}
        {
        }
        ~ResizeEvent() final = default;

        /** Function for retrieving the associated width.
         
         @return    width
         */
        uint getWidth() const { return m_size.x; }
        
        /** Function for retrieving the associated height.
         
         @return    height
         */
        uint getHeight() const { return m_size.y; }
        
        /** Function for retrieving the associated size.
         
         @return    size
         */
        const Vec2u& getSize() const { return m_size; }
        
        /** Function for retrieving the associated content size.
         
         @return    size
         */
        const Vec2u& getContentSize() const { return m_contentSize; }

    private:
        Vec2u m_size;
        Vec2u m_contentSize;
    };
}

#endif // PTK_EVENTS_WINDOWEVENT_HPP
