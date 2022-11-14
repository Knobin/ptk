//
//  platform/win/ApplicationHandle_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

#ifndef PTK_PLATFORM_WIN_APPLICATIONHANDLE_HPP
#define PTK_PLATFORM_WIN_APPLICATIONHANDLE_HPP

// pTK Headers
#include "ptk/platform/base/ApplicationHandle.hpp"

// C++ Headers
#include <string>

// TODO(knobin): Fix documentation in this file.

namespace pTK
{
    /** ApplicationHandle_win class implementation.

        Base application implementation for Windows.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandle_win : public ApplicationHandle
    {
    public:
        explicit ApplicationHandle_win(std::string_view name);

        /** Destructor for ApplicationHandle_win

        */
        virtual ~ApplicationHandle_win();

        /** Function for polling all the window events.

        */
        void pollEvents() override;

        /** Function for waiting for an event.

        */
        void waitEvents() override;

        /** Function for waiting for an event with a timeout.

            @param ms   max time to wait for an event
        */
        void waitEventsTimeout(uint32_t ms) override;

        /** Function for converting std::string to UTF16 std::stringw.

            @param str  string to convert
            @return     UTF16 string
        */
        static std::wstring stringToUTF16(const std::string& str);

        /** Callback that will be called when a window is added to the
            application.

            @param key  ptk window id
        */
        void onWindowAdd(int32_t key, Window* window) override;

        /** Callback that will be called when a window is removed from the
            application.

            @param key  ptk window id
        */
        void onWindowRemove(int32_t key, Window* window) override;
    };
} // namespace pTK

#endif // PTK_PLATFORM_WIN_APPLICATIONHANDLE_HPP