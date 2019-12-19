//
//  widgets/VBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

// Local Headers
#include "ptk/widgets/VBox.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>


namespace pTK
{       
    VBox::VBox()
        : Box()
    {

    }
    
    void VBox::onAdd(const Ref<Widget>&)
    {
        Size vbSize = getSize();
        Size minLayoutSize = calculateMinSize();
        setMinSize(minLayoutSize);
        
        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            /** Children will not fit in the current size.
             
             Set minimal size to VBox and set minimal size to each child.
             */
            expandOnAdd(minLayoutSize);
        }else
        {
            /** Childs will fit in the current size.
             
             Only need to resize and position children.
             */
            refitContent();
        }
    }
    
    void VBox::onRemove(const Ref<Widget>&)
    {
        refitContent();
    }
    
    void VBox::onChildUpdate(uint)
    {
        refitContent();
    }
    
    void VBox::expandOnAdd(const Size& newSize)
    {
        Size layoutSize = newSize;
        Size vbSize = getSize();
        Point vbPos = getPosition();
        size_t children = size();
        
        layoutSize.height   = (vbSize.height > layoutSize.height) ? vbSize.height : layoutSize.height;
        layoutSize.width    = (vbSize.width > layoutSize.width) ? vbSize.width : layoutSize.width;
        
        setSize(layoutSize); // this will generate a Resize event.
        for (uint i = 0; i < children; ++i)
        {
            auto child = at(i);
            Size cSize = child->getMinSize();
            
            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
             */
            child->setSize(cSize);
            Margin cMargin = child->getMargin();
            vbPos.y += cMargin.top;
            child->setPosHint(Point(vbPos.x + alignChildH(i, newSize, cSize), vbPos.y));
            vbPos.y += cSize.height + cMargin.bottom;
        }
    }
    
    void VBox::refitContent()
    {
        Size layoutSize = calculateMinSize();
        setMinSize(layoutSize);
        Size vbSize = getSize();
        Point vbPos = getPosition();
        size_t children = size();
        std::vector<Size> sizes(children);
        
        // Initialize sizes.
        for (uint i = 0; i < children; ++i)
        {
            sizes.at(i) = at(i)->getMinSize();
            int maxWidth = at(i)->getMaxSize().width;
            sizes.at(i).width = (vbSize.width > maxWidth) ? maxWidth : vbSize.width;
        }
        
        // Expand children to its max sizes possible.
        int heightLeft  = vbSize.height - layoutSize.height;
        int totalEachLeft = heightLeft;
        
        // Distribute heightLeft.
        // Need to fix this some time.
        // TODO: it takes many iteration before the height is distributed, especially if only 1 can grow.
        while (totalEachLeft > 0)
        {
            int eachAdd = static_cast<int>(std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(children)));
            eachAdd = (totalEachLeft < (int)children) ? 1 : eachAdd;
            bool done = true;
            for (uint i = 0; i < children; ++i)
            {
                int min = sizes.at(i).height;
                int max = at(i)->getMaxSize().height;
                int delta = max - min;
                
                if (delta > 0)
                {
                    if (eachAdd > delta)
                    {
                        sizes.at(i).height += delta;
                        totalEachLeft -= (delta);
                        done = false;
                    }else
                    {
                        sizes.at(i).height += eachAdd;
                        totalEachLeft -= (eachAdd);
                        done = false;
                    }
                }
                
                if (totalEachLeft == 0)
                {
                    done = true;
                    break;
                }
            }
            
            // We cannot add more to the widgets.
            if ((done) || (eachAdd == 0))
                break;
        }
        
        // TODO: Fix if we break in while loop (size left unused).
        std::vector<int> spaces = calcSpaces(totalEachLeft);
        
        // Set sizes to childs and spaces.
        for (uint i = 0; i != children; i++)
        {
            auto child = at(i);
            Size cSize = sizes.at(i);
            child->setSize(cSize);

            Margin cMargin = child->getMargin();
            vbPos.y += cMargin.top + spaces.at(i);
            child->setPosHint(Point(vbPos.x + alignChildH(i, vbSize, cSize), vbPos.y));
            vbPos.y += cSize.height + cMargin.bottom;
        }
    }
    
    void VBox::setPosHint(const Point& pos)
    {
        Point currentPos = getPosition();
        Point deltaPos = pos - currentPos;
        
        for (auto it = begin(); it != end(); it++)
        {
            Point wPos = (*it)->getPosition();
            wPos += deltaPos;
            (*it)->setPosHint(wPos);
        }
        
        Widget::setPosHint(pos);
    }
    
    void VBox::setSize(const Size& newSize)
    {
        Widget::setSize(newSize);
        refitContent();
    }
    
    Size VBox::calculateMinSize() const
    {
        Size contentMinSize;
        for (uint i = 0; i < size(); ++i)
        {
            Margin cMargin = at(i)->getMargin();
            int hMargin = cMargin.top + cMargin.bottom;
            int vMargin = cMargin.left + cMargin.right;
            
            Size cMinSize = at(i)->getMinSize();
            contentMinSize.height += (vMargin + cMinSize.height);
            contentMinSize.width = ((cMinSize.width + hMargin) > contentMinSize.width) ? cMinSize.width + hMargin : contentMinSize.width;
        }
        
        return contentMinSize;
    }
    
    Size VBox::calculateMaxSize() const
    {
        Size contentMaxSize;
        auto it = cbegin();
        if (it != cend())
        {
            contentMaxSize = (*it)->getMaxSize();
            while (++it != cend())
            {
                Size maxSize = (*it)->getMaxSize();
                contentMaxSize.width = (maxSize.width < contentMaxSize.width) ? maxSize.width : contentMaxSize.width;
                contentMaxSize.height += maxSize.height;
            }
        }
        
        return contentMaxSize;
    }
    
    std::vector<int> VBox::calcSpaces(uint height)
    {
        const uint children = size();
        const size_t spaceCount = size() + 1;
        std::vector<int> spaces(spaceCount);
        if (height != 0)
        {
            for (uint i = 0; i != children; i++)
            {
                int32 cAlign = at(i)->getAlign();
                
                if (isAlignSet(cAlign, Align::Top))
                {
                    spaces.at(i) = 0;
                    spaces.at(i+1) = 1;
                }
                else if (isAlignSet(cAlign, Align::Bottom))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 0;
                }
                else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::VCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 1;
                }
            }
            
            uint spacesToUse = 0;
            for (uint i = 0; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;
            
            uint spaceHeight = (spacesToUse != 0) ? height / spacesToUse : 0;
            for (uint i = 0; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceHeight;
            
        }
        
        return spaces;
    }
    
    int VBox::alignChildH(uint index, const Size& parentSize, const Size& childSize)
    {
        int posx = 0;
        Margin cMargin = at(index)->getMargin();

        // Align
        int32 cAlign = at(index)->getAlign();
        if (isAlignSet(cAlign, Align::Right))
            posx = parentSize.width - childSize.width;
        else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::HCenter))
            posx = (parentSize.width/2) - (childSize.width/2);
        else if (cMargin.left > 0)
            posx = cMargin.left;

        // Apply negative margin.
        posx += (cMargin.left < 0) ? cMargin.left : 0;

        return posx;
    }
}

