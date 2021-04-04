//
//  platform/mac/Application_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PLATFORM_MAC_APPLICATION_HPP
#define PTK_PLATFORM_MAC_APPLICATION_HPP

// pTK Headers
#include "ptk/core/platform/ApplicationBase.hpp"
#include "ptk/menu/MenuBar.hpp"

namespace pTK
{
    class MainWindow_mac;

    /** Application_mac class implementation.

        Base application implementation for macOS.
        This class will be instantiated when the Application is created.
    */
    class Application_mac : public ApplicationBase
    {
    public:
        static void Init(const std::string& name);
        static void Destroy();
        static ApplicationBase *Instance();

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
        Application_mac() = default;
        virtual ~Application_mac() = default;
        
    private:
        static Application_mac s_Instance;
    };
}

#endif // PTK_PLATFORM_MAC_APPLICATION_HPP
