//
//  platform/win32/Win32Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WIN32_WINDOW_HPP
#define PTK_PlATFORM_WIN32_WINDOW_HPP

// Local Headers
#include "ptk/core/BaseWindow.hpp"

// Windows Headers
#include <windows.h>

namespace pTK
{
    class Window : public BaseWindow
    {
    public:
        Window(const std::string& name, const Vec2u& size);
        virtual ~Window() = default;

        void pollEvents() override;
        void forceDrawAll() override {}
        void close() override {}

    private:
        static LRESULT CALLBACK wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HWND m_handle;
    };
}

#endif // PTK_PlATFORM_WIN32_WINDOW_HPP