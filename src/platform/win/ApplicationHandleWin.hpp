//
//  platform/win/ApplicationHandleWin.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

#ifndef PTK_PLATFORM_WIN_APPLICATIONHANDLE_HPP
#define PTK_PLATFORM_WIN_APPLICATIONHANDLE_HPP

// pTK Headers
#include "ptk/platform/ApplicationHandle.hpp"

namespace pTK::Platform
{
    /** ApplicationHandleWin class implementation.

        Base application implementation for Windows.
        This class will be instantiated when the Application is created.
    */
    class PTK_API ApplicationHandleWin : public ApplicationHandle
    {
    public:
        /** Constructs ApplicationHandleWin with name.

            @param base     valid pointer to application base
            @param name     name of the application
            @return         default initialized ApplicationHandleWin with base and name
        */
        ApplicationHandleWin(ApplicationBase* base, std::string_view name);

        /** Destructor for ApplicationHandleWin

        */
        virtual ~ApplicationHandleWin();

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

        /** Function for converting std::string to UTF16 std::wstring.

            @param str  string to convert
            @return     UTF16 string
        */
        static std::wstring stringToUTF16(const std::string& str);

        /** Callback that will be called when a window is added to the application.

            Note: Window has been added before calling this callback.

            @param key      window id
            @param window   pointer to window
        */
        void onWindowAdd(int32_t key, Window* window) override;

        /** Callback that will be called when a window is removed from the application.

            Note: Window will be removed after the call to this callback.

            @param key      window id
            @param window   pointer to window
        */
        void onWindowRemove(int32_t key, Window* window) override;
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_WIN_APPLICATIONHANDLE_HPP