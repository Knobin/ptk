//
//  platform/unix/ApplicationHandle_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_UNIX_APPLICATIONHANDLE_HPP
#define PTK_PLATFORM_UNIX_APPLICATIONHANDLE_HPP

// Local Headers
#include "ptk/platform/unix/x11.hpp"

// pTK Headers
#include "ptk/platform/base/ApplicationHandle.hpp"

namespace pTK
{
    /** ApplicationHandle_unix class implementation.

        Base application implementation for Unix.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandle_unix : public ApplicationHandle
    {
    public:
        /** Constructs ApplicationHandle_unix with name.

            @return        default initialized ApplicationHandle_unix with name
        */
        explicit ApplicationHandle_unix(std::string_view name);

        /** Destructor for ApplicationHandle_unix

        */
        virtual ~ApplicationHandle_unix();

        // TODO: Add documentation.
        void pollEvents() override;

        // TODO: Add documentation.
        void waitEvents() override;

        // TODO: Add documentation.
        void waitEventsTimeout(uint32_t ms) override;

    public:
        // TODO: Add documentation.
        static ::Display* Display();

        // TODO: Add documentation.
        static ::XContext Context();

        // TODO: Add documentation.
        static ::Window Root();

        // TODO: Add documentation.
        static int Screen();

    private:
        void handleEvent(XEvent* event);
    };
} // namespace pTK

#endif // PTK_PLATFORM_UNIX_APPLICATIONHANDLE_HPP
