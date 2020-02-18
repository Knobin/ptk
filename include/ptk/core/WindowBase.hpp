//
//  core/WindowBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-08.
//

#ifndef PTK_CORE_WINDOWBASE_HPP
#define PTK_CORE_WINDOWBASE_HPP

// Local Headers
#include "ptk/util/Singleton.hpp"
#include "ptk/widgets/VBox.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/SafeQueue.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    class WindowBase : public VBox, public Singleton
    {
    public:
        WindowBase(const std::string& name, const Vec2u& size);

        /** Deconstructor for Canvas.

        */
        virtual ~WindowBase() = default;

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
        const Vec2f& getDPIScale() const;

        /** Function for closing the window.

        */
        virtual void close() = 0;

        /** Function for checking if the window should close.

            @return     close status
        */
        bool shouldClose();

        /** Function for polling the window events.

        */
        virtual void pollEvents() = 0;

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
        virtual void forceDrawAll() = 0;

    protected:
        void setScale(const Vec2f& scale);

    private:
        // Event processing
        void handleEvent(Event *event);
        void handleKeyboardEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);

        void onChildDraw(size_type) override;

    private:
        SafeQueue<std::unique_ptr<Event>> m_eventQueue;
        Vec2f m_scale;
        bool m_draw;
        bool m_close;

    };
}

#endif // PTK_CORE_WINDOWBASE_HPP
