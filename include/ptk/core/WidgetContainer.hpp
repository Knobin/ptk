//
//  core/WidgetContainer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

#ifndef PTK_CORE_WIDGETCONTAINER_HPP
#define PTK_CORE_WIDGETCONTAINER_HPP

// pTK Headers
#include "ptk/util/IterableContainer.hpp"
#include "ptk/core/Widget.hpp"

// C++ Headers
#include <vector>
#include <utility>

namespace pTK
{
    /** WidgetContainer class implementation.

        Derived from Container, this class for holding Widgets in
        order of displaying them.
    */
    class WidgetContainer : public IterableSequence<Ref<Widget>>, public Widget
    {
    public:
        /** Constructs Box with default values.

            @return    default initialized Box
        */
        WidgetContainer() = default;

        /** Destructor for Box.

        */
        virtual ~WidgetContainer();
        
        // TODO: Documentation.
        void add(const Ref<Widget>& widget);
        
        // TODO: Documentation.
        void remove(const Ref<Widget>& widget);

        /** Function for setting the position of the VBox and its children.

            @param pos     Position to set
        */
        void setPosHint(const Point& pos) override;

        /** Function is called when it is time to draw.

            @param canvas  canvas to draw to
        */
        void onDraw(SkCanvas* canvas) override;

        /** Function for a child Widget to call the Parent when it need to be drawn.

            @param widget  Child Widget
            @return        Status (true if handled)
        */
        bool updateChild(Widget* widget) override;

        /** Function for a child Widget to call the Parent when it need to be drawn.

            @param widget  Child Widget
            @return        Status (true if handled)
        */
        bool drawChild(Widget* widget) override;

        /** Function for handling when a key is pressed or released.

            @param type     Key event (press or release)
            @param KeyCode  Keycode
        */
        bool onKeyEvent(Event::Type type, KeyCode keycode) override;

        /** Function for handling when mouse is hovering.

            @param pos     position
        */
        bool onHoverEvent(const Point& pos) override;

        /** Function for handling when mouse is entering.

        */
        bool onEnterEvent() override;

        /** Function for handling when mouse is leaving.

        */
        bool onLeaveEvent() override;

        /** Function for handling when mouse is scrolling.

            @param offset     x and y offset
        */
        bool onScrollEvent(const Vec2f& offset) override;

        /** Function for handling when mouse is clicking.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        bool onClickEvent(Mouse::Button btn, const Point& pos) override;

        /** Function for handling when mouse is released.

            Derived from EventFunctions.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        bool onReleaseEvent(Mouse::Button btn, const Point& pos) override;

        /** Function for setting the background of the Container.

            @param color   background color
        */
        void setBackground(const Color& color);

        /** Function for getting the background of the Container.

            @return    current background color
        */
        [[nodiscard]] const Color& getBackground() const;

        [[nodiscard]] Widget *getSelectedWidget() const;

    protected:
        /** Function for checking if a child has called a parent function.

            Used to avoid unnecessary recursion.
            If this function will break something in the future (might do), replace it.

            @return     status
        */
        [[nodiscard]] bool busy() const;
        
        // TODO: Add doc.
        void drawBackground(SkCanvas *canvas) const;

    private:
        /** Callback to use when a Widget has been successfully added.

            @param widget   child that has been added
        */
        virtual void onAdd(const Ref<Widget>& UNUSED(widget)) {}

        /** Callback to use when a Widget has been removed.

            Note: widget has already been removed from the container and cannot be accessed with
            any non-modifying functions.

            @param widget   child that has been removed
        */
        virtual void onRemove(const Ref<Widget>& UNUSED(widget)) {}

        /** Callback to use when a child has called the parent update function.

            @param index   the index of the child
        */
        virtual void onChildUpdate(size_type UNUSED(index)) {}

        /** Callback to use when a child has called the parent draw function.

            @param index   the index of the child
        */
        virtual void onChildDraw(size_type UNUSED(index)) {}

    private:
        // Variables
        Color m_background{0xf5f5f5ff};
        Widget* m_lastClickedWidget{nullptr};
        Widget* m_currentHoverWidget{nullptr};
        bool m_busy{false};
    };

} // namespace pTK

#endif // PTK_CORE_WIDGETCONTAINER_HPP
