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
            
            /** Childs will not fit in the current size.
             
                Set minimal size to VBox and set minimal size to each child.
             */
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
            
            /** Childs will fit in the current size.
                delta       = height that each child has to grow.
                heightDiff  = height over current size.
                remainder   = height that cannot be distributed across all childs.
                add         = height to add to each child.
             */
            int delta = std::ceil(((float)boxSize.height - (float)minLayoutSize.height) / (float)cellCount);
            uint heightDiff = (delta * cellCount) + minLayoutSize.height - boxSize.height;
            int remainder = heightDiff % cellCount;
            int add = heightDiff - remainder + delta;
            
            std::vector<std::pair<uint, int>> sizes;
            getSortedHeightDiffs(sizes);
            
            int unusedHeight = 0;
            int unusedDelta = 0;
            for (uint i = 0; i < sizes.size(); ++i)
            {
                std::pair<uint, int>& pair = sizes.at(i);
                
                if (pair.second < add && pair.second != -1)
                {
                    // Child cannot grow.
                    unusedHeight += add - pair.second;
                    pair.second += at(pair.first)->getMinSize().height;
                    
                }else if (pair.second >= add || pair.second == -1)
                {
                    // Child can grow.
                    if (unusedDelta == 0)
                        unusedDelta = unusedHeight / (sizes.size() - i);
                    
                    if ((unusedDelta + add) <= pair.second || pair.second == -1)
                    {
                        // Child can grow with new adder.
                        unusedHeight -= unusedDelta;
                        pair.second = at(pair.first)->getMinSize().height + unusedDelta + add;
                    }else if ((unusedDelta + add) > pair.second)
                    {
                        // Child cannot grow with new adder.
                        unusedHeight += (unusedDelta + add) - pair.second;
                        pair.second += at(pair.first)->getMinSize().height;
                        
                        unusedDelta = unusedHeight / (sizes.size() - i);
                    }
                }
            }
            
            // If there are pixels overflow we remove some to the largest childs (sort with greater).
            std::sort(sizes.begin(), sizes.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
                return lhs.first > rhs.first;
            });
            addRemainder(sizes, remainder, -1);
            
            // Set sizes to childs.
            applySizes(sizes);
            
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
        PTK_ASSERT((getMinSize().width <= newSize.width), "Exceeded min width of VBox");
        PTK_ASSERT((getMinSize().height <= newSize.height), "Exceeded min height of VBox");
    }
    
    void VBox::grow(const Size& newSize) const
    {
        // We should not really have to do anything with width.
        // The smallest Cell in VBox should be the max size possible
        // for the parent of this.
        PTK_ASSERT(((getMaxSize().width <= newSize.width) && (getMaxSize().width == -1)), "Exceeded max width of VBox");
        PTK_ASSERT(((getMaxSize().height <= newSize.height) && (getMaxSize().height == -1)), "Exceeded max height of VBox");
        
        uint cellCount = size();
        Size currentSize = getSize();
        std::vector<std::pair<uint, int>> internal;
        internal.reserve(cellCount);
        
        int deltaHeight = std::floor(((float)newSize.height - (float)currentSize.height) / (float)cellCount);
        int heightDiff = newSize.height - ((deltaHeight*cellCount) + currentSize.height);
        
        int remainder = heightDiff % cellCount;
        int add = (heightDiff - remainder) + deltaHeight;
        
        std::vector<std::pair<uint, int>> cantGrow;
        for (uint i = 0; i < cellCount; ++i)
        {
             auto cell = at(i);
            Size cMaxSize = cell->getMaxSize();
            Size cSize = cell->getSize();
            if (((cMaxSize.height-cSize.height) < add) && (cMaxSize.height != -1))
                cantGrow.push_back({i, cMaxSize.height});
            
            internal.push_back({i, cSize.height + add});
        }
        
        // If one or more Cells cant grow.
        if (!cantGrow.empty())
        {
            PTK_INFO("CANT GROW {0}", cantGrow.size());
        }
        
        // If there are some pixels overflow we add some to the smallest Cells.
        while (remainder != 0)
        {
            std::sort(internal.begin(), internal.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
                return lhs.second < rhs.second;
            });

            for (uint i = 0; i < cellCount; ++i)
            {
                int cMaxHeight = at(i)->getMaxSize().height;
                if (((internal.at(i).second+1) <= cMaxHeight) || (cMaxHeight == -1))
                {
                    remainder--;
                    internal.at(i).second += 1;
                    if (remainder == 0)
                        break;
                }
            }
        }
        
        std::sort(internal.begin(), internal.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
            return lhs.first < rhs.first;
        });
        
        // Apply Sizes.
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
    
    void VBox::getSortedHeightDiffs(std::vector<std::pair<uint, int>>& data)
    {
        uint contentSize = size();
        data.reserve(contentSize);
        
        // Get height diffs on content.
        for (uint i = 0; i < contentSize; ++i)
        {
            Size cMinSize = at(i)->getMinSize();
            Size cMaxSize = at(i)->getMaxSize();
            
            if (cMaxSize.height != -1)
                data.push_back({i, cMaxSize.height - cMinSize.height});
            else
                data.push_back({i, -1});
        }
        
        // Sort with less.
        std::sort(data.begin(), data.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
            return lhs.second < rhs.second;
        });
        
        // Put all -1 at the end.
        // They have no limit on how much to grow (or shrink).
        uint j = 0;
        for (uint i = 0; i < contentSize; i++)
        {
            if (data.at(i).second > 0)
            {
                if (i != j)
                    std::swap(data.at(i), data.at(j));
                
                j++;
            }
        }
    }
    
    void VBox::applySizes(std::vector<std::pair<uint, int>>& data)
    {
        Position vPos = getPosition();
        Size vSize = getSize();
        
        // Sort with less. For starting at index zero.
        std::sort(data.begin(), data.end(), [](const std::pair<uint, int>& lhs, const std::pair<uint, int>& rhs){
            return lhs.first < rhs.first;
        });
        
        // Apply sizes.
        for (uint i = 0; i != data.size(); i++)
        {
            auto cell = at(i);
            cell->setSize(Size(vSize.width, data.at(i).second));
            cell->setPosHint(vPos);
            vPos.y += data.at(i).second;
        }
    }
    
    void VBox::addRemainder(std::vector<std::pair<uint, int>>& data, int32 remainder, int32 add)
    {
        uint contentSize = data.size();
        
        while (remainder != 0)
        {
            bool remainderAdded = false;
            for (uint i = 0; i < contentSize; ++i)
            {
                int cMaxHeight = at(i)->getMaxSize().height;
                if (((data.at(i).second + 1) <= cMaxHeight) || (cMaxHeight == -1))
                {
                    remainderAdded = true;
                    remainder--;
                    data.at(i).second += add;
                    
                    if (remainder == 0)
                        break;
                }
            }
            
            // If remainder cannot be added to the childs, we exit the loop.
            if (!remainderAdded)
            {
                PTK_WARN("Remainder could not be added, {0} left", remainder);
                break;
            }
        }
    }
}

