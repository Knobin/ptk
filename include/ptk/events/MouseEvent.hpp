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
#include "ptk/util/Point.hpp"
#include "ptk/util/Vec2.hpp"

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
        MotionEvent(int posx, int posy)
            : Event(EventCategory::Mouse, EventType::MouseMoved), m_pos{posx, posy}
        {
        }
        
        /** Constructs MotionEvent with default values with pos.
         
         @param pos     associated x and y position
         @return        default initialized MotionEvent
         */
        MotionEvent(const Point& pos)
            : Event(EventCategory::Mouse, EventType::MouseMoved), m_pos{pos}
        {
        }
        virtual ~MotionEvent() = default;

        /** Function for retrieving the associated x position.
         
         @return    x position
         */
        int getX() const { return m_pos.x; }
        
        /** Function for retrieving the associated y position.
         
         @return    y position
         */
        int getY() const { return m_pos.y; }
        
        /** Function for retrieving the associated position.
         
         @return    y position
         */
        const Point& getPos() const { return m_pos; }
    private:
        Point m_pos;
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
        ScrollEvent(float x_offset, float y_offset)
            : Event(EventCategory::Mouse, EventType::MouseScrolled), m_offset{x_offset, y_offset}
        {
        }
        
        /** Constructs ScrollEvent with default values with offset.
         
         @param offset  associated x and y offset
         @return        default initialized ScrollEvent
         */
        ScrollEvent(const Vec2f& offset)
            : Event(EventCategory::Mouse, EventType::MouseScrolled), m_offset{offset}
        {
        }
        virtual ~ScrollEvent() = default;

        /** Function for retrieving the associated x offset.
         
         @return    x offset
         */
        float getX() const { return m_offset.x; }
        
        /** Function for retrieving the associated y offset.
         
         @return    y offset
         */
        float getY() const { return m_offset.y; }
        
        /** Function for retrieving the associated offset.
         
         @return    offset
         */
        const Vec2f& getOffset() const { return m_offset; }
        
    private:
        Vec2f m_offset;
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
        MouseButton getButton() const { return m_button; }
    private:
        MouseButton m_button;
    };
}

#endif // PTK_EVENTS_MOUSEEVENT_HPP
