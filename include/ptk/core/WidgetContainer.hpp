//
//  core/WidgetContainer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

#ifndef PTK_CORE_WIDGETCONTAINER_HPP
#define PTK_CORE_WIDGETCONTAINER_HPP

// pTK Headers
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
    class PTK_API WidgetContainer : public Widget
    {
    public:
        using value_type = Ref<Widget>;
        using container_type = std::vector<value_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename container_type::iterator;
        using reverse_iterator = typename container_type::reverse_iterator;
        using const_iterator = typename container_type::const_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs Box with default values.

            @return    default initialized Box
        */
        WidgetContainer();

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

        /** Function for setting the background of the Container.

            @param color   background color
        */
        void setBackground(const Color& color);

        /** Function for getting the background of the Container.

            @return    current background color
        */
        [[nodiscard]] const Color& getBackground() const;

        [[nodiscard]] Widget *getSelectedWidget() const;

        /** Function for retrieving the an iterator that points to the first
            value in the IterableContainer.

            The iterator may be equal to end iterator if the IterableContainer is empty.

            @return    iterator
        */
        iterator begin() noexcept
        {
            return m_holder.begin();
        }

        /** Function for retrieving the an iterator that points to the first
            value in the IterableContainer.

            The iterator may be equal to end iterator if the IterableContainer is empty.

            @return    const iterator
        */
        const_iterator begin() const noexcept
        {
            return m_holder.begin();
        }

        /** Function for retrieving the an const iterator that points to the first
            value in the IterableContainer.

            The iterator may be equal to end iterator if the IterableContainer is empty.

            @return    const iterator
        */
        const_iterator cbegin() const noexcept
        {
            return m_holder.cbegin();
        }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    iterator
        */
        iterator end() noexcept
        {
            return m_holder.end();
        }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        const_iterator end() const noexcept
        {
            return m_holder.end();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be dereferenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        const_iterator cend() const noexcept
        {
            return m_holder.cend();
        }

        /** Function for retrieving the an iterator that points to the last
            value in the IterableContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the IterableContainer is empty.

            @return    reverse iterator
        */
        reverse_iterator rbegin() noexcept
        {
            return m_holder.rbegin();
        }

        /** Function for retrieving the an iterator that points to the last
            value in the IterableContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the IterableContainer is empty.

            @return    const reverse begin iterator
        */
        const_reverse_iterator rbegin() const noexcept
        {
            return m_holder.rbegin();
        }

        /** Function for retrieving the an iterator that points to the last
            value in the IterableContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the IterableContainer is empty.

            @return    const reverse begin iterator
        */
        const_reverse_iterator crbegin() const noexcept
        {
            return m_holder.crbegin();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    reverse reverse end iterator
        */
        reverse_iterator rend() noexcept
        {
            return m_holder.rend();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const reverse end iterator
        */
        const_reverse_iterator rend() const noexcept
        {
            return m_holder.rend();
        }

        /** Function for retrieving the special const iterator referring to
             the past-the-end of the IterableContainer.

             The iterator should never be de-referenced, due to the fact that the iterator
             does not point to a value and should therefore only be used for checking.

             @return    const reverse end iterator
         */
        const_reverse_iterator crend() const noexcept
        {
            return m_holder.crend();
        }

        /** Function for retrieving the element at the index in the IterableContainer.
            @param pos      Index
            @return         element in IterableContainer
        */
        reference at(size_type pos)
        {
            if (pos >= m_holder.size())
                throw std::out_of_range("Index is out of range!");

            return m_holder[pos];
        }

        /** Function for retrieving the element at the index in the IterableContainer.
            @param pos      Index
            @return         element in IterableContainer
        */
        const_reference at(size_type pos) const
        {
            if (pos >= m_holder.size())
                throw std::out_of_range("Position is out of range!");

            return m_holder[pos];
        }

        /** Function for retrieving the first element in the IterableContainer.
            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!
            @return    first element in IterableContainer
        */
        reference front()
        {
            return m_holder.front();
        }

        /** Function for retrieving the first element in the IterableContainer.
            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!
            @return    first element in IterableContainer
        */
        const_reference front() const
        {
            return m_holder.front();
        }

        /** Function for retrieving the last element in the IterableContainer.
            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!
            @return    last element in IterableContainer
        */
        reference back()
        {
            return m_holder.back();
        }

        /** Function for retrieving the last element in the IterableContainer.
            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!
            @return    last element in IterableContainer
        */
        const_reference back() const
        {
            return m_holder.back();
        }

        size_type count() const noexcept
        {
            return m_holder.size();
        }

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
        void initCallbacks();

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

        /** Function for handling when a key is pressed or released.

            @param type     Key event (press or release)
            @param KeyCode  Keycode
        */
        void onKeyCallback(const KeyEvent& evt);

        /** Function for handling key input.

            @param data         array of display characters
            @param size         amount of characters
            @param encoding     encoding of the characer data
        */
        void onInputCallback(const InputEvent& evt);

        /** Function for handling when mouse is hovering.

            @param pos     position
        */
        void onHoverCallback(const MotionEvent& evt);

        /** Function for handling when mouse is entering.

        */
        void onEnterCallback(const EnterEvent& evt);

        /** Function for handling when mouse is leaving.

        */
        void onLeaveCallback(const LeaveEvent& evt);

        /** Function for handling when mouse is scrolling.

            @param offset     x and y offset
        */
        void onScrollCallback(const ScrollEvent& evt);

        /** Function for handling when mouse is clicking.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onClickCallback(const ClickEvent& evt);

        /** Function for handling when mouse is released.

            Derived from EventFunctions.

            @param button      which button on mouse triggered the event.
            @param position    x and y position
        */
        void onReleaseCallback(const ReleaseEvent& evt);

    private:
        // Variables
        container_type m_holder{};
        Color m_background{0xf5f5f5ff};
        Widget* m_lastClickedWidget{nullptr};
        Widget* m_currentHoverWidget{nullptr};
        bool m_busy{false};
    };

} // namespace pTK

#endif // PTK_CORE_WIDGETCONTAINER_HPP
