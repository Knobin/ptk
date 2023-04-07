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

        Derived from Box, this class for holding Cells in
        order of displaying them in a vertical style.
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

        // TODO(knobin): docs.
        explicit VBox(Direction direction)
            : BoxLayout(direction)
        {}

        /** Destructor for VBox.

        */
        ~VBox() override = default;

    private:
        [[nodiscard]] bool onLayoutRequest(Direction direction) override { return IsVerticalDirection(direction); }
    };
} // namespace pTK

#endif // PTK_WIDGETS_VBOX_HPP
