//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// pTK Headers
#include "ptk/core/CommandBuffer.hpp"
#include "ptk/core/ContextBase.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/core/WindowBase.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/platform/WindowHandle.hpp"
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>

namespace pTK
{
    /** Window class implementation.

        Window creation is handled in a platform specific handler.

        Currently, it will default to a hardware based backend if no one is specified.
        If that backend is not available, it will fall back to a software based backend.
    */
    class PTK_API Window : public WindowBase, public SingleObject
    {
    public:
        /** Constructs Window with default values.

            @return    default initialized Window
        */
        Window(const std::string& name, const Size& size, const WindowInfo& flags = {});

        /** Destructor for Window.

        */
        virtual ~Window();

        /** Function for checking if the window should close.

            @return     closing status
        */
        bool shouldClose() const;

        /** Function for executing the commands sent to the window.

        */
        void runCommands();

        /** Function for sending commands to the window.

            Commands will be put on a queue (FIFO) and the function returns instantly.
            Note: This function is thread safe.
        */
        template <typename Command>
        void postCommand(Command cmd);

        /** Function for invalidating the window and it needs to be redrawn.

            May or may not result in directly drawing before returning, otherwise
            a draw event will be queued and drawing will happen a bit later.
        */
        void invalidate();

        /** Function for checking if the content is valid.

            @return     status
        */
        [[nodiscard]] bool isContentValid() const noexcept { return !m_contentInvalidated; }

        /** Function for drawing the content, if the content has been invalidated.

            @return     status
        */
        bool drawContent();

        /** Function for retrieving the target refresh rate of the window.

            @return     refresh rate
        */
        [[nodiscard]] std::size_t targetRefreshRate() const { return m_handle->targetRefreshRate(); }

        /** Function for retrieving the time past since last draw.

            @return     milliseconds since last draw
        */
        std::size_t timeSinceLastDraw() const;

        /** Function for showing the window.

        */
        void show() override;

        /** Function for closing the window.

        */
        void close() override;

        /** Function for hiding the window.

        */
        void hide() override;

        /** Function for checking if the window is visible.

            @return    status
        */
        [[nodiscard]] bool visible() const override { return m_handle->visible(); }

        /** Function for retrieving if the window is hidden.

            @return     true if window is hidden, otherwise false
        */
        [[nodiscard]] bool isHidden() const { return m_handle->isHidden(); }

        /** Function for setting the title of the window.

            @param name     title to show
            @return         true if operation is successful, otherwise false
        */
        bool setTitle(const std::string& name) { return m_handle->setTitle(name); }

        /** Function for setting the icon of the window.

            Note: The function expects the pixels to be in a RGBA format with size of 32 bits (4 bytes).
            Therefore, size of the pixel array is width * height * 32.
            This is why a size parameter is not needed.

            @param width    width of the image
            @param height   height of the image
            @param pixels   image pixels in a RGBA format.
            @return         true if operation is successful, otherwise false
        */
        bool setIcon(int32_t width, int32_t height, uint8_t* pixels)
        {
            return m_handle->setIcon(width, height, pixels);
        }

        /** Function for setting the window icon.

            @param path     path to image
        */
        bool setIconFromFile(const std::string& path);

        /** Function for minimizing the window.

            @return     true if operation is successful, otherwise false
        */
        bool minimize();

        /** Function for retrieving the minimizing status of the window.

            @return     true if window is minimized, otherwise false
        */
        [[nodiscard]] bool isMinimized() const { return m_handle->isMinimized(); }

        /** Function for restoring a window from the minimized state.

            @return     true if operation is successful, otherwise false
        */
        bool restore();

        /** Function for retrieving the focus status of the window.

            @return     true if window is focused, otherwise false
        */
        [[nodiscard]] bool isFocused() const { return m_handle->isFocused(); }

        /** Function for retrieving the Context of the Backend.

            @return     Context
        */
        [[nodiscard]] ContextBase* getContext() const { return m_context.get(); }

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        [[nodiscard]] Vec2f getDPIScale() const { return m_handle->getDPIScale(); }

        /** Function for setting the SizePolicy of the Widget.

            @param  policy   policy to set
        */
        void setSizePolicy(SizePolicy policy) override;

        /** Function for retrieving the platform specific handle to the window.

            @return     window handle
        */
        [[nodiscard]] Platform::WindowHandle* platformHandle() const { return m_handle.get(); }

        /** Function for setting the position of the window.

            @param pos  position to set
        */
        void setPosHint(const Point& pos) override;

        /** Function for retrieving the closed status of the window.

            Note: A closed window cannot be opened again.

            @return     true if closed, otherwise false
        */
        [[nodiscard]] bool isClosed() const noexcept { return m_closed; }

    private:
        void onAdd(const value_type&) override;
        void onRemove(const value_type&) override;
        void onChildDraw(size_type) override;
        void onChildUpdate(size_type) override;
        void onSizeChange(const Size& size) override;
        void onLayoutChange() override;

    private:
        // This draw function gets called from the backend.
        void paint();

        // Callback for when content needs to be redrawn. Called from base class.
        void regionInvalidated(const PaintEvent&) override;

        // Helper for setting the content as valid (does not need to be redrawn).
        void markContentValid();

        /** Callback for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        void onLimitChange(const Size& UNUSED(min), const Size& UNUSED(max)) override;

        // Helper for setting the new window limits based on the set SizePolicy.
        void setLimitsWithSizePolicy();

    private:
        mutable std::mutex m_commandBufferMutex{};
        CommandBuffer<void()> m_commandBuffer{};
        std::unique_ptr<Platform::WindowHandle> m_handle;
        std::unique_ptr<ContextBase> m_context;
        std::chrono::time_point<std::chrono::steady_clock> m_lastDrawTime;
        std::thread::id m_threadID;
        std::atomic<bool> m_contentInvalidated{false};
        bool m_close{false};
        bool m_closed{false};
    };

    template <typename Command>
    void Window::postCommand(Command cmd)
    {
        {
            std::lock_guard<std::mutex> lock{m_commandBufferMutex};
            m_commandBuffer.add(std::move(cmd));
        }

        if (std::this_thread::get_id() != m_threadID)
            m_handle->notifyEvent();
    }

} // namespace pTK

#endif // PTK_WINDOW_HPP
