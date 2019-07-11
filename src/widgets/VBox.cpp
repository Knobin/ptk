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
            Size size = getSize();
            Position pos = getPosition();
            uint cellCount = m_layoutData.size();
            Size cellSize = {size.width, (size.height/cellCount)};
            
            Size minLayoutSize{};
            m_layoutData.forEach([&minLayoutSize](const layoutData& data){
                minLayoutSize.height += data.size.height;
                minLayoutSize.width = (data.size.width > minLayoutSize.width) ? data.size.width : minLayoutSize.width;
            });
            
            // Widgets wont fit, resize VBox.
            if ((minLayoutSize.width > size.width) || (minLayoutSize.height > size.height))
            {
                setSize(minLayoutSize); // this will generate a Resize event.
                for (auto it = m_layoutData.begin(); it != m_layoutData.end(); it++)
                {
                    (*it).widget->setPosHint(pos);
                    (*it).pos = pos;
                    pos.y += (*it).size.height;
                }
                
                return true;
            }
            
            // minCellSize required
            Size minCell{};
            m_layoutData.forEach([&minCell](const layoutData& data){
                minCell.height = (data.size.height > minCell.height) ? data.size.height : minCell.height;
                minCell.width = (data.size.width > minCell.width) ? data.size.width : minCell.width;
            });
            
            if ((minCell.width > cellSize.width) || (minCell.height > cellSize.height))
            {
                Size newSize{minCell.width, minCell.height*cellCount};
                setSize(newSize); // this will generate a Resize event.
                Position wPos{pos};
                for (auto it = m_layoutData.begin(); it != m_layoutData.end(); it++)
                {
                    Position offset{(minCell.width/2), (minCell.height/2)};
                    offset.x -= ((*it).size.width/2);
                    offset.y -= ((*it).size.height/2);
                    Position newWidgetPosition = wPos + offset;
                    (*it).widget->setPosHint(newWidgetPosition);
                    (*it).pos = newWidgetPosition;
                    pos.y += minCell.height;
                }
                
                return true;
            }
            
            // TODO: No resize needed. only repositioning.
            for (auto it = m_layoutData.begin(); it != m_layoutData.end(); it++)
            {
                Position offset{(cellSize.width/2), (cellSize.height/2)};
                offset.x -= ((*it).size.width/2);
                offset.y -= ((*it).size.height/2);
                Position newWidgetPosition{pos + offset};
                (*it).widget->setPosHint(newWidgetPosition);
                (*it).pos = newWidgetPosition;
                pos.y += cellSize.height;
            }
            
            draw();
            return true;
        }
        
        return false;
    }
    
    bool VBox::drawChild(Widget* widget)
    {
        Container<layoutData>::iterator it = m_layoutData.findIf([widget](const layoutData& data){
            if (data.widget == widget)
                return true;
            return false;
        });
        if (it != m_layoutData.end())
        {
            // Position
            if (widget->getPosition() != it->pos)
                widget->setPosHint(it->pos);
            
            // Size
            Size wSize = widget->getSize();
            if (wSize != it->size)
            {
                it->size = wSize;
                Size newSize;
                for (const layoutData& data : m_layoutData)
                {
                    newSize.width = (data.size.width > newSize.width) ? data.size.width : newSize.width;
                    newSize.height += data.size.height;
                }
                setSize(newSize);
            }else
            {
                draw();
            }
            
            return true;
        }
        
        return false;
    }
    
    void VBox::setPosHint(const Position& pos)
    {
        Position currentPos = getPosition();
        Position deltaPos = pos - currentPos;
        
        for (auto it = m_layoutData.begin(); it != m_layoutData.end(); it++)
        {
            Position wPos = it->widget->getPosition();
            wPos += deltaPos;
            it->pos = wPos;
            it->widget->setPosHint(wPos);
        }
        
        Widget::setPosHint(pos);
    }
}

