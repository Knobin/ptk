//
//  platform/win/WinBackend.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WIN_WINBACKEND_HPP
#define PTK_PlATFORM_WIN_WINBACKEND_HPP

// Local Headers
#include "ptk/core/WindowBackend.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/events/MouseEvent.hpp"

// C++ Headers
#include <map>

// Windows Headers
#include <windows.h>

namespace pTK
{
    class Window;

    /** WinBackend class implementation.

        This class handles the Windows Window.
    */
    class WinBackend : public WindowBackend
    {
    public:
        /** Constructs WinBackend with default values.

            @param window   parent Window class
            @param name     name of the window
            @param size     size of the window
            @param backend  type of backend
            @return         default initialized WinBackend
        */
        WinBackend(Window *window, const std::string& name, const Size& size, BackendType backend);

        /** Deconstructor for WinBackend.

        */
        virtual ~WinBackend() = default;

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

            @param pos  requested position of the Window.
        */
        void setPosHint(const Point& pos) override;

        /** Function for retrieving the Context.

            @return context
        */
        ContextBase *getContext() const override;

        /** Function for swapping the buffers.

        */
        void swapBuffers() override;

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        Vec2f getDPIScale() const override;

        /** Function for resizing the window.

            @param size  size to set
        */
        void resize(const Size& size) override;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        void setLimits(const Size&, const Size&) override;

        /** Function for initiating the drawing.

            This function will be called before any drawing is supposed to be done.
        */
        void beginPaint() override;

        /** Function for ending the drawing.

            This function will be called after the drawing is completed.
        */
        void endPaint() override;

        /** Function for retrieving the current Windows style of the Window.

            @return Window style
        */
        DWORD getWindowStyle() const;

    private:
        static LRESULT CALLBACK wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static void handleMouseClick(Window *window, Event::Type type, Mouse::Button btn, LPARAM lParam);

    private:
        Window *m_parentWindow;
        HWND m_handle;
        DWORD m_style;
        std::unique_ptr<ContextBase> rasterCanvas;
        PAINTSTRUCT ps;
        Vec2f m_scale;
    };
}

#endif // PTK_PlATFORM_WIN_WINBACKEND_HPP