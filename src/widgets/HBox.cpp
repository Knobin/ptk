//
//  widgets/HBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-27.
//

// Local Headers
#include "ptk/widgets/HBox.hpp"
#include "ptk/Log.hpp"

namespace pTK
{       
    HBox::HBox()
        : Box()
    {

    }
    
    void HBox::onAdd(const Ref<Widget>&)
    {
        Size vbSize = getSize();
        Size minLayoutSize = calculateMinSize();
        setMinSize(minLayoutSize);
        
        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            /** Children will not fit in the current size.
             
             Set minimal size to HBox and set minimal size to each child.
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
    
    void HBox::onRemove(const Ref<Widget>&)
    {
        refitContent();
    }
    
    void HBox::onChildUpdate(size_type)
    {
        refitContent();
    }
    
    void HBox::expandOnAdd(const Size& newSize)
    {
        Size layoutSize = newSize;
        Size vbSize = getSize();
        Point vbPos = getPosition();
        size_type children = size();
        
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
            vbPos.x += cMargin.left;
            child->setPosHint(Point(vbPos.x, vbPos.y + alignChildV(i, vbSize, cSize)));
            vbPos.x += cSize.width;
        }
    }
    
    void HBox::refitContent()
    {
        Size layoutSize = calculateMinSize();
        setMinSize(layoutSize);
        Size vbSize = getSize();
        Point vbPos = getPosition();
        size_type children = size();
        std::vector<Size> sizes(children);
        
        // Initialize sizes.
        for (size_type i{0}; i < children; ++i)
        {
            sizes.at(i) = at(i)->getMinSize();

            Size::value_type maxHeight = at(i)->getMaxSize().height;
            sizes.at(i).height = (vbSize.height > maxHeight) ? maxHeight : vbSize.height;
        }
        
        // Expand children to its max sizes possible.
        Size::value_type widthLeft  = vbSize.width - layoutSize.width;
        Size::value_type totalEachLeft = widthLeft;
        
        // Distribute heightLeft.
        // Need to fix this some time.
        // TODO: it takes many iteration before the height is distributed, especially if only 1 can grow.
        while (totalEachLeft > 0)
        {
            Size::value_type eachAdd = static_cast<Size::value_type>(std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(children)));
            eachAdd = (totalEachLeft < static_cast<Size::value_type>(children)) ? 1 : eachAdd;
            bool done = true;
            for (size_type i{0}; i < children; ++i)
            {
                Size::value_type min = sizes.at(i).width;
                Size::value_type max = at(i)->getMaxSize().width;
                Size::value_type delta = max - min;
                
                if (delta > 0)
                {
                    if (eachAdd > delta)
                    {
                        sizes.at(i).width += delta;
                        totalEachLeft -= (delta);
                        done = false;
                    }else
                    {
                        sizes.at(i).width += eachAdd;
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
            vbPos.x += cMargin.left + spaces.at(i);
            child->setPosHint(Point(vbPos.x, vbPos.y + alignChildV(i, vbSize, cSize)));
            vbPos.x += cSize.width + cMargin.right;
        }
    }
    
    void HBox::setPosHint(const Point& pos)
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
    
    void HBox::setSize(const Size& newSize)
    {
        Widget::setSize(newSize);
        refitContent();
    }
    
    Size HBox::calculateMinSize() const
    {
        Size contentMinSize;
        for (size_type i{0}; i < size(); ++i)
        {
            Margin cMargin = at(i)->getMargin();
            Margin::value_type vMargin = cMargin.top + cMargin.bottom;
            Margin::value_type hMargin = cMargin.left + cMargin.right;

            Size cMinSize = at(i)->getMinSize();
            contentMinSize.height = 
                ((cMinSize.height + static_cast<Size::value_type>(vMargin)) > contentMinSize.height) ? 
                static_cast<Size::value_type>(cMinSize.height + vMargin) : contentMinSize.height;
            contentMinSize.width += static_cast<Size::value_type>(hMargin + cMinSize.width);
        }
        
        return contentMinSize;
    }
    
    Size HBox::calculateMaxSize() const
    {
        Size contentMaxSize;
        auto it = cbegin();
        if (it != cend())
        {
            contentMaxSize = (*it)->getMaxSize();
            while (++it != cend())
            {
                Size maxSize = (*it)->getMaxSize();
                contentMaxSize.width += maxSize.width;
                contentMaxSize.height = (maxSize.height < contentMaxSize.height) ? maxSize.height : contentMaxSize.height;
            }
        }
        
        return contentMaxSize;
    }
    
    std::vector<Size::value_type> HBox::calcSpaces(Size::value_type width)
    {
        const size_type children = size();
        const size_type spaceCount = size() + 1;
        std::vector<Size::value_type> spaces(spaceCount);
        if (width != 0)
        {
            for (size_type i{0}; i != children; i++)
            {
                std::underlying_type<Align>::type cAlign = at(i)->getAlign();
                
                if (isAlignSet(cAlign, Align::Left))
                {
                    spaces.at(i) = 0;
                    spaces.at(i+1) = 1;
                }
                else if (isAlignSet(cAlign, Align::Right))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 0;
                }
                else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::HCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 1;
                }
            }
            
            uint spacesToUse = 0;
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;
            
            uint spaceWidth = (spacesToUse != 0) ? static_cast<uint>(width) / spacesToUse : 0;
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceWidth;
        }
        
        return spaces;
    }
    
    Point::value_type HBox::alignChildV(size_type index, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posy{0};
        Margin cMargin = at(index)->getMargin();

        // Align
        std::underlying_type<Align>::type cAlign = at(index)->getAlign();
        if (isAlignSet(cAlign, Align::Bottom))
            posy = static_cast<Point::value_type>(parentSize.height - childSize.height);
        else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::VCenter))
            posy = static_cast<Point::value_type>((parentSize.height/2) - (childSize.height/2));
        else if (cMargin.top > 0)
            posy = static_cast<Point::value_type>(cMargin.top);

        // Apply negative margin.
        posy += (cMargin.top < 0) ? cMargin.top : 0;

        return posy;
    }
}

