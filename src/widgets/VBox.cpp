//
//  widgets/VBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

// Local Headers
#include "ptk/widgets/VBox.hpp"
#include "ptk/Log.hpp"

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
            Size cellSize = {boxSize.width, (boxSize.height/cellCount)};
            
            Size minLayoutSize{};
            forEach([&minLayoutSize](const std::shared_ptr<Cell>& cell){
                Size cSize = cell->getMinSize();
                minLayoutSize.height += cSize.height;
                minLayoutSize.width = (cSize.width > minLayoutSize.width) ? cSize.width : minLayoutSize.width;
            });
            
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
            
            // Set last inserted size. (it is smaller than other cells)
            // No resize needed. only repositioning.
            //float delta = minLayout
            float delta = (boxSize.height - minLayoutSize.height) / cellCount;
            for (auto it = begin(); it != end(); it++)
            {
                Size wMinSize = (*it)->getMinSize();
                (*it)->setSize(Size(boxSize.width, wMinSize.height+delta));
                (*it)->setPosHint(pos);
                pos.y += (*it)->getSize().height;
            }
            
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
        uint cellCount = size();
        Size currentBoxSize = getSize();
        Size delta = (newSize - currentBoxSize);
        delta.height /= cellCount;
        
        float height = 0;
        for (auto it = begin(); it != end(); it++)
        {
            Size wSize = (*it)->getSize();
            (*it)->setSize(Size(newSize.width, (wSize.height+delta.height)));
            Position wPos = (*it)->getPosition();
            wPos.y += height;
            (*it)->setPosHint(wPos);
            height += delta.height;
        }
        
        Widget::setSize(newSize);
    }
}

