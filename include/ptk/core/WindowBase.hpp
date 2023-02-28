//
//  core/WindowBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

#ifndef PTK_CORE_WINDOWBASE_HPP
#define PTK_CORE_WINDOWBASE_HPP

// pTK Headers
#include "ptk/Core.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/widgets/VBox.hpp"

namespace pTK
{
    namespace Platform
    {
        class WindowHandle;
    }

    /** WindowBase class implementation

        Specifies the backend API that the platform must implement.
    */
    class PTK_API WindowBase : public VBox
    {
    public:
        /** Constructs WindowBase with default values.

            @return    default initialized WindowBase
        */
        WindowBase() = default;

        /** Move Constructor for WindowBase.

            @return    initialized WindowBase from value
        */
        WindowBase(WindowBase&& other) = default;

        /** Move Assignment operator for WindowBase.

            @return    WindowBase with value
        */
        WindowBase& operator=(WindowBase&& other) = default;

        /** Destructor for WindowBase.

        */
        virtual ~WindowBase() = default;

        /** Function for setting the position of the window.

            This will result in Widget::setPosHint not getting called
            and for VBox to not change children positioning.
            VBox (when inherited by Window) will always have pos {0,0}
            in drawing space.

            TODO(knobin): Remove getPosition dependancy in VBox.

            @param pos  position to set
        */
        void setPosHint(const Point& UNUSED(pos)) override {}

    protected:
        // Use this function to send events.
        template <typename Event>
        void handlePlatformEvent(const Event& evt);

        friend class Platform::WindowHandle;

    private:
        // Window specific callbacks.
        // TODO(knobin): Add docs.
        virtual void onCloseEvent() {}
        virtual void onMoveEvent(const Point& UNUSED(pos)) {}
        virtual void onResizeEvent(const Size& UNUSED(size)) {}
        virtual void onFocusEvent() {}
        virtual void onLostFocusEvent() {}
        virtual void onMinimizeEvent() {}
        virtual void onRestoreEvent() {}

    private:
        // Gets called when drawing the window is needed (only from a window backend).
        virtual void regionInvalidated(const PaintEvent&) = 0;

        // Gets called when Window should close.
        virtual void close() = 0;
    };

    // Default event that is not handled by the window.
    template <typename Event>
    void WindowBase::handlePlatformEvent(const Event& evt)
    {
        handleEvent<Event>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<CloseEvent>(const CloseEvent& evt)
    {
        close();
        onCloseEvent();
        handleEvent<CloseEvent>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<ResizeEvent>(const ResizeEvent& evt)
    {
        setSize(evt.size);
        onResizeEvent(evt.size);
        handleEvent<ResizeEvent>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<MoveEvent>(const MoveEvent& evt)
    {
        setPosHint(evt.pos);
        onMoveEvent(evt.pos);
        handleEvent<MoveEvent>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<PaintEvent>(const PaintEvent& evt)
    {
        // This paint event is from the backend and is only called when the platform
        // wants a redraw.
        handleEvent<PaintEvent>(evt);
        regionInvalidated(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<FocusEvent>(const FocusEvent& evt)
    {
        onFocusEvent();
        handleEvent<FocusEvent>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<LostFocusEvent>(const LostFocusEvent& evt)
    {
        onLostFocusEvent();
        handleEvent<LostFocusEvent>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<MinimizeEvent>(const MinimizeEvent& evt)
    {
        onMinimizeEvent();
        handleEvent<MinimizeEvent>(evt);
    }

    template <>
    inline void WindowBase::handlePlatformEvent<RestoreEvent>(const RestoreEvent& evt)
    {
        onRestoreEvent();
        handleEvent<RestoreEvent>(evt);
    }
} // namespace pTK

#endif // PTK_CORE_WINDOWBASE_HPP
