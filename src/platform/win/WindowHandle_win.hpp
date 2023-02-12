//
//  platform/win/MainWindow_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_PlATFORM_WIN_WINDOWHANDLE_HPP
#define PTK_PlATFORM_WIN_WINDOWHANDLE_HPP

// Local Headers
#include "MenuBarUtil_win.hpp"

// pTK Headers
#include "ptk/platform/base/WindowHandle.hpp"

// Windows Headers
#define NOMINMAX
#include <windows.h>

namespace pTK::Platform
{
    /** WindowHandle_win class implementation.

        This class handles the Windows Window.
    */
    class PTK_API WindowHandle_win : public WindowHandle
    {
    public:
        /** Constructs WindowHandle_win with default values.

            @param base     pointer to WindowBase
            @param name     name of the window
            @param size     size of the window
            @param backend  type of backend
            @return         default initialized WindowHandle_win
        */
        WindowHandle_win(WindowBase* base, const std::string& name, const Size& size, const WindowInfo& flags);

        /** Destructor for WindowHandle_win.

        */
        virtual ~WindowHandle_win();

        /** Function for closing the window.

            @return     true if operation is successful, otherwise false
        */
        bool close() override;

        /** Function for showing the window.

            @return     true if operation is successful, otherwise false
        */
        void show() override;

        /** Function for hiding the window.

            @return     true if operation is successful, otherwise false
        */
        void hide() override;

        /** Function for retrieving if the window is hidden.

            @return     true if window is hidden, otherwise false
        */
        [[nodiscard]] bool isHidden() const override;

        /** Function for setting the position of the window.

            @param pos  position to set
            @return     true if operation is successful, otherwise false
        */
        bool setPosHint(const Point& pos) override;

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        [[nodiscard]] Vec2f getDPIScale() const override;

        /** Function for resizing the window.

            @param size  size to set
            @return     true if operation is successful, otherwise false
        */
        bool resize(const Size& size) override;

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
        bool setIcon(int32_t width, int32_t height, uint8_t* pixels) override;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        void notifyEvent() override;

        /** Function for retrieving the window position.

           @return     Window Position
       */
        [[nodiscard]] Point getPosition() const override;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        [[nodiscard]] Size getSize() const override;

        /** Function for retrieving the current Windows style of the Window.

            @return Window style
        */
        [[nodiscard]] DWORD getWindowStyle() const;

        /** Function for minimizing the window.

            @return     true if operation is successful, otherwise false
        */
        bool minimize() override;

        /** Function for minimizing the window.

        */
        [[nodiscard]] bool isMinimized() const override;

        /** Function for restoring a window from the minimized state.

            @return     true if operation is successful, otherwise false
        */
        bool restore() override;

        /** Function for retrieving the focus status of the window.

            @return     status
        */
        [[nodiscard]] bool isFocused() const override;

        /** Function for hinting the scale of the window.

            @param scale    scale hint
            @return         true if operation is successful, otherwise false
        */
        bool setScaleHint(const Vec2f& scale) override;

        /** Function for invalidating the window.

        */
        void inval() override;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        void setLimits(const Size& min, const Size& max) override;

        /** Function for retrieving HWND from the window.

            @return    HWND
        */
        [[nodiscard]] HWND handle() const;

        /** Function for retrieving HACCEL from the window.

            @return    HACCEL
        */
        [[nodiscard]] HACCEL accelTable() const;

        // WndPro callback.
        static LRESULT CALLBACK WndPro(HWND hwnd, UINT msg, WPARAM wParam, ::LPARAM lParam);

    public:
        // Data struct that the static Win32 functions can manipulate freely.
        struct Data
        {
            WindowHandle_win* window{nullptr};
            bool minimized{false};
            uint32_t wait{0};
            MenuBarUtil_win::MenuMap menuItems{};
            bool ignoreSize{false};
            bool hasMenu{false};
            bool invalidated{false};
            DWORD style{WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX |
                        WS_MAXIMIZEBOX};
        };

    private:
        template <typename Event>
        friend void EventSendHelper(WindowHandle_win*, const Event&);

        friend Limits GetWindowLimits(WindowHandle_win*);

    private:
        bool destroyWindow();

    private:
        HWND m_hwnd;
        PAINTSTRUCT m_ps{};
        [[maybe_unused]] HDC m_hdc{};
        Vec2f m_scale{};
        HACCEL m_accelTable{nullptr};
        Data m_data{};
    };
} // namespace pTK::Platform

#endif // PTK_PlATFORM_WIN_WINDOWHANDLE_HPP
