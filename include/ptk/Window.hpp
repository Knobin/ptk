//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// pTK Headers
#include "ptk/core/platform/MainWindowBase.hpp"
#include "ptk/util/SingleObject.hpp"
#include "ptk/widgets/VBox.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/core/EventQueue.hpp"
#include "ptk/core/WindowInfo.hpp"

// C++ Headers
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

namespace pTK
{
    /** Window class implementation.

        This class is not responsible for handling the Window, it is only supposed to bridge
        the backend and connect the events with the widgets.

        Window creation is handled in the Backend and this class is sort of an API for the
        backend. The class will always have the same size independent of platform.
        This makes the class suitable to derive from, if needed.

        Currently, it will default to a hardware based backend if no one is specified.
        If that backend is not available, it will fall back to a software based backend.
    */
    class Window : public VBox, public SingleObject
    {
    public:
        /** Constructs Window with default values.

            @return    default initialized Window
        */
        Window(const std::string& name, const Size& size, const WindowInfo& flags = {});

        /** Deconstructor for Window.

        */
        virtual ~Window();

        /** Function for retrieving the DPI Scale of the window.

            @return    DPI scale
        */
        Vec2f getDPIScale() const;

        /** Function to show the Window.

        */
        void show() override;

        /** Function to hide the Window.

        */
        void hide() override;

        /** Function for retrieving if the window is hidden.

        */
        bool isHidden() const;

        /** Function for closing the window.

            This function will set a close flag and return instantly.
            Window will close when the current loop is finished.
        */
        void close();

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
        void sendEvent(Event *event);

        /** Function for sending events to the window to
            be handled later when handleEvents() is called.

            Event gets put on a queue (FIFO) and returns directly.
            Note: event must be allocated on the HEAP, as soon as the
            event is handled it will be deleted.

            This function is thread safe.
        */
        template<typename T, typename... Args>
        void postEvent(Args&& ...args);

        /** Function for forcing the window to redraw everything.

        */
        void forceDrawAll();

        /** Function for setting the window position.

            @param pos  requested position of the Window.
        */
        void setPosHint(const Point& pos) override;

        /** Function for retrieving the window position.

            Note: getPosition should not be used due to being internally used by either children or VBox.
            getPosition always return {0, 0}.
            Use this function for retrieving the Window Coordinates.

            @return window position
        */
        Point getWinPos() const;

        /** Function for retrieving the window size.

            Note: getSize returns the content size of the window.
            If the DPI is {1.0f, 1.0f} this function will return the same value as getSize.
            Use this function for retrieving the window size.

            @return window size
        */
        Size getWinSize() const;

        /** Function for setting the window title.

            @param name     title to set
        */
        void setTitle(const std::string& name);

        /** Function for setting the window icon.

            @param path     path to image
        */
        bool setIcon(const std::string& path);

        /** Function for retrieving the backend.

            @return    backend
        */
        MainWindowBase* getBackend() const;

        /** Function for minimizing the window.

        */
        void minimize();

        /** Function for restoring a window from the minimized state.

        */
        void restore();

        /** Function for retrieving the minimizing status of the window.

            @return     status
        */
        bool isMinimized() const;

        /** Function for retrieving the focus status of the window.

            @return     status
        */
        bool isFocused() const;

    public:
        // Window specific callbacks.
        void onClose(const std::function<bool()>& callback);
        void onMove(const std::function<bool(const Point& pos)>& callback);
        void onResize(const std::function<bool(const Size& pos)>& callback);
        void onFocus(const std::function<bool()>& callback);
        void onLostFocus(const std::function<bool()>& callback);
        void onMinimize(const std::function<bool()>& callback);
        void onRestore(const std::function<bool()>& callback);

    private:
        // Event processing
        void handleEvent(Event *event);
        void handleKeyboardEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);

        void onChildDraw(size_type) override;
        void onSizeChange(const Size& size) override;
        void onLimitChange(const Size& min, const Size& max) override;

        // getPosition should not be used outside this class.
        using VBox::getPosition;

    private:
        // Window specific callbacks.
        std::function<bool()> m_onClose{nullptr};
        std::function<bool(const Point& pos)> m_onMove{nullptr};
        std::function<bool(const Size& pos)> m_onResize{nullptr};
        std::function<bool()> m_onFocus{nullptr};
        std::function<bool()> m_onLostFocus{nullptr};
        std::function<bool()> m_onMinimize{nullptr};
        std::function<bool()> m_onRestore{nullptr};

    private:
        EventQueue<std::deque> m_eventQueue{};
        std::unique_ptr<MainWindowBase> m_winBackend{nullptr};
        std::thread::id m_threadID;
        bool m_draw{false};
        bool m_close{false};
        bool m_minimized{false};
        WindowInfo m_info;

    };

    template<typename T, typename... Args>
    void Window::postEvent(Args&& ...args)
    {
        m_eventQueue.lock();
        if (m_eventQueue.push<T>(std::forward<Args>(args)...))
        {
            if (std::this_thread::get_id() != m_threadID)
            {
                m_winBackend->notifyEvent();
            }
        }
        m_eventQueue.unlock();
    }

}

#endif // PTK_WINDOW_HPP
