//
//  platform/winapi/Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WINAPI_WINDOW_HPP
#define PTK_PlATFORM_WINAPI_WINDOW_HPP

// Windows Headers
#include <windows.h>

namespace pTK
{
    namespace WinAPI
    {
        class Window
        {
        public:
            Window();
            ~Window();

        private:
            static LRESULT CALLBACK wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        private:
            HWND m_handle;
        };
    }
}

#endif // PTK_PlATFORM_WINAPI_WINDOW_HPP