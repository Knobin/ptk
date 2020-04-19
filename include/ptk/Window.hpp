//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/core/WindowBackend.hpp"
#include "ptk/util/Singleton.hpp"
#include "ptk/widgets/VBox.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/SafeQueue.hpp"

// C++ Headers
#include <memory>
#include <string>

namespace pTK
{
    /** Window class implementation.

        This class is not responsible for handling the Window, it is only supposed to bridge
        the backend and connect the events with the widgets.

        Window creation is handled in the Backend and this class is sort of an API for the
        backend. The class will always have the same size independent of platform.
        This makes the class suitable to derive from, if needed.

        Currently, it will default to a software based backend if no one is specified.
    */
    class Window : public VBox, public Singleton
    {
    public:
        /** Constructs Window with default values.

            @return    default initialized Window
        */
        Window(const std::string& name, const Size& size, BackendType backend = BackendType::SOFTWARE);

        /** Deconstructor for Window.

        */
        virtual ~Window() = default;

        /** Function for retrieving the content size.

            If the monitor supports high DPI, the content of
            the window will size*DPIScale.

            If high DPI is not supported this will return the
            window size.

            @return    content size
        */
        Size getContentSize() const;

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

        /** Function for closing the window.

            This function will set a close flag and return instantly.
            Window will close when the current loop is finished.
        */
        void close();

        /** Function for checking if the window should close.

            @return     close status
        */
        bool shouldClose();

        /** Function for polling the window events.

        */
        void pollEvents();

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
        void postEvent(Event *event);

        /** Function for forcing the window to redraw everything.

        */
        void forceDrawAll();

        /** Function for setting the window position.

            @param pos  requested position of the Window.
        */
        void setPosHint(const Point& pos) override;

        /** Function for retrieving the backend.

            @return    backend
        */
        WindowBackend* getBackend() const;

    private:
        // Event processing
        void handleEvent(Event *event);
        void handleKeyboardEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);

        void onChildDraw(size_type) override;
        void onResize(const Size& size) override;
        void onLimitChange(const Size& min, const Size& max) override;

    private:
        std::unique_ptr<WindowBackend> m_winBackend;
        SafeQueue<std::unique_ptr<Event>> m_eventQueue;
        bool m_draw;
        bool m_close;
    };
}

#endif // PTK_WINDOW_HPP
