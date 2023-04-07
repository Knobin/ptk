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
    // TODO(knobin): docs.
    class PTK_API BoxLayout : public WidgetContainer
    {
    public:
        // TODO(knobin): docs.
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

        // TODO(knobin): docs.
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

        // TODO(knobin): docs.
        [[nodiscard]] Direction direction() const noexcept { return m_direction; }

        // TODO(knobin): docs.
        void setDirection(Direction direction);

        /** Draw function.

            Function is called when it is time to draw.

            @param canvas   valid Canvas pointer to draw to
        */
        void onDraw(Canvas* canvas) override;

    protected:
        virtual void refitContent(Size size, Point pos);
        virtual void expandOnAdd(Size size, Point pos);
        [[nodiscard]] virtual bool onLayoutRequest(Direction direction);
        virtual void onLayoutChange();

        /** Function that paints all the children in the BoxLayout with respect to set direction.

            @param canvas   valid Canvas pointer to draw to
        */
        void drawChildrenWithDir(Canvas* canvas);

        [[nodiscard]] virtual Size calcMinSize() const;
        [[nodiscard]] virtual Size calcMaxSize() const;

    private:
        void onAdd(const value_type&) override;
        void onRemove(const value_type&) override;
        void onChildUpdate(size_type) override;
        void onSizeChange(const Size& size) override;

    private:
        Direction m_direction{Direction::LeftToRight};
    };

    constexpr bool IsHorizontalDirection(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::LeftToRight) || (direction == BoxLayout::Direction::RightToLeft));
    }

    constexpr bool IsVerticalDirection(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::TopToBottom) || (direction == BoxLayout::Direction::BottomToTop));
    }

    constexpr bool IsForwardLayout(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::LeftToRight) || (direction == BoxLayout::Direction::TopToBottom));
    }

    constexpr bool IsReverseLayout(BoxLayout::Direction direction) noexcept
    {
        return ((direction == BoxLayout::Direction::RightToLeft) || (direction == BoxLayout::Direction::BottomToTop));
    }
} // namespace pTK

#endif // PTK_WIDGETS_BOXLAYOUT_HPP
