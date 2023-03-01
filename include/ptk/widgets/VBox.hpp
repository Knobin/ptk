//
//  widgets/VBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_WIDGETS_VBOX_HPP
#define PTK_WIDGETS_VBOX_HPP

// pTK Headers
#include "ptk/core/WidgetContainer.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** VBox class implementation.

        Derived from Box, this class for holding Cells in
        order of displaying them in a vertical style.
    */
    class PTK_API VBox : public WidgetContainer
    {
    public:
        /** Constructs VBox with default values.

            @return    default initialized VBox
        */
        VBox();

        /** Move Constructor for VBox.

            @return    initialized VBox from value
        */
        VBox(VBox&& other) = default;

        /** Move Assignment operator for VBox.

            @return    VBox with value
        */
        VBox& operator=(VBox&& other) = default;

        /** Destructor for VBox.

        */
        virtual ~VBox() = default;

    protected:
        void refitContent(const Size& size);

    private:
        void onAdd(const value_type&) override;
        void onRemove(const value_type&) override;
        void onChildUpdate(size_type) override;
        void onSizeChange(const Size& size) override;

    private:
        void expandOnAdd(const Size& newSize);
        Size calcMinSize() const;
        Size calcMaxSize() const;

        std::vector<Size::value_type> calcSpaces(Size::value_type height);
        Point::value_type alignChildH(Widget* child, const Size& parentSize, const Size& childSize);
    };
} // namespace pTK

#endif // PTK_WIDGETS_VBOX_HPP
