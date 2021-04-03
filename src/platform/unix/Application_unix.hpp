//
//  platform/win/Platform_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_UNIX_APPLICATION_HPP
#define PTK_PLATFORM_UNIX_APPLICATION_HPP

// Local Headers
#include "x11.hpp"

// pTK Headers
#include "ptk/core/platform/ApplicationBase.hpp"

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
        static void Init(const std::string& name);
        static void Destroy();

        static Application_unix *Instance();

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
        Application_unix() = default;
        virtual ~Application_unix() = default;
        
        bool init();
        void handleEvent(XEvent *event);
    
    private:
        static Application_unix s_Instance;
    };
}

#endif // PTK_PLATFORM_UNIX_APPLICATION_HPP
