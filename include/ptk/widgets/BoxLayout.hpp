//
//  widgets/BoxLayout.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-03-25.
//

#ifndef PTK_WIDGETS_BOXLAYOUT_HPP
#define PTK_WIDGETS_BOXLAYOUT_HPP

// pTK Headers
#include "ptk/core/WidgetContainer.hpp"

namespace pTK
{
    /** BoxLayout class implementation.

        * Container for Widgets that implements ordering based on a direction.
            - LeftToRight
            - RightToLeft
            - TopToBottom
            - BottomToTop

        * Widgets can only be ordered in either a column or row as in HBox or VBox.
        * No grids or other types of ordering.

        * All Widgets must fit inside the container.
    */
    class PTK_API BoxLayout : public WidgetContainer
    {
    public:
        /** Direction enum class.

            Contains possible ways for ordering Widgets in BoxLayout.
        */
        enum class Direction
        {
            LeftToRight,
            RightToLeft,
            TopToBottom,
            BottomToTop
        };

    public:
        /** Deleted constructor.

            BoxLayout needs a layout direction.
        */
        BoxLayout() = delete;

        /** Constructs BoxLayout with direction.

            @param direction    ordering of widgets
            @return             initialized BoxLayout
        */
        explicit BoxLayout(Direction direction);

        /** Destructor for BoxLayout.

        */
        ~BoxLayout() override = default;

        /** Move Constructor for BoxLayout.

            @return    initialized BoxLayout from value
        */
        BoxLayout(BoxLayout&& other) = default;

        /** Move Assignment operator for BoxLayout.

            @return    BoxLayout with value
        */
        BoxLayout& operator=(BoxLayout&& other) = default;

        /** Deleted Copy Constructor.

            Copying is prohibited (for now), use clone() instead.
        */
        BoxLayout(const BoxLayout&) = delete;

        /** Deleted Copy Assignment operator.

            Copying is prohibited (for now), use clone() instead.
        */
        BoxLayout& operator=(const BoxLayout&) = delete;

        /** Function for retrieving the ordering direction of the BoxLayout.

            @return  ordering direction
        */
        [[nodiscard]] Direction direction() const noexcept { return m_direction; }

        /** Function for setting the ordering direction of the BoxLayout.

            @param direction    ordering direction
        */
        void setDirection(Direction direction);

        /** Draw function.

            Function is called when it is time to draw.

            @param canvas   valid Canvas pointer to draw to
        */
        void onDraw(Canvas* canvas) override;

    protected:
        /** Function for ordering the children inside size at pos.

            @param size     content size
            @param pos      content position
        */
        virtual void refitContent(Size size, Point pos);

        /** Function for ordering the children inside size at pos with minimal size.

            Children will just barely fit with minimal sizes.

            @param size     content size
            @param pos      content position
        */
        virtual void expandOnAdd(Size size, Point pos);

        /** Function for checking if the direction is allowed.

            @param direction    new direction
            @return             allowed status
        */
        [[nodiscard]] virtual bool onLayoutRequest(Direction direction);

        // Ordering changed callback.
        virtual void onLayoutChange();

        /** Function for retrieving the minimal size based on all children.

            @return     minimal bounds
        */
        [[nodiscard]] virtual Size calcMinSize() const;

        /** Function for retrieving the maximum size based on all children.

            @return     maximum bounds
        */
        [[nodiscard]] virtual Size calcMaxSize() const;

        /** Function that paints all the children in the BoxLayout with respect to set direction.

            @param canvas   valid Canvas pointer to draw to
        */
        void drawChildrenWithDir(Canvas* canvas);

        /** Function for setting the ordering direction of the BoxLayout.

            In contrast to setDirection, this function will not call the callbacks
            after setting the new ordering direction and will not check onLayoutRequest.

            @param direction    ordering direction
        */
        void updateDirection(Direction direction);

    private:
        void onAdd(const value_type&) override;
        void onRemove(const value_type&) override;
        void onChildUpdate(size_type) override;
        void onSizeChange(const Size& size) override;

    private:
        Direction m_direction{Direction::LeftToRight};
    };

    constexpr bool IsHorizontalOrdering(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::LeftToRight) || (direction == BoxLayout::Direction::RightToLeft));
    }

    constexpr bool IsVerticalOrdering(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::TopToBottom) || (direction == BoxLayout::Direction::BottomToTop));
    }

    constexpr bool IsForwardOrdering(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::LeftToRight) || (direction == BoxLayout::Direction::TopToBottom));
    }

    constexpr bool IsReverseOrdering(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::RightToLeft) || (direction == BoxLayout::Direction::BottomToTop));
    }
} // namespace pTK

#endif // PTK_WIDGETS_BOXLAYOUT_HPP
