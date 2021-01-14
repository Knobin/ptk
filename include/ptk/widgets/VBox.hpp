//
//  widgets/VBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_WIDGETS_VBOX_HPP
#define PTK_WIDGETS_VBOX_HPP

// Local Headers
#include "ptk/core/Box.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** VBox class implementation.
     
        Derived from Box, this class for holding Cells in
        order of displaying them in a vertical style.
    */
    class VBox : public Box
    {
    public:
        /** Constructs VBox with default values.
         
            @return    default initialized VBox
        */
        VBox() = default;

        /** Deconstructor for VBox.

        */
        virtual ~VBox() = default;
        
    protected:
        void refitContent(const Size& size);

    private:
        void onAddNotify(const Ref<Widget>&) override;
        void onRemoveNotify(const Ref<Widget>&) override;
        void onChildUpdate(size_type) override;
        void onSizeChange(const Size& size) override;
        
    private:
        void expandOnAdd(const Size& newSize);
        Size calcMinSize() const;
        Size calcMaxSize() const;
        
        std::vector<Size::value_type> calcSpaces(Size::value_type height);
        Point::value_type alignChildH(size_type index, const Size& parentSize, const Size& childSize);
    };
}

#endif // PTK_WIDGETS_VBOX_HPP
