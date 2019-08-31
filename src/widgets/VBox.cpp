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

namespace pTK
{
    template<typename T>
    int arrayFind(T* data, uint size, const T& find, int start = 0)
    {
        for (int i = start; i < (int)size; ++i)
            if (data[i] == find)
                return i;
        
        return -1;
    }
    
    struct VBoxData
    {
        Position pos;
        Size size;
        Margin margin;
    };
        
    VBox::VBox()
        : Container(), m_data{}
    {
    }
    
    VBox::~VBox()
    {
        for (void* ptr : m_data)
            delete (VBoxData*)ptr;
    }
    
    bool VBox::add(const std::shared_ptr<Widget>& widget)
    {
        if (Container::add(widget))
        {
            m_data.push_back(new VBoxData{});
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
        Position vbPos = getPosition();
        uint children = size();
        
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
            cell->setPosHint(Position(vbPos.x + alignChildH(i, cSize), vbPos.y));
            vbPos.y += cSize.height + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
        }
    }
    
    void VBox::refitOnAdd(const Size& newSize)
    {
        Size layoutSize = newSize;
        Size vbSize = getSize();
        Position vbPos = getPosition();
        uint children = size();
        
        std::pair<uint, Size>* data = new std::pair<uint, Size>[children];
        PTK_ASSERT(data, "error obtaining memory on heap");
        
        // Get height diffs on content.
        int maxHeightAdd = 0;
        for (uint i = 0; i < children; ++i)
        {
            Size minSize = at(i)->getMinSize();
            Size maxSize = at(i)->getMaxSize();
            
            int width = maxSize.width - minSize.width;
            int height = maxSize.height - minSize.height;
            
            // Width
            if (maxSize.width == pTK::Auto)
                width = pTK::Auto;
            
            // Height
            if (maxSize.height == pTK::Auto)
                height = pTK::Auto;
            
            data[i] = std::make_pair(i, Size(width, height));
            maxHeightAdd += height;
        }
        
        // Sort by data[].second.height and put all pTK::Auto at the end.
        sortByHeight(data);
        
        // Expand children to its max sizes possible.
        int heightLeft  = vbSize.height - layoutSize.height;
        int eachAdd     = std::floor((float)heightLeft / (float)children);
        int totalAdd    = expandChildren(data, eachAdd, layoutSize);
        
        // Sort by data[].first
        sortByIndex(data);
        
        // Create Margins
        int spaces = children + 1;
        int* marginData = new int[spaces];
        PTK_ASSERT(marginData, "error obtaining memory on heap");
        int remainder       = (vbSize.height - ((eachAdd * children) + layoutSize.height)) % children;
        int unusedHeight    = remainder + totalAdd;
        createMargins(marginData, unusedHeight);
        
        // Set sizes to childs and spaces.
        for (uint i = 0; i != children; i++)
        {
            auto cell = at(i);
            cell->setSize(data[i].second);
            vbPos.y += marginData[i]; // Add margin
            cell->setPosHint(Position(vbPos.x + alignChildH(i, data[i].second), vbPos.y));
            vbPos.y += data[i].second.height; // Add margin
        }
        
        delete [] marginData;
        delete [] data;
    }
    
    void VBox::sortByHeight(std::pair<uint, Size>* data) const
    {
        uint children = size();
        
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
        uint children = size();
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
        uint children   = size();
        int eachAdd     = add;
        int totalAdd    = eachAdd * children;
        
        bool autoFound = false;
        for (uint i = 0; i < children; ++i)
        {
            int height = data[i].second.height;
            auto child = at(data[i].first);
            Size minSize = child->getMinSize();
            
            if (height == pTK::Auto)
            {
                if (!autoFound)
                {
                    eachAdd = std::floor((float)totalAdd / (float)(children - i));
                    autoFound = true;
                }
                
                // Child can grow.
                data[i].second.height = eachAdd;
                totalAdd -= eachAdd;
            }else if (height == 0)
            {
                // Child cannot grow.
                data[i].second.height = at(i)->getSize().height;
                eachAdd = std::floor((float)totalAdd / (float)(children - i));
            }else if (height < eachAdd)
            {
                // Child can grow a little.
                totalAdd -= height;
                data[i].second.height += minSize.height;
                eachAdd = std::floor((float)totalAdd / (float)(children - i));
            }else if (height >= eachAdd)
            {
                // Child can grow.
                data[i].second.height = minSize.height + eachAdd;
                totalAdd -= eachAdd;
            }
            
            Size maxSize = child->getMaxSize();
            if (maxSize.width != pTK::Auto)
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
    
    void VBox::createMargins(int* data, int unusedHeight)
    {
        uint children   = size();
        uint spaces     = children + 1;
        
        VBoxData* front = (VBoxData*)m_data.front();
        data[0] = front->margin.top;
        data[1] = front->margin.bottom;
        int marginLost = 0;
        
        for (uint i = 1; i < children; ++i)
        {
            VBoxData* childData = (VBoxData*)m_data.at(i);
            if ((data[i] != pTK::Auto) && (childData->margin.top != pTK::Auto))
                data[i] += childData->margin.top;
            else if (data[i] != pTK::Auto)
                data[i] = childData->margin.top;
            else
                marginLost += childData->margin.top;
            
            data[i+1] = childData->margin.bottom;
        }
        PTK_INFO("START");
        for (uint i = 0; i < spaces; ++i)
            PTK_INFO("{0}", data[i]);
        
        
        int marginAutoCount = 0;
        for (uint i = 0; i < spaces; ++i)
        {
            if (data[i] == pTK::Auto)
                ++marginAutoCount;
        }
        
        int marginLeft = unusedHeight + marginLost;
        int dist = std::floor((float)marginLeft / (float)marginAutoCount);
        
        for (uint i = 0; i < spaces; ++i)
            if (data[i] == pTK::Auto)
                data[i] = dist;
        
        PTK_INFO("END");
        for (uint i = 0; i < spaces; ++i)
            PTK_INFO("{0}", data[i]);
    }
    
    bool VBox::drawChild(Widget* widget)
    {
        // TODO: Check internal
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
            int vMargin = ((cMargin.top < 0) ? 0 : cMargin.top) + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
            int hMargin = ((cMargin.left < 0) ? 0 : cMargin.left) + ((cMargin.right < 0) ? 0 : cMargin.right);
            
            Size cMinSize = item->getMinSize();
            contentMinSize.height += (vMargin + cMinSize.height);
            contentMinSize.width = ((cMinSize.width + hMargin) > contentMinSize.width) ? cMinSize.width + hMargin : contentMinSize.width;
        });
        
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
        
        // TODO
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
        
        /**
         1 = top only.
         2 = bottom only.
         3 = top and bottom.
         */
        int* autoMarginData = new int[(cellCount*2)+1];
        PTK_ASSERT(autoMarginData, "error obtaining memory on heap");
        autoMarginData[0] = 1; // Size
        
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
            
            if ((cMargin.top == pTK::Auto) || (cMargin.bottom == pTK::Auto))
            {
                int pos = autoMarginData[0];
                autoMarginData[pos] = i;
                autoMarginData[0] = pos + 2;
                
                int margin = 0;
                if ((cMargin.top == pTK::Auto) && (cMargin.bottom == pTK::Auto))
                    margin = 3;
                else if (cMargin.top == pTK::Auto)
                    margin = 1;
                else if (cMargin.bottom == pTK::Auto)
                    margin = 2;
                    
                autoMarginData[pos+1] = margin;
            }
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
        
        PTK_INFO("Before");
        for (uint i = 0; i < spaceCount; ++i)
            PTK_INFO("{0}: {1}", i, spaceData[i]);
            
        if (autoMarginData[1] == 0)
            growSpacing(spaceData, spaceCount, unusedHeight);
        else
            growMargin(spaceData, spaceCount, autoMarginData, unusedHeight);
        
        PTK_INFO("After");
        for (uint i = 0; i < spaceCount; ++i)
            PTK_INFO("{0}: {1}", i, spaceData[i]);
        
        // Set sizes to childs and spaces.
        pos.y += spaceData[0];
        for (uint i = 0; i != cellCount; i++)
        {
            auto cell = at(i);
            Margin cMargin = cell->getMargin();
            cell->setSize(sizes[i]);
            pos.y += ((cMargin.top < 0) ? 0 : cMargin.top);
            cell->setPosHint(Position(pos.x + alignChildH(i, sizes[i]), pos.y));
            pos.y += sizes[i].height + spaceData[i+1] + ((cMargin.bottom < 0) ? 0 : cMargin.bottom);
        }
        
        delete [] sizes;
        delete [] spaceData;
        delete [] autoMarginData;
    }
    
    void VBox::growSpacing(int* spaceData, uint spaceCount, int32 height)
    {
        PTK_ASSERT(spaceData, "Undefined pointer");
        
        int spaceHeight = std::floor((float)height / (float)spaceCount);
        int spaceDiff = height - (spaceHeight * (int)spaceCount);
        int spaceRemainder = spaceDiff % spaceCount;
        int spaceAdd = (spaceDiff - spaceRemainder) + spaceHeight;
        
        for (uint i = 0; i < spaceCount; ++i)
            spaceData[i] += spaceAdd;
        
        growRemainder(spaceData, spaceCount, spaceRemainder);
    }
    
    void VBox::growMargin(int* spaceData, uint spaceCount, int* marginData, int32 height)
    {
        PTK_ASSERT(spaceData, "Undefined pointer");
        
        uint marginDataSize = (uint)marginData[0];
        uint marginCount = 0;
        
        for (uint i = 1; i < marginDataSize; i+=2)
        {
            int index = marginData[i];
            int cMargin = marginData[i + 1];
            
            if ((cMargin & 1) == 1)
            {
                if (i == 1)
                    marginCount++;
                else if (i > 2)
                {
                    int previousIndex = index - 1;
                    if (marginData[i - 2] == previousIndex)
                    {
                        if ((marginData[i - 1] & 2) == 2)
                            marginCount++;
                        
                    }else
                        marginCount++;
                }
            }
            
            if ((cMargin & 2) == 2)
            {
                if ((i + 2) < marginDataSize)
                {
                    int nextIndex = index + 1;
                    if (marginData[i + 2] == nextIndex)
                    {
                        if ((marginData[i + 3 ] & 2) == 2)
                            marginCount++;
                        
                    }else
                        marginCount++;
                }else
                {
                    marginCount++;
                }
            }
        }
        
        int spaceHeight = std::floor((float)height / (float)marginCount);
        int spaceDiff = height - (spaceHeight * (int)marginCount);
        int spaceRemainder = spaceDiff % spaceCount;
        int spaceAdd = (spaceDiff - spaceRemainder) + spaceHeight;
        
        for (uint i = 0; i < spaceCount; ++i)
        {
            if (i == 0)
            {
                if (arrayFind<int>(marginData, marginDataSize, 0, 1) != -1)
                {
                    auto child = at(0);
                    Margin cMargin = child->getMargin();
                    
                    if (cMargin.top == pTK::Auto)
                        spaceData[i] += spaceAdd;
                }
            } else if (i != (spaceCount - 1))
            {
                int index = arrayFind<int>(marginData, marginDataSize, (i - 1), 1);
                if (index != -1)
                {
                    auto child = at(marginData[index]);
                    Margin cMargin = child->getMargin();
                    if (cMargin.bottom == pTK::Auto)
                        spaceData[i] += spaceAdd;
                }
                
                index = arrayFind<int>(marginData, marginDataSize, i, 1);
                if (index != -1)
                {
                    auto child = at(marginData[index]);
                    Margin cMargin = child->getMargin();
                    if (cMargin.top == pTK::Auto)
                        spaceData[i] += spaceAdd;
                }
            } else if (i == (spaceCount - 1))
            {
                int index = arrayFind<int>(marginData, marginDataSize, (i - 1), 2);
                if (index != -1)
                {
                    auto child = at(marginData[index]);
                    Margin cMargin = child->getMargin();
                    if (cMargin.bottom == pTK::Auto)
                        spaceData[i] += spaceAdd;
                }
            }
        }
        
        while (spaceRemainder != 0)
        {
            // Find smallest
            int smallest = spaceData[marginData[2]];
            int location = marginData[2];
            for (uint i = 1;  i < spaceCount; ++i)
            {
                if (spaceData[i] < smallest)
                {
                    if (arrayFind<int>(marginData, marginDataSize, i, 1) != -1)
                    {
                        smallest = spaceData[i];
                        location = i;
                    }
                }
            }
            
            spaceData[location] += 1;
            spaceRemainder--;
        }
    }
    
    void VBox::growRemainder(int* spaceData, uint spaceCount, int32 remainder)
    {
        PTK_ASSERT(spaceData, "Undefined pointer");
        
        while (remainder != 0)
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
            remainder--;
        }
    }
    
    int VBox::growChilds(int totalHeight)
    {
        uint cellCount = size();
        int add = std::floor((float)totalHeight/(float)cellCount);
        int unusedHeight = totalHeight - (add * cellCount);
        
        if (add != 0)
        {
            Size* sizes = new Size[cellCount];
            PTK_ASSERT(sizes, "error obtaining memory on heap");
            
            bool run = true;
            while (run)
            {
                for (uint i = 0; i < cellCount; ++i)
                {
                    Size cSize = at(i)->getMinSize();
                    //if ()
                }
            }
        }
        
        return unusedHeight;
    }
    
    int VBox::alignChildH(uint index, const Size& childSize)
    {
        Size pSize = getSize();
        Margin cMargin = at(index)->getMargin();
        
        int posx = cMargin.left;
        if (posx == pTK::Auto)
        {
            if (cMargin.right == pTK::Auto)
            {
                posx += pSize.width/2;
                posx -= childSize.width/2;
            }else
            {
                posx = pSize.width - childSize.width;
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
            std::shared_ptr<Widget> child = at(i);
            
            data->pos = child->getPosition();
            data->size = child->getSize();
            data->margin = child->getMargin();
        }
    }
}

