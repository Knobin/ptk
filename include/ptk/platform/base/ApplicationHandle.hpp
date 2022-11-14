//
//  core/platform/ApplicationHandle.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP
#define PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP

// pTK Headers
#include "ptk/core/ApplicationBase.hpp"

namespace pTK
{
    /** ApplicationHandle class implementation.

        This is the implementation for the Application backend.
        An implementation of this class must exist for the target platform.
    */
    class PTK_API ApplicationHandle : public ApplicationBase
    {
    public:
        // Use constructors from ApplicationBase.
        using ApplicationBase::ApplicationBase;

        /** Destructor for ApplicationHandle

        */
        virtual ~ApplicationHandle() = default;

    protected:
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

        /** Callback for when the application should be closed.

            Note: This will not be called on destruction, use destructor for that.
        */
        virtual void onClose() {}

        /** Callback that will be called when a window is added to the
            application.

            @param key  ptk window id
        */
        virtual void onWindowAdd(int32_t UNUSED(key), Window* UNUSED(window)) {}

        /** Callback that will be called when a window is removed from the
            application.

            @param key  ptk window id
        */
        virtual void onWindowRemove(int32_t UNUSED(key), Window* UNUSED(window)) {}
    };
} // namespace pTK

#endif // PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP
