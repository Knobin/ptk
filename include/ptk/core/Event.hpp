//
//  core/Event.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENT_HPP
#define PTK_CORE_EVENT_HPP

namespace pTK
{
    /** Event class implementation.
     
        This class is the base class for events.
        It specifies which event that occured and cannot be
        created without specifying the category and type.
     
        Event::Category and Event::Type cannot be changed after the
        event is created.
    */
    class Event
    {
    public:
        /** Event::Type enum class implementation.
     
            This enum class is to specify the event type of the event.
        */
        enum class Type
        {
            NONE = 0,
            WindowClose, WindowLostFocus, WindowFocus, WindowDraw, WindowResize, WindowMoved,
            KeyPressed, KeyReleased,
            MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
        };

        /** Event::Category enum class implementation.
        
            This enum class is to specify the category type of the event.
        */
        enum class Category
        {
            NONE = 0,
            Window,
            Key,
            Mouse
        };

    public:
        /** Constructs Event with default values with t_category
            and t_type.
         
            @param category  category of event
            @param type      type of event
            @return            default initialized Event
        */
        Event(Category category, Type type)
            : m_type{type}, m_category{category}
        {
        }

        /** Deconstructor for Event.

        */
        virtual ~Event() = default;
        
        /** Function for retrieving the type of the event.
         
            @return    Type of event
        */
        Type type() const { return m_type; }
        
        /** Function for retrieving the category of the event.
         
            return    Type of event
        */
        Category category() const { return m_category; }

    protected:
        Type m_type;
        Category m_category;
    };
}

#endif // PTK_CORE_EVENT_HPP
