//
//  events/mouseevent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_EVENTS_MOUSEEVENT_HPP
#define PTK_EVENTS_MOUSEEVENT_HPP

// Local Headers
#include "ptk/core/event.hpp"

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
         
         @return    default initialized MotionEvent
         */
        MotionEvent(int t_posx, int t_posy)
            : Event(EventCategory::Mouse, EventType::MouseMoved), m_posx{t_posx}, m_posy{t_posy}
        {
        }
        virtual ~MotionEvent() = default;

        /** Function for retrieving the associated x position.
         
         @return    x position
         */
        int get_posx() const { return m_posx; }
        
        /** Function for retrieving the associated y position.
         
         @return    y position
         */
        int get_posy() const { return m_posy; }
    private:
        int m_posx;
        int m_posy;
    };

    /** ScrollEvent class implementation.
     
     Derived from Event, this class if for creating
     events from the scroll motion of the mouse.
     */
    class ScrollEvent : public Event
    {
    public:
        /** Constructs ScrollEvent with default values with t_xoffset and t_yoffset.
         
         @return    default initialized ScrollEvent
         */
        ScrollEvent(int t_xoffset, int t_yoffset)
            : Event(EventCategory::Mouse, EventType::MouseScrolled), m_xoffset{t_xoffset}, m_yoffset{t_yoffset}
        {
        }
        virtual ~ScrollEvent() = default;

        /** Function for retrieving the associated x offset.
         
         @return    x offset
         */
        int get_x_offset() const { return m_xoffset; }
        
        /** Function for retrieving the associated y offset.
         
         @return    y offset
         */
        int get_y_offset() const { return m_yoffset; }
    private:
        int m_xoffset;
        int m_yoffset;
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
         
         @return    default initialized ButtonEvent
         */
        ButtonEvent(EventType type, MouseButton t_button, int t_posx, int t_posy)
            : MotionEvent(t_posx, t_posy), m_button{t_button}
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
