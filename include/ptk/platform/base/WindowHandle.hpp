//
//  platform/base/WindowHandle.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_BASE_WINDOWHANDLE_HPP
#define PTK_PLATFORM_BASE_WINDOWHANDLE_HPP

// pTK Headers
#include "ptk/core/WindowBase.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <memory>

namespace pTK::Platform
{
    /** WindowHandle class implementation

        Specifies the backend API that the platform must implement.
    */
    class PTK_API WindowHandle : public SingleObject
    {
    public:
        /** Function for creating the platform specific WindowHandle.

            @return pointer to platform specific WindowHandle
        */
        static std::unique_ptr<WindowHandle> Make(WindowBase* winBase, const std::string& name, const Size& size,
                                                  const WindowInfo& flags);

    public:
        /** Constructs WindowHandle with default values.

            @return    default initialized WindowHandle
        */
        WindowHandle() = delete;

        /** Constructs WindowHandle with WindowBase.

            @param app      pointer to WindowHandle
            @return         default initialized WindowHandle
        */
        explicit WindowHandle(WindowBase* win)
            : SingleObject(),
              m_winBase{win}
        {}

        /** Destructor for WindowHandle.

        */
        virtual ~WindowHandle() = default;

    public:
        /** Function for showing the window.

        */
        virtual void show() = 0;

        /** Function for closing the window.

            @return     true if operation is successful, otherwise false
        */
        virtual bool close() = 0;

        /** Function for hiding the window.

        */
        virtual void hide() = 0;

        /** Function for checking if the window is visible.

            @return    status
        */
        [[nodiscard]] bool visible() const { return !isHidden(); }

        /** Function for retrieving if the window is hidden.

            @return     true if window is hidden, otherwise false
        */
        [[nodiscard]] virtual bool isHidden() const = 0;

        /** Function for setting the position of the window.

            @param pos  position to set
            @return     true if operation is successful, otherwise false
        */
        virtual bool setPosHint(const Point& UNUSED(pos)) = 0;

        /** Function for resizing the window.

            @param size  size to set
            @return     true if operation is successful, otherwise false
        */
        virtual bool resize(const Size& UNUSED(size)) = 0;

        /** Function for setting the title of the window.

            @param name     title to show
            @return         true if operation is successful, otherwise false
        */
        virtual bool setTitle(const std::string& UNUSED(name)) = 0;

        /** Function for setting the icon of the window.

            Note: The function expects the pixels to be in a RGBA format with size of 32 bits (4 bytes).
            Therefore, size of the pixel array is width * height * 32.
            This is why a size parameter is not needed.

            @param width    width of the image
            @param height   height of the image
            @param pixels   image pixels in a RGBA format.
            @return         true if operation is successful, otherwise false
        */
        virtual bool setIcon(int32_t UNUSED(width), int32_t UNUSED(height), uint8_t* UNUSED(pixels)) = 0;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        virtual void notifyEvent() {}

        /** Function for retrieving the window position.

            @return     Window Position
        */
        [[nodiscard]] virtual Point getPosition() const = 0;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        [[nodiscard]] virtual Size getSize() const = 0;

        /** Function for minimizing the window.

            @return     true if operation is successful, otherwise false
        */
        virtual bool minimize() = 0;

        /** Function for retrieving the minimizing status of the window.

            @return     true if window is minimized, otherwise false
        */
        [[nodiscard]] virtual bool isMinimized() const = 0;

        /** Function for restoring a window from the minimized state.

            @return     true if operation is successful, otherwise false
        */
        virtual bool restore() = 0;

        /** Function for retrieving the focus status of the window.

            @return     true if window is focused, otherwise false
        */
        [[nodiscard]] virtual bool isFocused() const = 0;

        /** Function for hinting the scale of the window.

            @param scale    scale hint
            @return         true if operation is successful, otherwise false
        */
        virtual bool setScaleHint(const Vec2f& UNUSED(scale)) { return true; }

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        virtual void setLimits(const Size& UNUSED(min), const Size& UNUSED(max)) {}

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        [[nodiscard]] virtual Vec2f getDPIScale() const { return {1.0f, 1.0f}; }

        /** Function for invalidating the window.

            When this is called the window needs to be redrawn.
            This is accomplished by sending a PaintEvent to the window.
            The event does not have to be sent in this function, but it
            should result in that event being sent as soon as possible.
        */
        virtual void invalidate() {}

        /** Function for retrieving the target refresh rate of the window.

            @return     refresh rate
        */
        virtual std::size_t targetRefreshRate() { return 60; }

        /** Function for retrieving the window.

            @return pointer to WindowBase
        */
        [[nodiscard]] WindowBase* window() const { return m_winBase; }

        // Forwarding of events from platform handler to window.
        template <typename Event>
        void HandlePlatformEvent(const Event& evt)
        {
            m_winBase->HandlePlatformEvent<Event>(evt);
        }

    private:
        WindowBase* m_winBase{nullptr};
    };

} // namespace pTK

#endif // PTK_PLATFORM_BASE_WINDOWHANDLE_HPP
