//
//  core/platform/ApplicationHandle.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP
#define PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP

// pTK Headers
#include "ptk/Core.hpp"
#include "ptk/util/SingleObject.hpp"

// C++ Header
#include <cstdint>
#include <map>

namespace pTK
{
    class Window;

    /** ApplicationHandle class implementation.

        This is the implementation for the Application backend.
        An implementation of this class must exist for the target platform.
    */
    class PTK_API ApplicationHandle : public SingleObject
    {
    public:
        /** Constructs ApplicationBase with default values.

            @return        default initialized ApplicationBase
        */
        ApplicationHandle() = default;

        /** Destructor for ApplicationBase

        */
        virtual ~ApplicationHandle() = default;

        /** Function for polling all the window events.

        */
        virtual void pollEvents() = 0;

        /** Function for waiting for an event.

        */
        virtual void waitEvents() = 0;

        /** Function for waiting for an event with a timeout.

            @param ms   max time to wait for an event
        */
        virtual void waitEventsTimeout(uint32_t ms) = 0;

        // TODO(knobin): Add docs.
        virtual void onClose() {}

        /** Callback that will be called when a window is added to the
            application.

            @param key  ptk window id
        */
        virtual void onWindowAdd(const std::pair<int32_t, Window*> UNUSED(item)) {}

        /** Callback that will be called when a window is removed from the
            application.

            @param key  ptk window id
        */
        virtual void onWindowRemove(const std::pair<int32_t, Window*> UNUSED(item)) {}
    };
}

#endif // PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP
