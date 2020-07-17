//
//  platform/win/WinBackend.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WIN_WINBACKEND_HPP
#define PTK_PlATFORM_WIN_WINBACKEND_HPP

// Local Headers
#include "ptk/core/platform/WindowBackend.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/events/MouseEvent.hpp"

// C++ Headers
#include <map>

// Windows Headers
#include <windows.h>

namespace pTK
{
    class Window;
    struct WinBackendData;

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

            @return     true if operation is successful, otherwise false
        */
        bool close() override;

        /** Function for showing the window.

            @return     true if operation is successful, otherwise false
        */
        bool show() override;

        /** Function for hiding the window.

            @return     true if operation is successful, otherwise false
        */
        bool hide() override;

        /** Function for retrieving if the window is hidden.

            @return     true if window is hidden, otherwise false
        */
        bool isHidden() const override;

        /** Function for polling the window events.

            @param ms   max time to wait for an event
        */
        void pollEvents(uint ms) override;

        /** Function for setting the position of the window.

            @param pos  position to set
            @return     true if operation is successful, otherwise false
        */
        bool setPosHint(const Point& pos) override;

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
            @return     true if operation is successful, otherwise false
        */
        bool resize(const Size& size) override;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
            @return     true if operation is successful, otherwise false
        */
        bool setLimits(const Size&, const Size&) override;

        /** Function for initiating the drawing.

            This function will be called before any drawing is supposed to be done.
        */
        void beginPaint() override;

        /** Function for ending the drawing.

            This function will be called after the drawing is completed.
        */
        void endPaint() override;

        /** Function for setting the title of the window.

            @param name     title to show
            @return         true if operation is successful, otherwise false
        */
        bool setTitle(const std::string& name) override;

        /** Function for setting the icon of the window.

            Note: The function expects the pixels to be in a RGBA format with size of 32 bits (4 bytes).
            Therefore, size of the pixel array is width * height * 32.
            This is why a size parameter is not needed.

            @param width    width of the image
            @param height   height of the image
            @param pixels   image pixels in a RGBA format.
            @return         true if operation is successful, otherwise false
        */
        bool setIcon(int32 width, int32 height, byte* pixels)  override;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        void notifyEvent() override;

        /** Function for retrieving the window position.

           @return     Window Position
       */
        Point getWinPos() const override;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        Size getWinSize() const override;

        /** Function for retrieving the current Windows style of the Window.

            @return Window style
        */
        DWORD getWindowStyle() const;

        /** Function for minimizing the window.

            @return     true if operation is successful, otherwise false
        */
        bool minimize() override;

        /** Function for minimizing the window.

        */
        bool isMinimized() const override;

        /** Function for restoring a window from the minimized state.

            @return     true if operation is successful, otherwise false
        */
        bool restore() override;

        /** Function for retrieving the focus status of the window.

            @return     status
        */
        bool isFocused() const override;

        /** Function for hinting the scale of the window.

            @param scale    scale hint
            @return         true if operation is successful, otherwise false
        */
        bool setScaleHint(const Vec2f& scale) override;

        static LRESULT CALLBACK wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        Window *m_parentWindow;
        HWND m_hwnd;
        std::unique_ptr<ContextBase> m_context;
        PAINTSTRUCT m_ps;
        [[maybe_unused]] HDC m_hdc;

        std::unique_ptr<WinBackendData> m_data;
    };
}

#endif // PTK_PlATFORM_WIN_WINBACKEND_HPP