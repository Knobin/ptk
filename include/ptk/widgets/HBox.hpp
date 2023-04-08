//
//  widgets/HBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-27.
//

#ifndef PTK_WIDGETS_HBOX_HPP
#define PTK_WIDGETS_HBOX_HPP

// pTK Headers
#include "ptk/widgets/BoxLayout.hpp"

namespace pTK
{
    /** HBox class implementation.

        Specialization of BoxLayout for Horizontal ordering.
        Does not allow Vertical ordering and only allowed directions are:
            - LeftToRight
            - RightToLeft
    */
    class PTK_API HBox : public BoxLayout
    {
    public:
        /** Constructs HBox with default values.

            @return    default initialized HBox
        */
        HBox()
            : BoxLayout(BoxLayout::Direction::LeftToRight)
        {}

        /** Constructs HBox with direction.

            @param direction    ordering of widgets that must satisfy IsHorizontalOrdering()
            @return             initialized HBox
        */
        explicit HBox(Direction direction)
            : BoxLayout(BoxLayout::Direction::LeftToRight)
        {
            if (IsHorizontalOrdering(direction))
                updateDirection(direction);
        }

        /** De-constructor for HBox.

        */
        ~HBox() override = default;

    private:
        [[nodiscard]] bool onLayoutRequest(Direction direction) override { return IsHorizontalOrdering(direction); }
    };
} // namespace pTK

#endif // PTK_WIDGETS_HBOX_HPP
