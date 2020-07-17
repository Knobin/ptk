//
//  core/Box.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

#ifndef PTK_CORE_BOX_HPP
#define PTK_CORE_BOX_HPP

// Local Headers
#include "ptk/core/Container.hpp"
#include "ptk/core/Widget.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** Box class implementation.

        Derived from Container, this class for holding Widgets in
        order of displaying them in a vertical style.

        Be sure to override these functions:
            - virtual void setPosHint(const Point& pos);
            - virtual void setSize(const Size& newSize);
            - virtual void drawContent(SkCanvas *canvas);
    */
    class Box : public Container<Ref<Widget>>, public Widget
    {
    public:
        /** Constructs Box with default values.

            @return    default initialized Box
        */
        Box();

        /** Deconstructor for Box.

        */
        virtual ~Box();

        /** Function for adding a Widget

            @param widget  Widget to add
            @return        Status (true if added)
        */
        bool add(const Ref<Widget>& widget) override;

        /** Function for removing a Widget.

            @param widget  Widget to remove
        */
        void remove(const Ref<Widget>& widget) override;

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
        const Color& getBackground() const;

    protected:
        /** Function for checking if a child has called a parent function.

            Used to avoid unnecessary recursion.
            If this function will break something in the future (might do), replace it.

            @return     status
        */
        bool busy() const;

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
        Color m_background;
        Widget* m_lastClickedWidget;
        Widget* m_currentHoverWidget;
        bool m_busy;
    };
} // namespace pTK

#endif // PTK_CORE_BOX_HPP
