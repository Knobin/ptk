//
//  widgets/HBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-27.
//

#ifndef PTK_WIDGETS_HBOX_HPP
#define PTK_WIDGETS_HBOX_HPP

// pTK Headers
#include "ptk/core/WidgetContainer.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** HBox class implementation.

        Derived from Box, this class for holding Cells in
        order of displaying them in a vertical style.
    */
    class PTK_API HBox : public WidgetContainer
    {
    public:
        /** Constructs HBox with default values.

            @return    default initialized HBox
        */
        HBox();

        /** Move Constructor for HBox.

            @return    initialized HBox from value
        */
        HBox(HBox&& other) = default;

        /** Move Assignment operator for HBox.

            @return    HBox with value
        */
        HBox& operator=(HBox&& other) = default;

        /** De-constructor for HBox.

        */
        virtual ~HBox() = default;

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

        std::vector<Size::value_type> calcSpaces(Size::value_type width);
        Point::value_type alignChildV(Widget* child, const Size& parentSize, const Size& childSize);
    };
} // namespace pTK

#endif // PTK_WIDGETS_HBOX_HPP
