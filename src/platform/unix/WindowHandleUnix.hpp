//
//  platform/unix/WindowHandleUnix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_UNIX_WINDOWHANDLE_HPP
#define PTK_PLATFORM_UNIX_WINDOWHANDLE_HPP

// Local Headers
#include "x11.hpp"

// pTK Headers
#include "ptk/platform/WindowHandle.hpp"

namespace pTK::Platform
{
    /** WindowHandleUnix class implementation.

        This class handles the Unix x11 Window.
    */
    class PTK_API WindowHandleUnix : public WindowHandle
    {
    public:
        /** Constructs WindowHandleUnix with values.

            @param base     valid pointer to window base
            @param name     name of the window
            @param size     size of the window
            @param flags    initial window settings
            @return         initialized WindowHandleUnix with values
        */
        WindowHandleUnix(WindowBase* base, const std::string& name, const Size& size, const WindowInfo& flags);

        /** Destructor for WindowHandleUnix.

        */
        virtual ~WindowHandleUnix();

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

            Sends a WM_PAINT event to the window.
        */
        void invalidate() override;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        void setLimits(const Size& min, const Size& max) override;

        /** Function for retrieving the target refresh rate of the window.

            @return     refresh rate
        */
        [[nodiscard]] std::size_t targetRefreshRate() const noexcept override;

        /** Function for retrieving the XWindow struct of the window.

            @return     XWindow struct
        */
        ::Window xWindow() const;

        /** Function for retrieving the XVisualInfo struct of the window.

            @return     XVisualInfo struct
        */
        XVisualInfo xVisualInfo() const;

        /** Function for retrieving the delelte Atom of the window.

            @return     XWindow struct
        */
        Atom deleteAtom() const;

    private:
        std::pair<unsigned long, unsigned char*> getWindowProperty(Atom property, Atom type) const;

        friend Size& WindowLastSize(WindowHandleUnix*);
        friend Point& WindowLastPos(WindowHandleUnix*);

    private:
        Size m_lastSize;
        Point m_lastPos{};

        Vec2f m_scale{1.0f, 1.0f};

        ::Window m_window;
        Atom m_atomWmDeleteWindow;
        XVisualInfo m_info;
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_UNIX_WINDOWHANDLE_HPP
