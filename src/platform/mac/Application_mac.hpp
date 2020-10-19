//
//  platform/mac/Application_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PLATFORM_MAC_APPLICATION_HPP
#define PTK_PLATFORM_MAC_APPLICATION_HPP

// Local Headers
#include "ptk/core/platform/ApplicationBase.hpp"

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
        /** Constructs Application_mac with default values.

            @return        default initialized Application_mac
        */
        Application_mac();

        /** Deconstructor for Application_mac

        */
        virtual ~Application_mac();

        /** Function for closing the application.

        */
        void close() override;
        
        int messageLoop() override;

        static void addMacWindow(long key, MainWindow_mac* window);
        static void removeMacWindow(long key);
        
        void pollEvents() override;
        void waitEvents() override;
        void waitEventsTimeout(uint ms) override;
    };
}

#endif // PTK_PLATFORM_MAC_APPLICATION_HPP
