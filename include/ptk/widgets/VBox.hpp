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
#include <type_traits>

namespace pTK
{
    /** VBox class implementation.


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

    private:
        [[nodiscard]] bool onLayoutRequest(Direction direction) override { return IsVerticalOrdering(direction); }
    };
} // namespace pTK

#endif // PTK_WIDGETS_VBOX_HPP
