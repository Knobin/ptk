//
//  platform/win32/Win32Backend.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WIN32_WIN32BACKEND_HPP
#define PTK_PlATFORM_WIN32_WIN32BACKEND_HPP

// Local Headers
#include "ptk/core/WindowBackend.hpp"
#include "ptk/platform/win32/Win32RasterContext.hpp"

// Windows Headers
#include <windows.h>

namespace pTK
{
    class Window;

    class Win32Backend : public WindowBackend
    {
    public:
        Win32Backend(Window *window, const std::string& name, const Vec2u& size, Backend backend);
        virtual ~Win32Backend() = default;

        /** Function for closing the window.

        */
        void close() override;

        /** Function for showing the window.

        */
        void show() override;

        /** Function for hiding the window.

        */
        void hide() override;

        /** Function for polling the window events.

        */
        void pollEvents() override;

        /** Function for setting the window position.

        */
        void setPosHint(const Point& pos) override;

        ContextBase *getContext() const override;

        void swapbuffers() override;

        Vec2f getDPIScale() const override;

        void resize(const Size& size) override;
        void setLimits(const Size& min, const Size& max) override;

        void beginPaint() override;
        void endPaint() override;

    private:
        static LRESULT CALLBACK wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        Size calcAdjustedWindowSize(const Size& from, DWORD style) const;

    private:
        Window *m_parentWindow;
        HWND m_handle;
        Win32RasterContext *rasterCanvas;
        PAINTSTRUCT ps;
    };
}

#endif // PTK_PlATFORM_WIN32_WIN32WINDOW_HPP