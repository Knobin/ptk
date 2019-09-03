//
//  widgets/VBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_WIDGETS_VBOX_HPP
#define PTK_WIDGETS_VBOX_HPP

// Local Headers
#include "ptk/core/Container.hpp"

// C++ Headers
#include <utility>

namespace pTK
{
    /** VBox class implementation.
     
     Derived from Box, this class for holding Cells in
     order of displaying them in a vertical style.
     */
    class VBox : public Container
    {
    public:
        /** Constructs VBox with default values.
         
         @return    default initialized VBox
         */
        VBox();
        virtual ~VBox();
        
        /** Function for adding a Widget
         This will create a cell and put the Widget in it.
         
         Derived from Box.
         
         @param widget  Widget to add
         @return        Status (true if added)
         */
        bool add(const std::shared_ptr<Widget>& widget) override;
        
        // TODO: override remove
        
        /** Function for a child Widget to call the Parent when it
         need to be drawn.
         
         Derived from Widget.
         
         @param widget  Child Widget
         @return        Status (true if handled)
         */
        bool drawChild(Widget* widget) override;
        
        /** Function for setting the position of the VBox and its
         children.
         
         Derived from Widget.
         
         @param pos     Position to set
         */
        void setPosHint(const Position& pos) override;
        
        /** Function for setting the size of the VBox and its
         children.
         
         Derived from Widget.
         
         @param newSize     Size to set
         */
        void setSize(const Size& newSize) override;
        
        Size calculateMaxSize() const override;
        Size calculateMinSize() const override;
        
    private:
        void expandOnAdd(const Size& newSize);
        void refitOnAdd(const Size& newSize);
        void createMarginsOnAdd(int* data, int unusedHeight);
        
        void sortByHeight(std::pair<uint, Size>* data) const;
        void sortByIndex(std::pair<uint, Size>* data) const;
        
        int readInternalMargins(int* data);
        void readMargins(int* data);
        
        void shrink(const Size& newSize);
        int shrinkChildren(std::pair<uint, Size>* data, int sub, const Size& layoutSize);
        void shrinkMargins(int* data, int unusedHeight);
        
        void grow(const Size& newSize);
        int expandChildren(std::pair<uint, Size>* data, int add, const Size& layoutSize);
        void extendMargins(int* data, int unusedHeight);
        
        int alignChildH(uint index,  const Size& childSize);
        
        void internalUpdate();
        
    private:
        std::vector<void*> m_data;
    };
}

#endif // PTK_WIDGETS_VBOX_HPP
