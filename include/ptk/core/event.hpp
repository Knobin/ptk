//
//  core/event.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENT_HPP
#define PTK_CORE_EVENT_HPP

namespace pTK
{
    /** EventType enum class implementation.
     
     This enum class is to specify the event type of the event.
     */
    enum class EventType
    {
        NONE = 0,
        WindowClose, WindowLostFocus, WindowFocus, WindowDraw, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    /** EventCategory enum class implementation.
     
     This enum class is to specify the category type of the event.
     */
    enum class EventCategory
    {
        NONE = 0,
        Window,
        Key,
        Mouse
    };

    /** Event class implementation.
     
     This class is the base class for events.
     It specifies which event that occured and cannot be
     created without specifying the category and type.
     
     EventCategory and EventType cannot be changed after the
     event is created.
     */
    class Event
    {
    public:
        /** Constructs Event with default values with t_category
         and t_type.
         
         @param t_category  category of event
         @param t_type      type of event
         @return            default initialized Event
         */
        Event(EventCategory t_category, EventType t_type)
            : m_type{t_type}, m_category{t_category}
        {
        }
        virtual ~Event() = default;
        
        /** Function for retrieving the type of the event.
         
         @return    Type of event
         */
        EventType type() const { return m_type; }
        
        /** Function for retrieving the category of the event.
         
         @return    Type of event
         */
        EventCategory category() const { return m_category; }

    protected:
        EventType m_type;
        EventCategory m_category;
    };
}

#endif // PTK_CORE_EVENT_HPP
