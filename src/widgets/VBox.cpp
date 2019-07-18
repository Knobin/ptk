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
    
    bool VBox::add(const std::shared_ptr<Widget>& widget)
    {
        if (Box::add(widget))
        {
            Size boxSize = getSize();
            Position pos = getPosition();
            uint cellCount = size();
            
            Size minLayoutSize{};
            forEach([&minLayoutSize](const std::shared_ptr<Cell>& cell){
                Size cSize = cell->getMinSize();
                minLayoutSize.height += cSize.height;
                minLayoutSize.width = (cSize.width > minLayoutSize.width) ? cSize.width : minLayoutSize.width;
            });
            
            Widget::setMinSize(calculateMinSize());
            Widget::setMaxSize(calculateMaxSize());
            
            // Cells wont fit, resize VBox.
            if ((minLayoutSize.width > boxSize.width) || (minLayoutSize.height > boxSize.height))
            {
                setSize(minLayoutSize); // this will generate a Resize event.
                for (auto it = begin(); it != end(); it++)
                {
                    Size cSize = (*it)->getMinSize();
                    (*it)->setSize(Size(minLayoutSize.width, cSize.height));
                    (*it)->setPosHint(pos);
                    pos.y += (*it)->getSize().height;
                }
                
                return true;
            }
            
            // TODO
            int delta = std::ceil(((float)boxSize.height - (float)minLayoutSize.height) / (float)cellCount);
            uint heightDiff = (delta*cellCount) + minLayoutSize.height - boxSize.height;
            
            // Should probably replace with a set.
            std::vector<std::pair<uint, int>> sizes;
            sizes.reserve(cellCount);
            
            int remainder = heightDiff % cellCount;
            int add = heightDiff - remainder;
            
            for (uint i = 0; i < cellCount; ++i)
            {
                Size cMinSize = at(i)->getMinSize();
                cMinSize.height += (delta + add);
                sizes.push_back({i, cMinSize.height});
            }
            
            if (remainder != 0)
            {
                std::sort(sizes.begin(), sizes.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
                    return lhs.second > rhs.second;
                });
                
                for (int i = 0; i < remainder; ++i)
                sizes.at(i).second -= 1;
                
                std::sort(sizes.begin(), sizes.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
                    return lhs.first < rhs.first;
                });
            }

            // No resize needed. only repositioning.
            for (uint i = 0; i != cellCount; i++)
            {
                auto cell = at(i);
                cell->setSize(Size(boxSize.width, sizes.at(i).second));
                cell->setPosHint(pos);
                pos.y += sizes.at(i).second;
            }
            PTK_INFO("{0}", pos.y);
            
            draw();
            return true;
        }
        
        return false;
    }
    
    bool VBox::drawChild(Widget* widget)
    {
        iterator it = findIf([widget](const std::shared_ptr<Cell>& cell){
            if (cell->getWidget().get() == widget)
                return true;
            return false;
        });
        if (it != end())
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
        for (auto it = cbegin(); it != cend(); it++)
        {
            Size minSize = (*it)->getMinSize();
            contentMinSize.width = (minSize.width > contentMinSize.width) ? minSize.width : contentMinSize.width;
            contentMinSize.height = (minSize.height == -1) ? -1 : contentMinSize.height + minSize.height;
        }
        
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
    
    void VBox::shrink(const Size& newSize) const
    {
        // We should not really have to do anything with width.
        // The largest Cell in VBox should be the min size possible
        // for the parent of this.
        PTK_ASSERT((getMinSize().width <= newSize.width), "Exceeded min size of VBox");
    }
    
    void VBox::grow(const Size& newSize) const
    {
        // We should not really have to do anything with width.
        // The smallest Cell in VBox should be the max size possible
        // for the parent of this.
        PTK_ASSERT(((getMaxSize().width <= newSize.width) && (getMaxSize().width == -1)), "Exceeded max size of VBox");
        
        uint cellCount = size();
        Size currentSize = getSize();
        std::vector<std::pair<uint, int>> internal;
        internal.reserve(cellCount);
        
        int deltaHeight = std::ceil(((float)newSize.height - (float)currentSize.height) / (float)cellCount);
        int heightDiff = (deltaHeight*cellCount) + currentSize.height - newSize.height;
        
        int remainder = heightDiff % cellCount;
        int add = heightDiff - remainder;
        int max = deltaHeight + add + 1;
        
        bool doable = true;
        for (uint i = 0; i < cellCount; ++i)
        {
            auto cell = at(i);
            Size cMaxSize = cell->getMaxSize();
            Size cSize = cell->getSize();
            if (((cMaxSize.height-cSize.height) < max) && (cMaxSize.height != -1))
                doable = false;
                
            internal.push_back({i, cSize.height + deltaHeight + add});
        }
        
        // If all Cells can grow without hitting their max size.
        if (doable)
        {
            // If there are some pixels overflow we remove some the largest Cells.
            if (remainder != 0)
            {
                std::sort(internal.begin(), internal.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
                    return lhs.second > rhs.second;
                });
                
                for (int i = 0; i < remainder; ++i)
                    internal.at(i).second -= 1;
                
                std::sort(internal.begin(), internal.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
                    return lhs.first < rhs.first;
                });
            }
        }else
        {
            ; // TODO: One or more Cells cant grow.
        }
        
        // No resize needed. only repositioning.
        Position pos = getPosition();
        for (uint i = 0; i != cellCount; i++)
        {
            auto cell = at(i);
            int cHeight = internal.at(i).second;
            cell->setSize(Size(newSize.width, cHeight));
            cell->setPosHint(pos);
            pos.y += cHeight;
        }
        PTK_INFO("{0} should be {1}", pos.y, newSize.height);
    }
}

