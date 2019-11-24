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
        
        /** Function for setting the size of the VBox and its
         children.
         
         Derived from Widget.
         
         @param newSize     Size to set
         */
        void setSize(const Size& newSize) override;
        
    private:
        void onAdd(const Ref<Widget>&) override;
        void onRemove(const Ref<Widget>&) override;
        void onChildUpdate(uint) override;
        
    private:
        void expandOnAdd(const Size& newSize);
        void refitContent();
        
        std::vector<int> calcSpaces(uint height);
        int alignChildH(uint index, const Size& parentSize, const Size& childSize);
        
        Size calculateMinSize() const;
        Size calculateMaxSize() const;
    };
}

#endif // PTK_WIDGETS_VBOX_HPP
