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
            refitContent(vbSize);
        }
    }
    
    void VBox::onRemove(const Ref<Widget>&)
    {
        refitContent(getSize());
    }
    
    void VBox::onChildUpdate(size_type)
    {
        setMinSize(calculateMinSize());
        //setMaxSize(calculateMaxSize());
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
        for (size_type i{0}; i < children; ++i)
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
            Padding cPadding = child->getPadding();
            vbPos.y += cMargin.top + cPadding.top;
            child->setPosHint(Point(vbPos.x + alignChildH(i, newSize, cSize), vbPos.y));
            vbPos.y += cSize.height + cMargin.bottom + cPadding.bottom;
        }
    }
    
    void VBox::refitContent(const Size& nsize)
    {
        Size layoutSize = calculateMinSize();
        Size vbSize = nsize;
        Point vbPos = getPosition();
        size_type children = size();
        std::vector<Size> sizes(children);
        
        // Initialize sizes.
        for (uint i{0}; i < children; ++i)
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
            Size::value_type eachAdd = static_cast<int>(std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(children)));
            eachAdd = (totalEachLeft < (int)children) ? 1 : eachAdd;
            bool done = true;
            for (size_type i{0}; i < children; ++i)
            {
                Size::value_type min = sizes.at(i).height;
                Size::value_type max = at(i)->getMaxSize().height;
                Size::value_type delta = max - min;
                
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
        std::vector<Size::value_type> spaces = calcSpaces(totalEachLeft);
        
        // Set sizes to childs and spaces.
        for (size_type i{0}; i != children; i++)
        {
            auto child = at(i);
            Size cSize = sizes.at(i);
            child->setSize(cSize);

            Margin cMargin = child->getMargin();
            Padding cPadding = child->getPadding();
            vbPos.y += cMargin.top + cPadding.top + spaces.at(i);
            child->setPosHint(Point(vbPos.x + alignChildH(i, vbSize, cSize), vbPos.y));
            vbPos.y += cSize.height + cMargin.bottom + cPadding.bottom;
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

    void VBox::onResize(const Size& size)
    {
        refitContent(size);
    }
    
    Size VBox::calculateMinSize() const
    {
        Size contentMinSize{Size::Min};
        for (auto it{cbegin()}; it != cend(); ++it)
        {
            Padding cPadding = (*it)->getPadding();
            Padding::value_type vPadding = cPadding.top + cPadding.bottom;
            Padding::value_type hPadding = cPadding.left + cPadding.right;

            Size cMinSize = (*it)->getMinSize();
            contentMinSize.height += static_cast<Size::value_type>(vPadding + cMinSize.height);
            contentMinSize.width =
                ((cMinSize.width + static_cast<Size::value_type>(hPadding)) > contentMinSize.width)
                ? cMinSize.width + static_cast<Size::value_type>(hPadding) : contentMinSize.width;
        }
        
        return contentMinSize;
    }
    
    Size VBox::calculateMaxSize() const
    {
        Size contentMaxSize{Size::Max};
        for (auto it{cbegin()}; it != cend(); ++it)
        {
            Padding cPadding = (*it)->getPadding();
            Padding::value_type vPadding = cPadding.top + cPadding.bottom;
            Padding::value_type hPadding = cPadding.left + cPadding.right;

            Size maxSize = (*it)->getMaxSize();
            contentMaxSize.height += (maxSize.height < (contentMaxSize.height - static_cast<Size::value_type>(vPadding)))
                ? maxSize.height + static_cast<Size::value_type>(vPadding) : contentMaxSize.height;
            contentMaxSize.width += (maxSize.width < (contentMaxSize.width - static_cast<Size::value_type>(hPadding)))
                ? maxSize.width + static_cast<Size::value_type>(hPadding) : contentMaxSize.width;
        }
        
        return contentMaxSize;
    }
    
    std::vector<Size::value_type> VBox::calcSpaces(Size::value_type height)
    {
        const size_type children = size();
        const size_type spaceCount = size() + 1;
        std::vector<Size::value_type> spaces(spaceCount);
        if (height != 0)
        {
            for (size_type i{0}; i != children; i++)
            {
                std::underlying_type<Align>::type cAlign = at(i)->getAlign();
                
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
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;
            
            uint spaceHeight = (spacesToUse != 0) ? height / spacesToUse : 0;
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceHeight;
            
        }
        
        return spaces;
    }
    
    Point::value_type VBox::alignChildH(size_type index, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posx{0};
        Size cSize{childSize};
        Margin cMargin{at(index)->getMargin()};
        Padding cPadding{at(index)->getPadding()};
        
        // Pre
        cSize.width += static_cast<Size::value_type>(cPadding.left + cPadding.right);

        // Align
        std::underlying_type<Align>::type cAlign = at(index)->getAlign();
        if (isAlignSet(cAlign, Align::Right))
            posx = static_cast<Point::value_type>(parentSize.width - cSize.width);
        else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::HCenter))
            posx = static_cast<Point::value_type>((parentSize.width/2) - (cSize.width/2));

        // Post
        posx += static_cast<Point::value_type>(cMargin.left);
        posx += static_cast<Point::value_type>(cPadding.left);

        return posx;
    }
}

