//
//  platform/mac/ApplicationHandle_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PLATFORM_MAC_APPLICATION_HPP
#define PTK_PLATFORM_MAC_APPLICATION_HPP

// pTK Headers
#include "ptk/platform/base/ApplicationHandle.hpp"
#include "ptk/menu/MenuBar.hpp"

namespace pTK
{
    class WindowHandle_mac;

    /** ApplicationHandle_mac class implementation.

        Base application implementation for macOS.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandle_mac : public ApplicationHandle
    {
    public:
        static void Init(const std::string& name);
        static void Destroy();
        static ApplicationHandle *Instance();

        /** Function for polling all the window events.

        */
        void pollEvents() override;

        /** Function for waiting for an event.

        */
        void waitEvents() override;

        /** Function for waiting for an event with a timout.

            @param ms   max time to wait for an event
        */
        void waitEventsTimeout(uint ms) override;

        static void SetMenuBar(const Ref<MenuBar>& menuBar);

    private:
        ApplicationHandle_mac() = default;
        virtual ~ApplicationHandle_mac() = default;

    private:
        static ApplicationHandle_mac s_Instance;
    };
}

#endif // PTK_PLATFORM_MAC_APPLICATION_HPP
