//
//  platform/win/Platform_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

#ifndef PTK_PLATFORM_WIN_PLATFORM_HPP
#define PTK_PLATFORM_WIN_PLATFORM_HPP

// Local Headers
#include "ptk/core/platform/ApplicationBase.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    /** Application_win class implementation.

        Base application implementation for Windows.
        This class will be instantiated when the Application is created.
    */
    class Application_win : public ApplicationBase
    {
    public:
        /** Constructs Application_win with default values.

            @return        default initialized Application_win
        */
        Application_win();

        /** Deconstructor for Application_win

        */
        virtual ~Application_win();

        int messageLoop() override;

        /** Function for closing the application should close.

        */
        void close() override;

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

        /** Function for converting std::string to UTF16 std::stringw.

            @param str  string to convert
            @return     UTF16 string
        */
        static std::wstring stringToUTF16(const std::string& str);
    };
}

#endif // PTK_PLATFORM_WIN_PLATFORM_HPP
