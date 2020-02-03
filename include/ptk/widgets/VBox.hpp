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
        VBox();
        virtual ~VBox() = default;
        
        /** Function for setting the position of the VBox and its
            children.
         
            Derived from Widget.
         
            @param pos     Position to set
        */
        void setPosHint(const Point& pos) override;
        
    protected:
        void refitContent(const Size& size);

    private:
        void onAdd(const Ref<Widget>&) override;
        void onRemove(const Ref<Widget>&) override;
        void onChildUpdate(size_type) override;
        void onResize(const Size& size) override;
        
    private:
        void expandOnAdd(const Size& newSize);
        
        std::vector<Size::value_type> calcSpaces(Size::value_type height);
        Point::value_type alignChildH(size_type index, const Size& parentSize, const Size& childSize);
        
        Size calculateMinSize() const;
        Size calculateMaxSize() const;
    };
}

#endif // PTK_WIDGETS_VBOX_HPP
