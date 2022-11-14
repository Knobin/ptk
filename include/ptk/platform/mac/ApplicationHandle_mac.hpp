//
//  platform/mac/ApplicationHandle_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PLATFORM_MAC_APPLICATION_HPP
#define PTK_PLATFORM_MAC_APPLICATION_HPP

// pTK Headers
#include "ptk/menu/MenuBar.hpp"
#include "ptk/platform/base/ApplicationHandle.hpp"

namespace pTK
{
    /** ApplicationHandle_mac class implementation.

        Base application implementation for macOS.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandle_mac : public ApplicationHandle
    {
    public:
        // TODO(knobin): Add documentation.
        explicit ApplicationHandle_mac(std::string_view name);

        // TODO(knobin): Add documentation.
        virtual ~ApplicationHandle_mac();

        /** Function for polling all the window events.

        */
        void pollEvents() override;

        /** Function for waiting for an event.

        */
        void waitEvents() override;

        /** Function for waiting for an event with a timout.

            @param ms   max time to wait for an event
        */
        void waitEventsTimeout(uint32_t ms) override;

        // TODO(knobin): Add documentation.
        static void SetMenuBar(const Ref<MenuBar>& menuBar);
    };
} // namespace pTK

#endif // PTK_PLATFORM_MAC_APPLICATION_HPP
