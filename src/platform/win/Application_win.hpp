//
//  platform/win/Platform_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

#ifndef PTK_PLATFORM_WIN_PLATFORM_HPP
#define PTK_PLATFORM_WIN_PLATFORM_HPP

// pTK Headers
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
        static void Init(const std::string& name);
        static void Destroy();

        static Application_win *Instance();

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

        /** Callback that will be called when a window is added to the
            application.

            @param key  ptk window id
        */
        void onWindowAdd(const std::pair<int32, Window*> item) override;

        /** Callback that will be called when a window is removed from the
            application.

            @param key  ptk window id
        */
        void onWindowRemove(const std::pair<int32, Window*> item) override;

    private:
        Application_win() = default;
        virtual ~Application_win() = default;

        static Application_win s_Instance;
        static bool s_Initialized;
    };
}

#endif // PTK_PLATFORM_WIN_PLATFORM_HPP
