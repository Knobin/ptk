//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// pTK Headers
#include "ptk/core/ContextBase.hpp"
#include "ptk/core/EventQueue.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/core/WindowBase.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/platform/base/WindowHandle.hpp"
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

namespace pTK
{
    /** Window class implementation.

        Window creation is handled in the Backend and this class is sort of an API for the
        backend.

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

            @return     close status
        */
        bool shouldClose() const;

        /** Function for handling the window events.

        */
        void handleEvents();

        /** Function for sending events to the window to
            be handled directly.

            Returns after the event is handled.
        */
        void sendEvent(Event* event);

        /** Function for sending events to the window to
            be handled later when handleEvents() is called.

            Event gets put on a queue (FIFO) and returns directly.
            Note: event must be allocated on the HEAP, as soon as the
            event is handled it will be deleted.

            This function is thread safe.
        */
        template <typename T, typename... Args>
        void postEvent(Args&&... args);

        /** Function for setting the window icon.

            @param path     path to image
        */
        bool setIconFromFile(const std::string& path);
        
        // TODO(knobin): Add docs.
        void invalidate()
        {
            // Setting m_draw to true will result in a call to paint()
            // after all events have been handled in handleEvents().
            m_draw = true;
        }

        /** Function for showing the window.

        */
        void show() override;

        /** Function for closing the window.

            @return     true if operation is successful, otherwise false
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

        /** Callback for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        void onLimitChange(const Size&, const Size&) final { setLimitsWithSizePolicy(); }

        /** Function for setting the SizePolicy of the Widget.

            @param  policy   policy to set
        */
        void setSizePolicy(SizePolicy policy) final
        {
            Widget::setSizePolicy(policy);
            setLimitsWithSizePolicy();
        }

        // Sets the new window limits based on the SizePolicy.
        void setLimitsWithSizePolicy()
        {
            Limits limits{getLimitsWithSizePolicy()};
            m_handle->setLimits(limits.min, limits.max);
        }

        // TODO(knobin): Add documentation.
        Platform::WindowHandle* handle() const { return m_handle.get(); }

        /** Function for setting the position of the window.

            @param pos  position to set
        */
        void setPosHint(const Point& pos) override { m_handle->setPosHint(pos); }

        /** Function for retrieving the closed status of the window.

            @return     true if closed is focused, otherwise false
        */
        [[nodiscard]] bool isClosed() const noexcept { return m_closed; }

    private:
        // Event processing (for pointer based event).
        void handleEvent(Event* event);
        void handleKeyboardEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);

        void onChildDraw(size_type) override;
        void onSizeChange(const Size& size) override;
        // void onLimitChange(const Size& min, const Size& max) override;

        // getPosition should not be used outside this class.
        using VBox::getPosition;

    private:
        
        // TODO(knobin): Add docs.
        void paint();

        // This draw function gets called from the backend.
        void regionInvalidated(const PaintEvent&) override
        {
            // Just assume that the entire window needs to be painted here (for now).
            // Another (better) solution would be to find what children needs to be
            // painted and just paint those. But currently there isn't any support
            // for rendering individual widgets anyway.
            paint();
            m_draw = false;
        }

    private:
        EventQueue<std::deque> m_eventQueue{};
        std::unique_ptr<Platform::WindowHandle> m_handle;
        std::unique_ptr<ContextBase> m_context;
        std::thread::id m_threadID;
        bool m_draw{false};
        bool m_close{false};
        bool m_closed{false};
    };

    template <typename T, typename... Args>
    void Window::postEvent(Args&&... args)
    {
        m_eventQueue.lock();
        if (m_eventQueue.push<T>(std::forward<Args>(args)...))
            if (std::this_thread::get_id() != m_threadID)
                m_handle->notifyEvent();
        m_eventQueue.unlock();
    }

} // namespace pTK

#endif // PTK_WINDOW_HPP
