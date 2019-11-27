//
//  widgets/HBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-27.
//

#ifndef PTK_WIDGETS_HBOX_HPP
#define PTK_WIDGETS_HBOX_HPP

// Local Headers
#include "ptk/core/Box.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** HBox class implementation.
     
     Derived from Box, this class for holding Cells in
     order of displaying them in a vertical style.
     */
    class HBox : public Box
    {
    public:
        /** Constructs HBox with default values.
         
         @return    default initialized HBox
         */
        HBox();
        virtual ~HBox() = default;
        
        /** Function for setting the position of the HBox and its
         children.
         
         Derived from Widget.
         
         @param pos     Position to set
         */
        void setPosHint(const Point& pos) override;
        
        /** Function for setting the size of the HBox and its
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
        
        std::vector<int> calcSpaces(uint width);
        int alignChildV(uint index, const Size& parentSize, const Size& childSize);
        
        Size calculateMinSize() const;
        Size calculateMaxSize() const;
    };
}

#endif // PTK_WIDGETS_HBOX_HPP
