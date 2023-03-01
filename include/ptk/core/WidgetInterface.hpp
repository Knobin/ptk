//
//  core/WidgetInterface.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2022-10-07.
//

#ifndef PTK_CORE_WIDGETINTERFACE_HPP
#define PTK_CORE_WIDGETINTERFACE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/core/EventCallbacks.hpp"

namespace pTK
{
    /** WidgetInterface class implementation.

        This class is intended for classes that will be inherited
        by widgets and that controls some portion of the widget or
        want a convenient way to access and store callbacks in the
        same location as the widget.

        Classes inheriting from this should call the stored callbacks
        itself (triggerEvent() function) when something it controls
        has changed.

        -   If the Widget needs to re-draw, call the draw() function.
        -   If the Widget do not need to be re-drawn, but some other
            important value has changed (position or size update for
            example) call the update() function.
    */
    class PTK_API WidgetInterface : public virtual EventCallbackInterface
    {
    public:
        /** Constructs WidgetInterface with default values.

            @return    default initialized WidgetInterface
        */
        WidgetInterface() = default;

        /** Destructor for WidgetInterface.

        */
        virtual ~WidgetInterface() = default;

        /** Move Constructor for WidgetInterface.

            @return    initialized WidgetInterface from value
        */
        WidgetInterface(WidgetInterface&&) noexcept {}

        /** Deleted Copy Constructor.

            Copying is prohibited (for now in Widget).
        */
        WidgetInterface(const WidgetInterface&) = delete;

        /** Move Assignment operator for WidgetInterface.

            @return    WidgetInterface with value
        */
        WidgetInterface& operator=(WidgetInterface&&) noexcept { return *this; }

        /** Deleted Copy Assignment operator.

            Copying is prohibited (for now in Widget).
        */
        WidgetInterface& operator=(const WidgetInterface&) = delete;

        /** Function for notifying the parent of a change and
            to put it on an internal render queue.
        */
        virtual bool update() { return false; }

        /** Function for notifying the parent of a change and
            put it on an internal render queue.
        */
        virtual bool draw() { return false; }
    };
} // namespace pTK

#endif // PTK_CORE_WIDGETINTERFACE_HPP
