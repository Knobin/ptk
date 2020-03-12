//
//  platform/win32/Win32Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WIN32_WIN32WINDOW_HPP
#define PTK_PlATFORM_WIN32_WIN32WINDOW_HPP

// Local Headers
#include "ptk/core/WindowBase.hpp"
#include "ptk/platform/win32/Win32RasterCanvas.hpp"

// Windows Headers
#include <windows.h>

namespace pTK
{
    class Window : public WindowBase
    {
    public:
        Window(const std::string& name, const Vec2u& size);
        virtual ~Window() = default;

        void pollEvents() override;
        void forceDrawAll() override;
        void close() override {}

    private:
        static LRESULT CALLBACK wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        Size calcAdjustedWindowSize(const Size& from, DWORD style) const;

    private:
        HWND m_handle;
        Win32RasterCanvas *rasterCanvas;
    };
}

#endif // PTK_PlATFORM_WIN32_WIN32WINDOW_HPP