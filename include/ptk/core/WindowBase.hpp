//
//  core/WindowBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

#ifndef PTK_CORE_WINDOWBASE_HPP
#define PTK_CORE_WINDOWBASE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
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
        ~WindowBase() override = default;

        /** Function for adding a callback that is invoked when window is closed.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onClose(const std::function<bool()>& callback) { return addListener<CloseEvent>(callback); }

        /** Function for adding a callback that is invoked when window is moved.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onMove(const std::function<bool(const MoveEvent&)>& callback)
        {
            return addListener<MoveEvent>(callback);
        }

        /** Function for adding a callback that is invoked when window is resized.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onResize(const std::function<bool(const ResizeEvent&)>& callback)
        {
            return addListener<ResizeEvent>(callback);
        }

        /** Function for adding a callback that is invoked when window is focused.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onFocus(const std::function<bool(const FocusEvent&)>& callback)
        {
            return addListener<FocusEvent>(callback);
        }

        /** Function for adding a callback that is invoked when window is not focused.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onLostFocus(const std::function<bool(const LostFocusEvent&)>& callback)
        {
            return addListener<LostFocusEvent>(callback);
        }

        /** Function for adding a callback that is invoked when window is minimized.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onMinimize(const std::function<bool()>& callback) { return addListener<MinimizeEvent>(callback); }

        /** Function for adding a callback that is invoked when window is restored.

            @param callback    function to call
            @return            callback id
        */
        uint64_t onRestore(const std::function<bool()>& callback) { return addListener<RestoreEvent>(callback); }

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
        updatePosition(evt.pos);
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
