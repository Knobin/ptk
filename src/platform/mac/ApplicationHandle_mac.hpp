//
//  src/platform/mac/ApplicationHandle_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PLATFORM_MAC_APPLICATION_HPP
#define PTK_PLATFORM_MAC_APPLICATION_HPP

// pTK Headers
#include "ptk/menu/MenuBar.hpp"
#include "ptk/platform/ApplicationHandle.hpp"

namespace pTK::Platform
{
    /** ApplicationHandle_mac class implementation.

        Base application implementation for macOS.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandle_mac : public ApplicationHandle
    {
    public:
        /** Constructs ApplicationHandle_mac with name.

            @return        default initialized ApplicationHandle_mac with name
        */
        ApplicationHandle_mac(ApplicationBase* appBase, std::string_view name);

        /** Destructor for ApplicationHandle_mac

        */
        virtual ~ApplicationHandle_mac();

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

        /** Function for setting the MenuBar of the Application.

            Since macOS has a common MenuBar for the application (not
            like windows that has one per window), the MenuBar will be
            set here using this function.

            @param menuBar      MenuBar to set
        */
        static void SetMenuBar(const Ref<MenuBar>& menuBar);

        /** Callback for when the application is terminating.

            Application has been closed and is exiting.
            Use this function to terminate at the platform level if necessary.
        */
        void terminate() override;
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_MAC_APPLICATION_HPP
