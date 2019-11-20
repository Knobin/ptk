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
// TODO: Fix Auto align for children...

// TODO: Maybe improve grow, so it doesn't need 3 allocations...

namespace pTK
{
    struct VBoxData
    {
        Point pos;
        Size size;
        Margin margin;
    };
        
    VBox::VBox()
        : Box(), m_data{}
    {
        
    }
    
    VBox::~VBox()
    {
        for (void* ptr : m_data)
            delete (VBoxData*)ptr;
    }
    
    bool VBox::add(const Ref<Widget>& widget)
    {
        if (Box::add(widget))
        {
            m_data.push_back(new VBoxData{});
            (static_cast<VBoxData*>(m_data.back()))->margin = widget->getMargin();
            internalUpdate();
            
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
                refitOnAdd(minLayoutSize);
            }
            
            internalUpdate();
            draw();
            
            return true;
        }
        
        return false;
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
            auto cell = at(i);
            
            Size cMinSize = cell->getMinSize();
            Size cSize = cell->getSize();
            if (cMinSize.height != -1)
                cSize.height = cMinSize.height;
            if (cMinSize.width != -1)
                cSize.width = cMinSize.width;
            
            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
             */
            cell->setSize(cSize);
            Margin cMargin = cell->getMargin();
            vbPos.y += ((cMargin.top < 0) ? 0 : cMargin.top);
            cell->setPosHint(Point(vbPos.x + alignChildH(i, newSize, cSize), vbPos.y));
            vbPos.y += cSize.height + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
        }
    }
    
    void VBox::refitOnAdd(const Size& newSize)
    {
        Size layoutSize = newSize;
        Size vbSize = getSize();
        Point vbPos = getPosition();
        size_t children = size();
        
        std::pair<uint, Size>* data = new std::pair<uint, Size>[children];
        PTK_ASSERT(data, "error obtaining memory on heap");
        
        // Get height diffs on content.
        for (uint i = 0; i < children; ++i)
        {
            Size minSize = at(i)->getMinSize();
            Size maxSize = at(i)->getMaxSize();
            
            int width = maxSize.width - minSize.width;
            int height = maxSize.height - minSize.height;
            
            // Width
            if (maxSize.width == pTK::Infinite)
                width = pTK::Infinite;
            
            // Height
            if (maxSize.height == pTK::Infinite)
                height = pTK::Infinite;
            
            data[i] = std::make_pair(i, Size(width, height));
        }
        
        // Sort by data[].second.height and put all pTK::Infinite at the end.
        sortByHeight(data);
        
        // Expand children to its max sizes possible.
        int heightLeft  = vbSize.height - layoutSize.height;
        int eachAdd     = static_cast<int>(std::floor(static_cast<float>(heightLeft) / static_cast<float>(children)));
        int totalAdd    = expandChildren(data, eachAdd, layoutSize);
        
        // Sort by data[].first
        sortByIndex(data);
        
        // Create Margins
        size_t spaces = children + 1;
        int* marginData = new int[spaces];
        PTK_ASSERT(marginData, "error obtaining memory on heap");
        int remainder       = (vbSize.height - ((eachAdd * static_cast<int>(children)) + layoutSize.height)) % static_cast<int>(children);
        int unusedHeight    = remainder + totalAdd;
        createMarginsOnAdd(marginData, unusedHeight);
        
        // Set sizes to childs and spaces.
        for (uint i = 0; i != children; i++)
        {
            auto cell = at(i);
            cell->setSize(data[i].second);
            vbPos.y += marginData[i]; // Add margin
            cell->setPosHint(Point(vbPos.x + alignChildH(i, vbSize, data[i].second), vbPos.y));
            vbPos.y += data[i].second.height; // Add margin
            
            if (i == 0)
            {
                cell->setMarginTop(marginData[0]);
            }
            
            if (i == (children - 1))
            {
                cell->setMarginBottom(marginData[i+1]);
            }else
            {
                VBoxData* top = (VBoxData*)m_data.at(i);
                VBoxData* bottom = (VBoxData*)m_data.at(i+1);
                
                if ((top->margin.bottom == pTK::Auto) && (bottom->margin.top == pTK::Auto))
                {
                    int temp = static_cast<int>(std::floor(static_cast<float>(marginData[i+1]) / 2.0f));
                    cell->setMarginBottom(temp);
                    at(i+1)->setMarginTop(marginData[i+1] - temp);
                }else if (top->margin.bottom == pTK::Auto)
                {
                    cell->setMarginBottom(marginData[i+1]);
                    at(i+1)->setMarginTop(0);
                }else if (bottom->margin.bottom == pTK::Auto)
                {
                    at(i+1)->setMarginTop(marginData[i+1]);
                    cell->setMarginBottom(0);
                }
            }
        }
        
        delete [] marginData;
        delete [] data;
    }
    
    void VBox::sortByHeight(std::pair<uint, Size>* data) const
    {
        size_t children = size();
        
        // Sort
        std::pair<uint, Size> temp;
        for (uint i = 0; i < children; ++i)
        {
            for (uint j = i+1; j < children; ++j)
            {
                if (data[i].second.height > data[j].second.height)
                {
                    temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
                }
            }
        }
        
        // Put -1 at end.
        uint j = 0;
        for (uint i = 0; i < children; i++)
        {
            if (data[i].second.height > 0)
            {
                if (i != j)
                {
                    temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
                }
                j++;
            }
        }
    }
    
    void VBox::sortByIndex(std::pair<uint, Size>* data) const
    {
        size_t children = size();
        std::pair<uint, Size> temp;
        
        for (uint i = 0; i < children; ++i)
        {
            for (uint j = i+1; j < children; ++j)
            {
                if (data[i].first > data[j].first)
                {
                    temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
                }
            }
        }
    }
    
    int VBox::expandChildren(std::pair<uint, Size>* data, int add, const Size& layoutSize)
    {
        size_t children = size();
        int eachAdd     = add;
        int totalAdd    = eachAdd * static_cast<int>(children);
        
        bool autoFound = false;
        for (uint i = 0; i < children; ++i)
        {
            int height = data[i].second.height;
            auto child = at(data[i].first);
            Size minSize = child->getMinSize();
            
            if (height == pTK::Infinite)
            {
                if (!autoFound)
                {
                    eachAdd = static_cast<int>(std::floor(static_cast<float>(totalAdd) / static_cast<float>((children - i))));
                    autoFound = true;
                }
                
                // Child can grow.
                data[i].second.height = eachAdd;
                totalAdd -= eachAdd;
            }else if (height == 0)
            {
                // Child cannot grow.
                data[i].second.height = child->getSize().height;
                eachAdd = static_cast<int>(std::floor(static_cast<float>(totalAdd) / static_cast<float>((children - i))));
            }else if (height < eachAdd)
            {
                // Child can grow a little.
                totalAdd -= height;
                data[i].second.height += minSize.height;
                eachAdd = static_cast<int>(std::floor(static_cast<float>(totalAdd) / static_cast<float>((children - i))));
            }else if (height >= eachAdd)
            {
                // Child can grow.
                data[i].second.height = minSize.height + eachAdd;
                totalAdd -= eachAdd;
            }
            
            Size maxSize = child->getMaxSize();
            if (maxSize.width != pTK::Infinite)
            {
                if (maxSize.width > layoutSize.width)
                    data[i].second.width = layoutSize.width;
                else
                    data[i].second.width = maxSize.width;
            }else
            {
                data[i].second.width = child->getSize().width;
            }
        }
        
        return totalAdd;
    }
    
    int VBox::readInternalMargins(int* data)
    {
        size_t children   = size();
        
        VBoxData* front = static_cast<VBoxData*>(m_data.front());
        data[0] = front->margin.top;
        data[1] = front->margin.bottom;
        int marginLost = 0;
        
        for (uint i = 1; i < children; ++i)
        {
            VBoxData* childData = static_cast<VBoxData*>(m_data.at(i));
            if ((data[i] != pTK::Auto) && (childData->margin.top != pTK::Auto))
                data[i] += childData->margin.top;
            else if (data[i] != pTK::Auto)
                data[i] = childData->margin.top;
            else if (childData->margin.top != pTK::Auto)
                marginLost += childData->margin.top;
            
            data[i+1] = childData->margin.bottom;
        }
        
        return marginLost;
    }
    
    void VBox::readMargins(int* data)
    {
        size_t children = size();
        
        auto first = front();
        data[0] = first->getMarginTop();
        data[1] = first->getMarginBottom();
        
        for (uint i = 1; i < children; ++i)
        {
            auto child = at(i);
            
            if ((data[i] != pTK::Auto) && (child->getMarginTop() != pTK::Auto))
                data[i] += child->getMarginTop();
            else if (data[i] != pTK::Auto)
                data[i] = child->getMarginTop();
            
            data[i+1] = child->getMarginBottom();
        }
    }
    
    void VBox::createMarginsOnAdd(int* data, int unusedHeight)
    {
        size_t children   = size();
        size_t spaces     = children + 1;
        
        int marginLost = readInternalMargins(data);
        
        int marginAutoCount = 0;
        for (uint i = 0; i < spaces; ++i)
            if (data[i] == pTK::Auto)
                ++marginAutoCount;
        
        int marginLeft = unusedHeight + marginLost;
        int dist = static_cast<int>(std::floor(static_cast<float>(marginLeft) / static_cast<float>(marginAutoCount)));
        
        if (dist != 0)
        {
            for (uint i = 0; i < spaces; ++i)
                if (data[i] == pTK::Auto)
                    data[i] = dist;
        }
        
        marginLeft -= (dist * marginAutoCount);
        if (marginLeft > 0)
        {
            for (uint i = 0; i < spaces; ++i)
            {
                if (data[i] == pTK::Auto)
                {
                    if (data[i] != 0)
                    {
                        data[i] += 1;
                        --marginLeft;
                    }else
                    {
                        data[i] = 0;
                    }
                }
            }
        }
    }
    
    void VBox::extendMargins(int* data, int unusedHeight)
    {
        size_t children   = size();
        size_t spaces     = children + 1;
        
        readInternalMargins(data);
        
        int marginAutoCount = 0;
        for (uint i = 0; i < spaces; ++i)
            if (data[i] == pTK::Auto)
                ++marginAutoCount;
            else
                data[i] = 0;
        
        int marginLeft = unusedHeight;// + marginLost;
        int dist = static_cast<int>(std::floor(static_cast<float>(marginLeft) / static_cast<float>(marginAutoCount)));
        
        for (uint i = 0; i < spaces; ++i)
            if (data[i] == pTK::Auto)
                data[i] = dist;
        
        marginLeft -= (dist * marginAutoCount);
        int* marginData;
        if (marginLeft > 0)
        {
            marginData = new int[spaces];
            PTK_ASSERT(marginData, "error obtaining memory on heap");
            readMargins(marginData);
        
            for (int i = 0; i < marginLeft; ++i)
            {
                size_t index = children;
                int min = 0;
                for (uint j = 0; j < spaces; ++j)
                {
                    if (min == 0)
                    {
                        index = j;
                        min = marginData[j];
                    }else if (marginData[j] != 0)
                    {
                        if (min > marginData[j])
                        {
                            index = j;
                            min = marginData[j];
                        }
                    }
                }
                
                marginData[index] += 1;
                data[index] += 1;
            }
            
            delete [] marginData;
        }
    }
    
    bool VBox::drawChild(Widget* widget)
    {
        if (findRaw(widget) != cend())
        {
            draw();
            return true;
        }
        
        return false;
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
        Size boxSize = getSize();
        int height = newSize.height - boxSize.height;
        int width = newSize.width - boxSize.width;
        
        if (size() > 0)
        {
            if ((height > 0) || (width > 0))
                grow(newSize);
            else if ((height < 0) || (width < 0))
                shrink(newSize);
        }
        
        Widget::setSize(newSize);
    }
    
    Size VBox::calculateMinSize() const
    {
        Size contentMinSize;
        for (uint i = 0; i < size(); ++i)
        {
            Margin cMargin = (static_cast<VBoxData*>(m_data.at(i)))->margin;
            int vMargin = ((cMargin.top < 0) ? 0 : cMargin.top) + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
            int hMargin = ((cMargin.left < 0) ? 0 : cMargin.left) + ((cMargin.right < 0) ? 0 : cMargin.right);
            
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
    
    void VBox::shrink(const Size& newSize)
    {
        // We should not really have to do anything with width.
        // The largest Cell in VBox should be the min size possible
        // for the parent of this.
        PTK_ASSERT((getMinSize().width <= newSize.width), "Exceeded min width of VBox");
        PTK_ASSERT((getMinSize().height <= newSize.height), "Exceeded min height of VBox");
        
        Size layoutSize = newSize;
        Size vbSize = getSize();
        const size_t children = size();
        
        std::pair<uint, Size>* data = new std::pair<uint, Size>[children];
        PTK_ASSERT(data, "error obtaining memory on heap");
        
        // Get height diffs on content.
        for (uint i = 0; i < children; ++i)
        {
            Size minSize = at(i)->getMinSize();
            Size size = at(i)->getSize();
            
            int width = size.width - minSize.width;
            int height = size.height - minSize.height;
            
            data[i] = std::make_pair(i, Size(width, height));
        }
        
        // Sort by data[].second.height and put all pTK::Infinite at the end.
        sortByHeight(data);
        
        // Expand children to its max sizes possible.
        int heightLeft  = vbSize.height - layoutSize.height;
        int eachSub     = static_cast<int>(std::floor(static_cast<float>(heightLeft) / static_cast<float>(children)));
        int totalSub    = shrinkChildren(data, eachSub, layoutSize);
        
        // Sort by data[].first
        sortByIndex(data);
        
        // Create Margins
        size_t spaces = children + 1;
        int* marginData = new int[spaces];
        PTK_ASSERT(marginData, "error obtaining memory on heap");
        int remainder       = (vbSize.height - ((eachSub * static_cast<int>(children)) + layoutSize.height)) % static_cast<int>(children);
        int unusedHeight    = remainder + totalSub;
        shrinkMargins(marginData, unusedHeight);    
            
        // Set sizes to childs and spaces.
        Point vbPos = getPosition();
        int hotfix = 0;
        for (uint i = 0; i != children; i++)
        {
            auto cell = at(i);
            cell->setSize(data[i].second);
            Point pos = cell->getPosition();
            hotfix += marginData[i];
            pos.y -= hotfix; // Add margin
            cell->setPosHint(Point(vbPos.x + alignChildH(i, newSize, data[i].second), pos.y));
            VBoxData* oldData = (VBoxData*)m_data.at(i);
            hotfix += (data[i].second.height < oldData->size.height) ? oldData->size.height - data[i].second.height : 0;
            
            if (i == 0)
            {
                cell->setMarginTop(cell->getMarginTop() - marginData[0]);
            }
            
            if (i == (children - 1))
            {
                cell->setMarginBottom(cell->getMarginBottom() - marginData[i+1]);
            }else
            {
                VBoxData* top = (VBoxData*)m_data.at(i);
                VBoxData* bottom = (VBoxData*)m_data.at(i+1);
                
                if ((top->margin.bottom == pTK::Auto) && (bottom->margin.top == pTK::Auto))
                {
                    int temp = static_cast<int>(std::floor((float)marginData[i+1] / 2.0f));
                    cell->setMarginBottom(cell->getMarginBottom() - temp);
                    at(i+1)->setMarginTop(at(i+1)->getMarginTop() - marginData[i+1] - temp);
                }else if (top->margin.bottom == pTK::Auto)
                {
                    cell->setMarginBottom(cell->getMarginBottom() - marginData[i+1]);
                    at(i+1)->setMarginTop(0);
                }else if (bottom->margin.bottom == pTK::Auto)
                {
                    at(i+1)->setMarginTop(at(i+1)->getMarginTop() - marginData[i+1]);
                    cell->setMarginBottom(0);
                }
            }
        }
        
        internalUpdate();
        
        delete [] marginData;
        delete [] data;
    }
    
    int VBox::shrinkChildren(std::pair<uint, Size>* data, int sub, const Size& layoutSize)
    {
        size_t children   = size();
        int eachSub     = sub;
        int totalSub    = eachSub * static_cast<int>(children);
        
        for (uint i = 0; i < children; ++i)
        {
            int height = data[i].second.height;
            auto child = at(data[i].first);
            Size size = child->getSize();
            
            if (height == 0)
            {
                // Child cannot grow.
                data[i].second.height = child->getSize().height;
                eachSub = static_cast<int>(std::floor(static_cast<float>(totalSub) / static_cast<float>((children - i))));
            }else if (height >= eachSub)
            {
                // Child can shrink.
                data[i].second.height = size.height - eachSub;
                totalSub -= eachSub;
            }else if (height < eachSub)
            {
                // Child can grow a little.
                totalSub -= height;
                data[i].second.height = size.height - height;
                eachSub = static_cast<int>(std::floor(static_cast<float>(totalSub) / static_cast<float>((children - i))));
            }
            
            if (size.width > layoutSize.width)
                data[i].second.width = layoutSize.width;
            else
                data[i].second.width = size.width;
        }
        
        return totalSub;
    }
    
    void VBox::shrinkMargins(int* data, int unusedHeight)
    {
        size_t children   = size();
        size_t spaces     = children + 1;
        
        readInternalMargins(data);
        
        int marginAutoCount = 0;
        for (uint i = 0; i < spaces; ++i)
            if (data[i] == pTK::Auto)
                ++marginAutoCount;
            else
                data[i] = 0;
        
        int marginLeft = unusedHeight;
        int dist = static_cast<int>(std::floor(static_cast<float>(marginLeft) / static_cast<float>(marginAutoCount)));
        
        for (uint i = 0; i < spaces; ++i)
            if (data[i] == pTK::Auto)
                data[i] = dist;
        
        marginLeft -= (dist * marginAutoCount);
        if ((marginLeft > 0) && (marginAutoCount != 0))
        {
            int* marginData = new int[spaces];
            PTK_ASSERT(marginData, "error obtaining memory on heap");
            readMargins(marginData);
            
            for (int i = 0; i < marginLeft; ++i)
            {
                uint index = 0;
                int max = marginData[index];
                for (uint j = 1; j < spaces; ++j)
                {
                    if ((marginData[j] > max) && (marginData[j] != 0))
                    {
                        max = marginData[j];
                        index = j;
                    }
                }
                
                marginData[index] -= 1;
                data[index] += 1;
            }
            
            delete [] marginData;
        }
    }
    
    void VBox::grow(const Size& newSize)
    {
        // We should not really have to do anything with width.
        // The smallest Cell in VBox should be the max size possible
        // for the parent of this.
        PTK_ASSERT(((getMaxSize().width <= newSize.width) && (getMaxSize().width == -1)), "Exceeded max width of VBox");
        PTK_ASSERT(((getMaxSize().height <= newSize.height) && (getMaxSize().height == -1)), "Exceeded max height of VBox");
        
        Size layoutSize = newSize;
        Size vbSize = getSize();
        const size_t children = size();
        
        std::pair<uint, Size>* data = new std::pair<uint, Size>[children];
        PTK_ASSERT(data, "error obtaining memory on heap");
        
        // Get height diffs on content.
        for (uint i = 0; i < children; ++i)
        {
            Size size = at(i)->getSize();
            Size maxSize = at(i)->getMaxSize();
            
            int width = maxSize.width - size.width;
            int height = maxSize.height - size.height;
            
            // Width
            if (maxSize.width == pTK::Infinite)
                width = pTK::Infinite;
            
            // Height
            if (maxSize.height == pTK::Infinite)
                height = pTK::Infinite;
            
            data[i] = std::make_pair(i, Size(width, height));
        }
        
        // Sort by data[].second.height and put all pTK::Infinite at the end.
        sortByHeight(data);
        
        // Expand children to its max sizes possible.
        int heightLeft  = layoutSize.height - vbSize.height;
        int eachAdd     = static_cast<int>(std::floor(static_cast<float>(heightLeft) / static_cast<float>(children)));
        int totalAdd    = expandChildren(data, eachAdd, layoutSize);
        
        // Sort by data[].first
        sortByIndex(data);
        
        // Create Margins
        size_t spaces = children + 1;
        int* marginData = new int[spaces];
        PTK_ASSERT(marginData, "error obtaining memory on heap");
        int remainder       = (layoutSize.height - ((eachAdd * static_cast<int>(children)) + vbSize.height)) % static_cast<int>(children);
        int unusedHeight    = remainder + totalAdd;
        extendMargins(marginData, unusedHeight);

        // Set sizes to childs and spaces.
        Point vbPos = getPosition();
        int hotfix = 0;
        for (uint i = 0; i != children; i++)
        {
            auto cell = at(i);
            cell->setSize(data[i].second);
            Point pos = cell->getPosition();
            hotfix += marginData[i];
            pos.y += hotfix; // Add margin
            cell->setPosHint(Point(vbPos.x + alignChildH(i, newSize, data[i].second), pos.y));
            VBoxData* oldData = (VBoxData*)m_data.at(i);
            pos.y += (data[i].second.height > oldData->size.height) ? data[i].second.height - oldData->size.height : 0;
            
            if (i == 0)
            {
                cell->setMarginTop(cell->getMarginTop() + marginData[0]);
            }
            
            if (i == (children - 1))
            {
                cell->setMarginBottom(cell->getMarginBottom() + marginData[i+1]);
            }else
            {
                VBoxData* top = (VBoxData*)m_data.at(i);
                VBoxData* bottom = (VBoxData*)m_data.at(i+1);
                
                if ((top->margin.bottom == pTK::Auto) && (bottom->margin.top == pTK::Auto))
                {
                    int temp = static_cast<int>(std::floor(static_cast<float>(marginData[i+1]) / 2.0f));
                    cell->setMarginBottom(cell->getMarginBottom() + temp);
                    at(i+1)->setMarginTop(at(i+1)->getMarginTop() + marginData[i+1] - temp);
                }else if (top->margin.bottom == pTK::Auto)
                {
                    cell->setMarginBottom(cell->getMarginBottom() + marginData[i+1]);
                    at(i+1)->setMarginTop(0);
                }else if (bottom->margin.bottom == pTK::Auto)
                {
                    at(i+1)->setMarginTop(at(i+1)->getMarginTop() + marginData[i+1]);
                    cell->setMarginBottom(0);
                }
            }
        }
        
        internalUpdate();
        
        delete [] marginData;
        delete [] data;
    }
    
    int VBox::alignChildH(uint index, const Size& parentSize, const Size& childSize)
    {
        Margin cMargin = ((VBoxData*)m_data.at(index))->margin;
        
        int posx = cMargin.left;
        if (posx == pTK::Auto)
        {
            if (cMargin.right == pTK::Auto)
            {
                posx += parentSize.width/2;
                posx -= childSize.width/2;
            }else
            {
                posx = parentSize.width - childSize.width;
            }
        }
        
        return posx;
    }
    
    void VBox::internalUpdate()
    {
        PTK_ASSERT(size() == m_data.size(), "Internal Size Error");
        
        for (uint i = 0; i < size(); ++i)
        {
            VBoxData* data = (VBoxData*)m_data.at(i);
            Ref<Widget> child = at(i);
            
            data->pos = child->getPosition();
            data->size = child->getSize();
        }
    }
}

