//
//  platform/unix/ApplicationHandleUnix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_UNIX_APPLICATIONHANDLE_HPP
#define PTK_PLATFORM_UNIX_APPLICATIONHANDLE_HPP

// Local Headers
#include "x11.hpp"

// pTK Headers
#include "ptk/platform/ApplicationHandle.hpp"

namespace pTK::Platform
{
    /** ApplicationHandleUnix class implementation.

        Base application implementation for Unix.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandleUnix : public ApplicationHandle
    {
    public:
        /** Constructs ApplicationHandleUnix with name.

            @param base     valid pointer to application base
            @param name     name of the application
            @return         default initialized ApplicationHandleUnix with base and name
        */
        ApplicationHandleUnix(ApplicationBase* base, std::string_view name);

        /** Destructor for ApplicationHandleUnix

        */
        virtual ~ApplicationHandleUnix();

        /** Function for polling all the window events.

        */
        void pollEvents() override;

        /** Function for waiting for an event.

        */
        void waitEvents() override;

        /** Function for waiting for an event with a timeout.

            @param ms       max time to wait for an event
        */
        void waitEventsTimeout(uint32_t ms) override;

    public:
        /** Function for retrieving the xlib Display structure.

            @return     pointer to xlib Display structure
        */
        static ::Display* Display();

        /** Function for retrieving the xlib XContext.

            @return     xlib XContext
        */
        static ::XContext Context();

        /** Function for retrieving the xlib Window.

            @return     xlib Window
        */
        static ::Window Root();

        /** Function for retrieving the xlib screen number.

            @return     xlib screen number
        */
        static int Screen();

    private:
        // Helper for handling a XEvent.
        void handleEvent(XEvent* event);
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_UNIX_APPLICATIONHANDLE_HPP
