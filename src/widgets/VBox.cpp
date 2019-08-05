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

// TODO: add and grow needs to be refactored...

namespace pTK
{
    VBox::VBox()
        : Container()
    {
    }
    
    bool VBox::add(const std::shared_ptr<Widget>& widget)
    {
        if (Container::add(widget))
        {
            Size boxSize = getSize();
            Position pos = getPosition();
            uint cellCount = size();
            
            Size minLayoutSize = calculateMinSize();
            Widget::setMinSize(minLayoutSize);
            Widget::setMaxSize(calculateMaxSize());
            
            /** Childs will not fit in the current size.
             
                Set minimal size to VBox and set minimal size to each child.
             */
            if ((minLayoutSize.width > boxSize.width) || (minLayoutSize.height > boxSize.height))
            {
                minLayoutSize.height = (boxSize.height > minLayoutSize.height) ? boxSize.height : minLayoutSize.height;
                minLayoutSize.width = (boxSize.width > minLayoutSize.width) ? boxSize.width : minLayoutSize.width;
                
                setSize(minLayoutSize); // this will generate a Resize event.
                for (uint i = 0; i < cellCount; ++i)
                {
                    auto cell = at(i);
                    
                    Size cMinSize = cell->getMinSize();
                    Size cSize = cell->getSize();
                    if (cMinSize.height != -1)
                        cSize.height = cMinSize.height;
                    if (cMinSize.width != -1)
                        cSize.width = cMinSize.width;
                    
                    cell->setSize(cSize);
                    Margin cMargin = cell->getMargin();
                    pos.y += ((cMargin.top < 0) ? 0 : cMargin.top);
                    cell->setPosHint(Position(pos.x + alignChild(i, cSize), pos.y));
                    pos.y += cSize.height + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
                }
                
                return true;
            }
            
            /** Childs will fit in the current size.
                delta       = height that each child has to grow.
                heightDiff  = height over current size.
                remainder   = height that cannot be distributed across all childs.
                add         = height to add to each child.
             */
            int delta = std::floor(((float)boxSize.height - (float)minLayoutSize.height) / (float)cellCount);
            uint heightDiff = boxSize.height - ((delta * cellCount) + minLayoutSize.height);
            int remainder = heightDiff % cellCount;
            int add = heightDiff - remainder + delta;
            
            Size* sizes = new Size[cellCount];
            PTK_ASSERT(sizes, "error obtaining memory on heap");
            
            // Get height diffs on content.
            for (uint i = 0; i < cellCount; ++i)
            {
                Size cMinSize = at(i)->getMinSize();
                Size cSize = at(i)->getSize();
                Size cMaxSize = at(i)->getMaxSize();
                
                int width = 0;
                if ((cMinSize.width != -1) && (cMaxSize.width != -1))
                    width = cMaxSize.width - cMinSize.width;
                else if ((cMinSize.width == -1) && (cMaxSize.width != -1))
                    width = cMaxSize.width - cSize.width;
                else if ((cMaxSize.width == -1) && (cMinSize.width != -1))
                    width = cSize.width - cMinSize.width;
                
                int height = 0;
                if ((cMinSize.height != -1) && (cMaxSize.height != -1))
                    height = cMaxSize.height - cMinSize.height;
                else if ((cMinSize.height == -1) && (cMaxSize.height != -1))
                    height = cMaxSize.height - cSize.height;
                else if ((cMaxSize.height == -1) && (cMinSize.height != -1))
                    height = cSize.height - cMinSize.height;
                
                sizes[i] = Size(width, height);
            }
            
            int unusedHeight = remainder;
            for (uint i = 0; i < cellCount; ++i)
            {
                Size cMinSize = at(i)->getMinSize();
                if (cMinSize.height == -1)
                {
                    unusedHeight += add;
                    sizes[i].height = at(i)->getSize().height;
                } else if (sizes[i].height < add)
                {
                    // Child cannot grow.
                    unusedHeight += add - sizes[i].height;
                    sizes[i].height += cMinSize.height;
                    
                }else if (sizes[i].height >= add)
                {
                    // Child can grow.
                    sizes[i].height = cMinSize.height + add;
                }
                
                Size cMaxSize = at(i)->getMaxSize();
                if (cMaxSize.width != -1)
                {
                    if (cMaxSize.width > minLayoutSize.width)
                        sizes[i].width = minLayoutSize.width;
                    else
                        sizes[i].width = cMaxSize.width;
                }else
                {
                    sizes[i].width = at(i)->getSize().width;
                }
                
            }
            
            // Spaces.
            // TODO: Maybe fix missing pixels from floor.
            // For now it will put them at the end.
            int spaceHeight = std::floor((float)unusedHeight / (float)(cellCount+1));
            
            // Set sizes to childs and spaces.
            for (uint i = 0; i != cellCount; i++)
            {
                pos.y += spaceHeight;
                auto cell = at(i);
                cell->setSize(sizes[i]);
                Margin cMargin = cell->getMargin();
                pos.y += ((cMargin.top < 0) ? 0 : cMargin.top);
                cell->setPosHint(Position(pos.x + alignChild(i, sizes[i]), pos.y));
                pos.y += sizes[i].height + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
            }
            
            draw();
            
            delete [] sizes;
            return true;
        }
        
        return false;
    }
    
    bool VBox::drawChild(Widget* widget)
    {
        if (find(widget) != cend())
        {
            draw();
            return true;
        }
        
        return false;
    }
    
    void VBox::setPosHint(const Position& pos)
    {
        Position currentPos = getPosition();
        Position deltaPos = pos - currentPos;
        
        for (auto it = begin(); it != end(); it++)
        {
            Position wPos = (*it)->getPosition();
            wPos += deltaPos;
            (*it)->setPosHint(wPos);
        }
        
        Widget::setPosHint(pos);
    }
    
    void VBox::setSize(const Size& newSize)
    {
        Size boxSize = getSize();
        int height = newSize.height - boxSize.height;
        int width = newSize.width - boxSize.width;
        
        if ((height > 0) || (width > 0))
            grow(newSize);
        else if ((height < 0) || (width < 0))
            shrink(newSize);
        
        Widget::setSize(newSize);
    }
    
    Size VBox::calculateMinSize() const
    {
        Size contentMinSize;
        forEach([&contentMinSize](const std::shared_ptr<Widget>& item){
            Margin cMargin = item->getMargin();
            int hMargin = ((cMargin.top < 0) ? 0 : cMargin.top) + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
            int vMargin = ((cMargin.left < 0) ? 0 : cMargin.left) + ((cMargin.right < 0) ? 0 : cMargin.right);
            
            Size cMinSize = item->getMinSize();
            Size cSize = item->getSize();
            cSize.width = ((cMinSize.width == -1) ? cSize.width : cMinSize.width) + vMargin;
            
            contentMinSize.height += ((cMinSize.height == -1) ? cSize.height : cMinSize.height) + hMargin;
            contentMinSize.width = (cSize.width > contentMinSize.width) ? cSize.width : contentMinSize.width;
        });
        
        return contentMinSize;
    }
    
    Size VBox::calculateMaxSize() const
    {
        Size contentMaxSize;
        auto it = cbegin();
        if (it != cend())
        {
            contentMaxSize.height = (*it)->getMaxSize().height;
            contentMaxSize.width = (*it)->getMaxSize().width;
            while (++it != cend())
            {
                Size maxSize = (*it)->getMaxSize();
                contentMaxSize.width = (maxSize.width < contentMaxSize.width) ? maxSize.width : contentMaxSize.width;
                contentMaxSize.height = (maxSize.height == -1) ? -1 : contentMaxSize.height + maxSize.height;
            }
        }
        
        return contentMaxSize;
    }
    
    void VBox::shrink(const Size& newSize)
    {
        // We should not really have to do anything with width.
        // The largest Cell in VBox should be the min size possible
        // for the parent of this.
        PTK_ASSERT((getMinSize().width <= newSize.width), "Exceeded min width of VBox");
        PTK_ASSERT((getMinSize().height <= newSize.height), "Exceeded min height of VBox");
    }
    
    void VBox::grow(const Size& newSize)
    {
        // We should not really have to do anything with width.
        // The smallest Cell in VBox should be the max size possible
        // for the parent of this.
        PTK_ASSERT(((getMaxSize().width <= newSize.width) && (getMaxSize().width == -1)), "Exceeded max width of VBox");
        PTK_ASSERT(((getMaxSize().height <= newSize.height) && (getMaxSize().height == -1)), "Exceeded max height of VBox");
        
        const uint cellCount = size();
        Size currentSize = getSize();
        Position pos = getPosition();
        
        // sizes is used for resizing the widgets. If possible.
        Size* sizes = new Size[cellCount];
        PTK_ASSERT(sizes, "error obtaining memory on heap");
        
        // spaceData is the space between widgets and VBox.
        // There is widgets + 1 spaces.
        const uint spaceCount = cellCount + 1;
        int* spaceData = new int[spaceCount];
        PTK_ASSERT(spaceData, "error obtaining memory on heap");
        
        // Get height diffs on content and current space information.
        for (uint i = 0; i < cellCount; ++i)
        {
            // Sizes
            Size cSize = at(i)->getSize();
            Size cMaxSize = at(i)->getMaxSize();
            
            int width = 0;
            if (cMaxSize.width != -1)
                width = cMaxSize.width - cSize.width;
            
            int height = 0;
            if (cMaxSize.height != -1)
                height = cMaxSize.height - cSize.height;
            
            sizes[i] = Size(width, height);
            
            // spaceData
            Position cPos = at(i)->getPosition();
            Margin cMargin = at(i)->getMargin();
            if (i == 0)
                spaceData[i] = cPos.y - pos.y - ((cMargin.top < 0) ? 0 : cMargin.top);
            
            if (i != (cellCount-1))
            {
                int marginBottom = (cMargin.bottom < 0) ? 0 : cMargin.bottom;
                int marginTop = at(i+1)->getMargin().top;
                spaceData[i+1] = at(i+1)->getPosition().y - ((marginTop < 0) ? 0 : marginTop) - (cPos.y + cSize.height + marginBottom);
            }
            else if (i == (cellCount-1))
                spaceData[i+1] = currentSize.height - (cPos.y + cSize.height + ((cMargin.bottom < 0) ? 0 : cMargin.bottom));
        }
        
        int deltaHeight = std::floor(((float)newSize.height - (float)currentSize.height) / (float)cellCount);
        int heightDiff = newSize.height - ((deltaHeight*cellCount) + currentSize.height);
        int remainder = heightDiff % cellCount;
        int add = (heightDiff - remainder) + deltaHeight;
        
        int unusedHeight = remainder;
        for (uint i = 0; i < cellCount; ++i)
        {
            Size cSize = at(i)->getSize();
            Size cMaxSize = at(i)->getMaxSize();
            
            if (sizes[i].height < add)
            {
                // Child cannot grow.
                unusedHeight += add - sizes[i].height;
                sizes[i].height += cSize.height;
                
            }else if (sizes[i].height >= add)
            {
                // Child can grow.
                sizes[i].height = cSize.height + add;
            }
            
            if (cMaxSize.width != -1)
            {
                if (cMaxSize.width > newSize.width)
                    sizes[i].width = cMaxSize.width;
                else
                    sizes[i].width = cSize.width;
            }else
            {
                sizes[i].width = cSize.width;
            }
        }
        
        int spaceHeight = std::floor((float)unusedHeight / (float)spaceCount);
        int spaceDiff = unusedHeight - (spaceHeight * (int)spaceCount);
        int spaceRemainder = spaceDiff % spaceCount;
        int spaceAdd = (spaceDiff - spaceRemainder) + spaceHeight;
        
        for (uint i = 0; i < spaceCount; ++i)
            spaceData[i] += spaceAdd;
        
        while (spaceRemainder != 0)
        {
            // Find smallest
            int smallest = spaceData[0];
            int location = 0;
            for (uint i = 1;  i < spaceCount; ++i)
            {
                if (spaceData[i] < smallest)
                {
                    smallest = spaceData[i];
                    location = i;
                }
            }

            spaceData[location] += 1;
            spaceRemainder--;
        }
        
        // Set sizes to childs and spaces.
        pos.y += spaceData[0];
        for (uint i = 0; i != cellCount; i++)
        {
            auto cell = at(i);
            Margin cMargin = cell->getMargin();
            cell->setSize(sizes[i]);
            pos.y += ((cMargin.top < 0) ? 0 : cMargin.top);
            cell->setPosHint(Position(pos.x + alignChild(i, sizes[i]), pos.y));
            pos.y += sizes[i].height + spaceData[i+1] + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
        }
        
        delete [] sizes;
        delete [] spaceData;
    }
    
    int VBox::alignChild(uint index, const Size& childSize)
    {
        Size pSize = getSize();
        Align::Horizontal alignment = at(index)->getAlignHorizontal();
        Margin cMargin = at(index)->getMargin();
        int posx = 0;
        
        if (alignment == Align::Horizontal::Center)
        {
            posx += pSize.width/2;
            posx -= childSize.width/2;
            posx = (posx < 0) ? 0 : posx;
            
            posx += cMargin.left;
            posx -= cMargin.right;
        }
        
        return posx;
    }
}

