//
//  widgets/VBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_WIDGETS_VBOX_HPP
#define PTK_WIDGETS_VBOX_HPP

// pTK Headers
#include "ptk/widgets/BoxLayout.hpp"

namespace pTK
{
    /** VBox class implementation.

        Specialization of BoxLayout for Vertical ordering.
        Does not allow Horizontal ordering and only allowed directions are:
            - TopToBottom
            - BottomToTop
    */
    class PTK_API VBox : public BoxLayout
    {
    public:
        /** Constructs VBox with default values.

            @return    default initialized VBox
        */
        VBox()
            : BoxLayout(BoxLayout::Direction::TopToBottom)
        {}

        /** Constructs VBox with direction.

            @param direction    ordering of widgets that must satisfy IsVerticalOrdering()
            @return             initialized VBox
        */
        explicit VBox(Direction direction)
            : BoxLayout(Direction::TopToBottom)
        {
            if (IsVerticalOrdering(direction))
                updateDirection(direction);
        }

        /** Destructor for VBox.

        */
        ~VBox() override = default;

        /** Move Constructor for VBox.

            @return    initialized VBox from value
        */
        VBox(VBox&& other) = default;

        /** Move Assignment operator for VBox.

            @return    VBox with value
        */
        VBox& operator=(VBox&& other) = default;

    private:
        [[nodiscard]] bool onLayoutRequest(Direction direction) override { return IsVerticalOrdering(direction); }
    };
} // namespace pTK

#endif // PTK_WIDGETS_VBOX_HPP
