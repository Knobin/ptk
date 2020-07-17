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
            WindowClose, WindowLostFocus, WindowFocus, WindowPaint, WindowResize,
            WindowMove, WindowMinimize, WindowRestore, WindowScale,
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
         
            @param t_category  category of event
            @param t_type      type of event
            @return            default initialized Event
        */
        Event(Category t_category, Type t_type)
            : category{t_category}, type{t_type}
        {
        }

        /** Deconstructor for Event.

        */
        virtual ~Event() = default;

        // Category of the event.
        const Category category;

        // Type of the event.
        const Type type;
    };
}

#endif // PTK_CORE_EVENT_HPP
