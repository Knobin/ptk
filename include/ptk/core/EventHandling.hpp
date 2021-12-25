//
//  core/EventHandling.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_CORE_EVENTHANDLING_HPP
#define PTK_CORE_EVENTHANDLING_HPP

// pTK Headers
#include "ptk/core/EventCallbacks.hpp"
#include "ptk/core/EventFunctions.hpp"
#include "ptk/events/KeyCodes.hpp"

namespace pTK
{
    /** EventHandling class implementation.

        This class is to handle the incoming events.

        Depending on the event category and type, one of
        the defined functions below will be called.
    */
    class EventHandling : public EventCallbacks, public EventFunctions
    {
    public:
        /** Constructs EventHandling with default values.

        */
        EventHandling();

        /** Destructor for EventHandling.

        */
        virtual ~EventHandling() = default;

    private:
        void registerMemberCallbacks();
    };
}

#endif // PTK_CORE_EVENTHANDLING_HPP
