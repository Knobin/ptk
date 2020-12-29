//
//  platform/win/Platform_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_UNIX_APPLICATION_HPP
#define PTK_PLATFORM_UNIX_APPLICATION_HPP

// Local Headers
#include "ptk/core/platform/ApplicationBase.hpp"
#include "x11.hpp"

namespace pTK
{
    class MainWindow_unix;

    /** Application_unix class implementation.

        Base application implementation for Unix.
        This class will be instantiated when the Application is created.
    */
    class Application_unix : public ApplicationBase
    {
    public:
        /** Constructs Application_unix with default values.

            @return        default initialized Application_unix
        */
        Application_unix();

        /** Deconstructor for Application_unix

        */
        virtual ~Application_unix();

        // TODO: Add documentation.
        int messageLoop() override;

        /** Function for closing the application should close.

        */
        void close() override;

        // TODO: Add documentation.
        void pollEvents() override;

        // TODO: Add documentation.
        void waitEvents() override;

        // TODO: Add documentation.
        void waitEventsTimeout(uint ms) override;

    public:
        // TODO: Add documentation.
        static Display *Display();
        
        // TODO: Add documentation.
        static XContext Context();

        // TODO: Add documentation.
        static ::Window Root();

        // TODO: Add documentation.
        static int Screen();

    private:
        bool init();
        void handleEvent(XEvent *event);
    };
}

#endif // PTK_PLATFORM_UNIX_APPLICATION_HPP
