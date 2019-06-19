//
//  core/eventshandler.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_CORE_EVENTHANDLING_HPP
#define PTK_CORE_EVENTHANDLING_HPP

// Local Headers
#include "ptk/core/event.hpp"

namespace pTK
{
    /** EventHandling class implementation.
     
     This class is to handle the incoming events.
     
     Depending on the event category and type, one of
     the defined functions below will be called.
     */
    class EventHandling
    {
    public:
        virtual ~EventHandling() = default;
        
        /** Function for handling KeyEvent.
         
         @param Event   KeyEvent to handle
         */
        virtual void key_event(Event*) {}
        
        /** Function for handling MouseEvent.
         
         @param Event   MouseEvent to handle
         */
        virtual void mouse_event(Event*) {}
    };
}

#endif // PTK_CORE_EVENTHANDLING_HPP
