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
    class EventHandling
    {
    public:
        EventHandling() = default;
        virtual ~EventHandling() = default;
        
        // Event processing
        virtual void key_event(Event*) {}
        virtual void mouse_event(Event*) {}
    };
}

#endif // PTK_CORE_EVENTHANDLING_HPP
