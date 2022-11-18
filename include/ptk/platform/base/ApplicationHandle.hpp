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

            @param ms       max time to wait for an event
        */
        virtual void waitEventsTimeout(uint32_t ms) = 0;

        /** Callback for when the application should be closed.

            Use this function to exit event loop and do the necessary
            things to stop the application from running. The app is
            considered "dead" here and no other calls will be done.

            Note: Current Windows will be removed with onWindowRemove callback
                  before the call to this function.

            Note: This will not be called on destruction, use destructor for that.
        */
        virtual void onApplicationClose() {}

        /** Callback that will be called when a window is added to the
            application.

            Note: Window has been added before calling this callback.

            @param key      window id
            @param window   pointer to window
        */
        virtual void onWindowAdd(int32_t UNUSED(key), Window* UNUSED(window)) {}

        /** Callback that will be called when a window is removed from the
            application.

            Note: Window will be removed after the call to this callback.

            @param key      window id
            @param window   pointer to window
        */
        virtual void onWindowRemove(int32_t UNUSED(key), Window* UNUSED(window)) {}
    };
} // namespace pTK

#endif // PTK_CORE_PLATFORM_APPLICATIONHANDLE_HPP
