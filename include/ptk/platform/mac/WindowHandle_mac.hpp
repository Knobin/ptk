//
//  platform/mac/MainWindow_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-20.
//

#ifndef PTK_PlATFORM_MAC_WINDOWHANDLE_HPP
#define PTK_PlATFORM_MAC_WINDOWHANDLE_HPP

// pTK Headers
#include "ptk/platform/base/WindowHandle.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/events/MouseEvent.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    /** WindowHandle_mac class implementation.

        This class handles the macOS Cocoa Window.
    */
    class WindowHandle_mac : public WindowHandle
    {
    public:
        /** Constructs WindowHandle_win with default values.

            @param window   pointer to parent Window
            @param name     name of the window
            @param size     size of the window
            @param flags    setup flags for the window
            @return         default initialized MainWindow_mac
        */
        WindowHandle_mac(const std::string& name, const Size& size, const WindowInfo& flags);

        /** Destructor for MainWindow_win.

        */
        virtual ~WindowHandle_mac();

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
        void setPosHint(const Point& pos) override;

        /** Function for retrieving the Context.

            @return context
        */
        [[nodiscard]] ContextBase *getContext() const override;

        /** Function for swapping the buffers.

        */
        void swapBuffers() override;

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
        bool setIcon(int32 width, int32 height, byte* pixels) override;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        void notifyEvent() override;

        /** Function for retrieving the window position.

           @return     Window Position
       */
        [[nodiscard]] Point getWinPos() const override;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        [[nodiscard]] Size getWinSize() const override;

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

        /** Function for retrieving the NSWindow id.

            @return     id of the NSWindow
        */
        [[nodiscard]] long windowID() const;

        /** Function for retrieving the NSWindow pointer.

            Be sure to cast this to a NSWindow*.

            @return     pointer to NSWindow
        */
        [[nodiscard]] void *handle() const;

    private:
        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
            @return     true if operation is successful, otherwise false
        */
        void onLimitChange(const Size& min, const Size& max) override;

    private:
        void init(const std::string& name, const Size& size, const WindowInfo& flags);

        template<typename Event>
        friend void EventSendHelper(WindowHandle_mac*, const Event&);

    private:
        std::unique_ptr<ContextBase> m_context{nullptr};
        Vec2f m_scale;
        void *m_NSWindow{nullptr};
        long m_id{};
    };
}

#endif // PTK_PlATFORM_MAC_WINDOWHANDLE_HPP
