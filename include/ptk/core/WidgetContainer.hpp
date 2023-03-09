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
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace pTK
{
    /** WidgetContainer class implementation.

        When inheriting from this class be sure to override (if you are implementing a special container):
            - onAdd
            - onRemove
            - onClear
    */
    class PTK_API WidgetContainer : public Widget
    {
    public:
        using value_type = std::shared_ptr<Widget>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using container_type = std::vector<value_type>;
        using iterator = container_type::iterator;
        using reverse_iterator = container_type::reverse_iterator;
        using const_iterator = container_type::const_iterator;
        using const_reverse_iterator = container_type::const_reverse_iterator;
        using size_type = std::size_t;

    public:
        /** Constructs WidgetContainer with default values.

            @return    default initialized WidgetContainer
        */
        WidgetContainer();

        /** Move Constructor for WidgetContainer.

            @return    initialized WidgetContainer from value
        */
        WidgetContainer(WidgetContainer&& other) = default;

        /** Move Assignment operator for WidgetContainer.

            @return    WidgetContainer with value
        */
        WidgetContainer& operator=(WidgetContainer&& other) = default;

        /** Destructor for WidgetContainer.

        */
        virtual ~WidgetContainer();

        /** Function for adding a Widget to the WidgetContainer.

            Does this in order:
                - Will check if the widget already exists and add it if not.
                - Sets the parent of the widget to this container.
                - calls onAdd
                - queues for drawing

            @param widget   widget to add
        */
        void add(const value_type& widget);

        /** Function for removing a Widget from the WidgetContainer.

             Does this in order:
                - Checks if it exists
                - Sets parent to nullptr.
                - Calls onRemove
                - Removes the widget (if exists).

            @param widget   widget to remove
        */
        void remove(const value_type& widget);

        /** Function for setting the position of the VBox and its children.

            @param pos     Position to set
        */
        void setPosHint(const Point& pos) override;

        /** Function is called when it is time to draw.

            @param canvas   valid Canvas pointer to draw to
        */
        void onDraw(Canvas* canvas) override;

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

        /** Function for retrieving the currently clicked on widget.

            @return    currently selected widget or nullptr
        */
        [[nodiscard]] Widget* getSelectedWidget() const;

        /** Function for retrieving the an iterator that points to the first
            value in the WidgetContainer.

            The iterator may be equal to end iterator if the WidgetContainer is empty.

            @return    iterator
        */
        [[nodiscard]] iterator begin() noexcept { return m_holder.begin(); }

        /** Function for retrieving the an iterator that points to the first
            value in the WidgetContainer.

            The iterator may be equal to end iterator if the WidgetContainer is empty.

            @return    const iterator
        */
        [[nodiscard]] const_iterator begin() const noexcept { return m_holder.begin(); }

        /** Function for retrieving the an const iterator that points to the first
            value in the WidgetContainer.

            The iterator may be equal to end iterator if the WidgetContainer is empty.

            @return    const iterator
        */
        [[nodiscard]] const_iterator cbegin() const noexcept { return m_holder.cbegin(); }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the WidgetContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    iterator
        */
        [[nodiscard]] iterator end() noexcept { return m_holder.end(); }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the WidgetContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        [[nodiscard]] const_iterator end() const noexcept { return m_holder.end(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the WidgetContainer.

            The iterator should never be dereferenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        [[nodiscard]] const_iterator cend() const noexcept { return m_holder.cend(); }

        /** Function for retrieving the an iterator that points to the last
            value in the WidgetContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the WidgetContainer is empty.

            @return    reverse iterator
        */
        [[nodiscard]] reverse_iterator rbegin() noexcept { return m_holder.rbegin(); }

        /** Function for retrieving the an iterator that points to the last
            value in the WidgetContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the WidgetContainer is empty.

            @return    const reverse begin iterator
        */
        [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return m_holder.rbegin(); }

        /** Function for retrieving the an iterator that points to the last
            value in the WidgetContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the WidgetContainer is empty.

            @return    const reverse begin iterator
        */
        [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return m_holder.crbegin(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the WidgetContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    reverse reverse end iterator
        */
        [[nodiscard]] reverse_iterator rend() noexcept { return m_holder.rend(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the WidgetContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const reverse end iterator
        */
        [[nodiscard]] const_reverse_iterator rend() const noexcept { return m_holder.rend(); }

        /** Function for retrieving the special const iterator referring to
             the past-the-end of the WidgetContainer.

             The iterator should never be de-referenced, due to the fact that the iterator
             does not point to a value and should therefore only be used for checking.

             @return    const reverse end iterator
         */
        [[nodiscard]] const_reverse_iterator crend() const noexcept { return m_holder.crend(); }

        /** Function for retrieving the element at the index in the WidgetContainer.
            @param pos      Index
            @return         element in WidgetContainer
        */
        [[nodiscard]] reference at(size_type pos)
        {
            if (pos >= m_holder.size())
                throw std::out_of_range("Index is out of range!");

            return m_holder[pos];
        }

        /** Function for retrieving the element at the index in the WidgetContainer.
            @param pos      Index
            @return         element in WidgetContainer
        */
        [[nodiscard]] const_reference at(size_type pos) const
        {
            if (pos >= m_holder.size())
                throw std::out_of_range("Position is out of range!");

            return m_holder[pos];
        }

        /** Function for retrieving the first element in the WidgetContainer.

            Should in no circumstances be called when the WidgetContainer is empty!
            This is considered to be undefined behavior!

            @return    first element in WidgetContainer
        */
        [[nodiscard]] reference front() noexcept { return m_holder.front(); }

        /** Function for retrieving the first element in the WidgetContainer.

            Should in no circumstances be called when the WidgetContainer is empty!
            This is considered to be undefined behavior!

            @return    first element in WidgetContainer
        */
        [[nodiscard]] const_reference front() const noexcept { return m_holder.front(); }

        /** Function for retrieving the last element in the WidgetContainer.

            Should in no circumstances be called when the WidgetContainer is empty!
            This is considered to be undefined behavior!

            @return    last element in WidgetContainer
        */
        [[nodiscard]] reference back() noexcept { return m_holder.back(); }

        /** Function for retrieving the last element in the WidgetContainer.

            Should in no circumstances be called when the WidgetContainer is empty!
            This is considered to be undefined behavior!

            @return    last element in WidgetContainer
        */
        [[nodiscard]] const_reference back() const noexcept { return m_holder.back(); }

        /** Function for retrieving the current amount of items in the WidgetContainer.

            @return     number of items in container
        */
        [[nodiscard]] size_type count() const noexcept { return m_holder.size(); }

        /** Function for removing all items in the WidgetContainer.

            Note: Does not call onRemove for all items.
        */
        void clear();

    protected:
        /** Function for checking if a child has called a parent function.

            Used to avoid unnecessary recursion.
            If this function will break something in the future (might do), replace it.

            @return     status
        */
        [[nodiscard]] bool busy() const;

        /** Function that paints the background.

            @param canvas   valid Canvas pointer to draw to
        */
        void drawBackground(Canvas* canvas) const;

        /** Function that paints all the children in the WidgetContainer.

            @param canvas   valid Canvas pointer to draw to
        */
        void drawChildren(Canvas* canvas)
        {
            for (auto it = m_holder.begin(); it != m_holder.end(); ++it)
                (*it)->onDraw(canvas);
        }

    private:
        void initCallbacks();

        /** Callback to use when a Widget has been successfully added.

            @param widget   child that has been added
        */
        virtual void onAdd(const value_type& UNUSED(widget)) {}

        /** Callback to use when a Widget has been removed.

            Note: widget has already been removed from the container and cannot be accessed with
            any non-modifying functions.

            @param widget   child that has been removed
        */
        virtual void onRemove(const value_type& UNUSED(widget)) {}

        /** Callback to use when the WidgetContainer is cleared.

            Note: This callback is called before the container is cleared.
        */
        virtual void onClear() {}

        /** Callback to use when a child has called the parent update function.

            @param index   the index of the child
        */
        virtual void onChildUpdate(size_type UNUSED(index)) {}

        /** Callback to use when a child has called the parent draw function.

            @param index   the index of the child
        */
        virtual void onChildDraw(size_type UNUSED(index)) {}

        /** Function for retrieving the child at a specific position.

            @param pos      position to search
            @return         iterator to child or cend()
        */
        [[nodiscard]] virtual const_iterator findChildAtPos(const Point& pos) const;

    private:
        /** Function for handling when a key is pressed or released.

            @param type     Key event (press or release)
            @param KeyCode  Keycode
        */
        void onKeyCallback(const KeyEvent& evt);

        /** Function for handling key input.

            @param data         array of display characters
            @param size         amount of characters
            @param encoding     encoding of the character data
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
        /** ContainerEntryPair struct implementation.

            Used to find and validate entries in the container.
            Helpful to get the shared_ptr to a widget from a raw pointer.
        */
        struct PTK_API ContainerEntryPair
        {
            Widget* ptr{nullptr};
            std::size_t index{0};
        };

        /** Function for checking if the ContainerEntryPair is a valid entry in the container.

            @param val      value to test
            @return         status
        */
        [[nodiscard]] bool validEntryPair(ContainerEntryPair val) const noexcept
        {
            if (val.index < m_holder.size())
                return (m_holder[val.index].get() == val.ptr);
            return false;
        }

    private:
        // Variables
        container_type m_holder{};
        ContainerEntryPair m_lastClickedWidget{};
        ContainerEntryPair m_currentHoverWidget{};
        Color m_background{0xf5f5f5ff};
        bool m_busy{false};
    };

} // namespace pTK

#endif // PTK_CORE_WIDGETCONTAINER_HPP
