//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// pTK Headers
#include "ptk/core/Event.hpp"
#include "ptk/core/EventQueue.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/platform/Platform.hpp"
#include "ptk/util/SingleObject.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/widgets/VBox.hpp"

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
    class PTK_API Window : public PTK_WINDOW_HANDLE_T, public SingleObject
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
        void handleEvents() override;

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
            invalidate();
        }

    private:
        EventQueue<std::deque> m_eventQueue{};
        // PTK_WINDOW_HANDLE_T m_handle;
        std::thread::id m_threadID;
        bool m_draw{false};
        bool m_close{false};
    };

    template <typename T, typename... Args>
    void Window::postEvent(Args&&... args)
    {
        m_eventQueue.lock();
        if (m_eventQueue.push<T>(std::forward<Args>(args)...))
        {
            if (std::this_thread::get_id() != m_threadID)
            {
                // m_handle.notifyEvent();
                notifyEvent();
            }
        }
        m_eventQueue.unlock();
    }

} // namespace pTK

#endif // PTK_WINDOW_HPP
