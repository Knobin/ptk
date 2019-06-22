//
//  events/MouseEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_MOUSEEVENT_HPP
#define PTK_EVENTS_MOUSEEVENT_HPP

// Local Headers
#include "ptk/core/Event.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <cstdint>

namespace pTK
{
    /** MouseButton enum class implementation.
     
     This enum class is to specify which button on the mouse
     if pressed or released.
     */
    enum class MouseButton
    {
        NONE = 0,
        Left,
        Middle,
        Right
    };

    /** MotionEvent class implementation.
     
     Derived from Event, this class if for creating
     events from the motion of the mouse.
     */
    class MotionEvent : public Event
    {
    public:
        /** Constructs MotionEvent with default values with t_posx and t_posy.
         
         @param posx    associated x position
         @param posy    associated y position
         @return        default initialized MotionEvent
         */
        MotionEvent(int32_t posx, int32_t posy)
            : Event(EventCategory::Mouse, EventType::MouseMoved), m_pos{posx, posy}
        {
        }
        
        /** Constructs MotionEvent with default values with pos.
         
         @param pos     associated x and y position
         @return        default initialized MotionEvent
         */
        MotionEvent(const Vec2<int32_t>& pos)
            : Event(EventCategory::Mouse, EventType::MouseMoved), m_pos{pos}
        {
        }
        virtual ~MotionEvent() = default;

        /** Function for retrieving the associated x position.
         
         @return    x position
         */
        int32_t get_posx() const { return m_pos.x; }
        
        /** Function for retrieving the associated y position.
         
         @return    y position
         */
        int32_t get_posy() const { return m_pos.y; }
        
        /** Function for retrieving the associated position.
         
         @return    y position
         */
        const Vec2<int32_t>& get_pos() const { return m_pos; }
    private:
        Vec2<int32_t> m_pos;
    };

    /** ScrollEvent class implementation.
     
     Derived from Event, this class if for creating
     events from the scroll motion of the mouse.
     */
    class ScrollEvent : public Event
    {
    public:
        /** Constructs ScrollEvent with default values with t_xoffset and t_yoffset.
         
         @param x_offset    associated x offset
         @param y_offset    associated y offset
         @return            default initialized ScrollEvent
         */
        ScrollEvent(int32_t x_offset, int32_t y_offset)
            : Event(EventCategory::Mouse, EventType::MouseScrolled), m_offset{x_offset, y_offset}
        {
        }
        
        /** Constructs ScrollEvent with default values with offset.
         
         @param offset  associated x and y offset
         @return        default initialized ScrollEvent
         */
        ScrollEvent(const Vec2<int32_t>& offset)
        : Event(EventCategory::Mouse, EventType::MouseScrolled), m_offset{offset}
        {
        }
        virtual ~ScrollEvent() = default;

        /** Function for retrieving the associated x offset.
         
         @return    x offset
         */
        int get_x_offset() const { return m_offset.x; }
        
        /** Function for retrieving the associated y offset.
         
         @return    y offset
         */
        int get_y_offset() const { return m_offset.y; }
        
        /** Function for retrieving the associated offset.
         
         @return    offset
         */
        const Vec2<int32_t>& get_offset() const { return m_offset; }
        
    private:
        Vec2<int32_t> m_offset;
    };

    /** ButtonEvent class implementation.
     
     Derived from Event, this class if for creating
     events from the mouse. Such as button presses or
     releases.
     */
    class ButtonEvent : public MotionEvent
    {
    public:
        /** Constructs ButtonEvent with default values with type,
         t_button, t_posx and t_posy.
         
         @param type    press or release
         @param button  which button
         @param posx    associated x position
         @param posy    associated y position
         @return        default initialized ButtonEvent
         */
        ButtonEvent(EventType type, MouseButton button, int posx, int posy)
            : MotionEvent(posx, posy), m_button{button}
        {
            m_type = type;
        }
        virtual ~ButtonEvent() final = default;

        /** Function for retrieving the associated button.
         
         @return    Pressed or Released button
         */
        MouseButton get_button() const { return m_button; }
    private:
        MouseButton m_button;
    };
}

#endif // PTK_EVENTS_MOUSEEVENT_HPP
