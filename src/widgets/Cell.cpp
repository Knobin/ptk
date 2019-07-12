//
//  core/Cell.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

// Local Headers
#include "ptk/widgets/Cell.hpp"
#include "ptk/Log.hpp"

namespace pTK
{
    Cell::Cell()
        : m_widget{nullptr}, m_widgetSize{}, m_widgetPos{}, m_clicked{false}, m_hover{false}
    {
    }
    
    Cell::Cell(const std::shared_ptr<Widget>& widget)
        : m_widget{widget}, m_widgetSize{}, m_widgetPos{}, m_clicked{false}, m_hover{false}
    {
        PTK_ASSERT(widget, "Widget is nullptr");
        m_widgetSize = widget->getSize();
        m_widgetPos = widget->getPosition();
        widget->setParent(this);
        Widget::setSize(m_widgetSize);
        setMinSize(m_widgetSize);
    }
    
    void Cell::setWidget(const std::shared_ptr<Widget>& widget)
    {
        PTK_ASSERT(widget, "Widget is nullptr");
        m_widget->setParent(nullptr);
        m_widget = widget;
        m_widgetSize = widget->getSize();
        m_widgetPos = widget->getPosition();
        widget->setParent(this);
    }
    
    std::shared_ptr<Widget> Cell::getWidget() const
    {
        return m_widget;
    }
    
    bool Cell::drawChild(Widget* widget)
    {
        // Position
        if (widget->getPosition() != m_widgetPos)
            widget->setPosHint(m_widgetPos);
        
        // Size
        Size wSize = widget->getSize();
        if (wSize != m_widgetSize)
        {
            m_widgetSize = wSize;
            Size cellSize = getSize();
            if ((wSize.width > cellSize.width) || (wSize.height > cellSize.height))
                setSize(wSize);
            
            return true;
        }else
        {
            draw();
            return true;
        }
        
        return false;
    }
    
    void Cell::setPosHint(const Position& pos)
    {
        Position currentPos = getPosition();
        Position deltaPos = pos - currentPos;
        
        Position wPos = m_widget->getPosition();
        wPos += deltaPos;
        m_widgetPos = wPos;
        m_widget->setPosHint(wPos);
        
        Widget::setPosHint(pos);
    }
    
    void Cell::setSize(const Size& size)
    {
        // Set widget position. ( TODO: Align)
        // We align center default.
        Widget::setSize(size);
        calculatePosition();
    }
    
    void Cell::calculatePosition()
    {
        Size size = getSize();
        Position currentPos = getPosition();
        Size widgetSize = m_widget->getSize();
        currentPos += Position{size.width/2, size.height/2};
        currentPos -= Position{widgetSize.width/2, widgetSize.height/2};
        currentPos.x = (currentPos.x < 0) ? 0 : currentPos.x;
        currentPos.y = (currentPos.y < 0) ? 0 : currentPos.y;
        m_widget->setPosHint(currentPos);
    }
    
    bool Cell::onClickEvent(MouseButton btn, const Position& pos)
    {
        Position wPos = m_widget->getPosition();
        Size wSize = m_widget->getSize();
        if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
        {
            if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
            {
                m_widget->handleClickEvent(btn, pos);
                m_clicked = true;
                return true;
            }
        }
        m_clicked = false;
        return false;
    }
    
    bool Cell::onReleaseEvent(MouseButton btn, const Position& pos)
    {
        if (m_clicked)
        {
            m_widget->handleReleaseEvent(btn, pos);
            return true;
        }
        return false;
    }
    
    bool Cell::onKeyEvent(EventType type, int32 keycode)
    {
        if (m_clicked)
        {
            m_widget->handleKeyEvent(type, keycode);
            return true;
        }
        return false;
    }
    
    bool Cell::onHoverEvent(const Position& pos)
    {
        Position wPos = m_widget->getPosition();
        Size wSize = m_widget->getSize();
        if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
        {
            if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
            {
                if (!m_hover)
                    m_widget->handleEnterEvent();
                
                m_widget->handleHoverEvent(pos);
                m_hover = true;
                return true;
            }
        }else if (m_hover)
        {
            m_widget->handleLeaveEvent();
            m_hover = false;
            return true;
        }
        
        m_hover = false;
        return false;
    }
    
    bool Cell::onEnterEvent()
    {
        if (m_hover)
        {
            m_widget->handleEnterEvent();
            return true;
        }
        return false;
    }
    
    bool Cell::onLeaveEvent()
    {
        if (m_hover)
        {
            m_widget->handleLeaveEvent();
            m_hover = false;
            return true;
        }
        return false;
    }
    
    bool Cell::onScrollEvent(const Vec2f& offset)
    {
        if (m_hover)
        {
            m_widget->handleScrollEvent(offset);
            return true;
        }
        return false;
    }
}
