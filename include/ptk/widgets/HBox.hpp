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

        Derived from Box, this class for holding Cells in
        order of displaying them in a vertical style.
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

        // TODO(knobin): docs.
        explicit HBox(Direction direction)
            : BoxLayout(direction)
        {}

        /** De-constructor for HBox.

        */
        ~HBox() override = default;

    private:
        [[nodiscard]] bool onLayoutRequest(Direction direction) override { return IsHorizontalDirection(direction); }
    };
} // namespace pTK

#endif // PTK_WIDGETS_HBOX_HPP
