//
//  core/Cell.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

// Local Headers
#include "ptk/widgets/Cell.hpp"
#include "ptk/Core.hpp"

namespace pTK
{
    Cell::Cell()
        : Rectangle(), m_widget{nullptr}, m_widgetSize{}, m_widgetPos{}, m_clicked{false}, m_hover{false}
    {
    }
    
    Cell::Cell(const Ref<Widget>& widget)
        : m_widget{widget}, m_widgetSize{}, m_widgetPos{}, m_clicked{false}, m_hover{false}
    {
        PTK_ASSERT(widget, "Widget is nullptr");
        m_widgetSize = widget->getSize();
        m_widgetPos = widget->getPosition();
        widget->setParent(this);
        Widget::setSize(m_widgetSize);
        setMinSize(m_widgetSize);
    }

	Cell::~Cell()
	{
		if (m_widget)
			m_widget->setParent(nullptr);
	}
    
    void Cell::setWidget(const Ref<Widget>& widget)
    {
        PTK_ASSERT(widget, "Widget is nullptr");
        m_widget->setParent(nullptr);
        m_widget = widget;
        m_widgetSize = widget->getSize();
        m_widgetPos = widget->getPosition();
        widget->setParent(this);
    }
    
    Ref<Widget> Cell::getWidget() const
    {
        return m_widget;
    }
    
    void Cell::onDraw(SkCanvas* canvas)
    {
        Rectangle::onDraw(canvas);
        m_widget->onDraw(canvas);
    }
    
    bool Cell::drawChild(Widget* widget)
    {
        if (m_widget.get() == widget)
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
                {
                    setSize(wSize);
                    return true;
                }
            }
            
            draw();
            return true;
        }
        
        return false;
    }
    
    void Cell::setPosHint(const Point& pos)
    {
        Point currentPos = getPosition();
        Point deltaPos = pos - currentPos;
        
        Point wPos = m_widget->getPosition();
        wPos += deltaPos;
        m_widgetPos = wPos;
        m_widget->setPosHint(wPos);
        
        Widget::setPosHint(pos);
    }

    void Cell::onResize(const Size&)
    {
        calculatePosition();
    }
    
    void Cell::calculatePosition()
    {
        Size size = getSize();
        Point currentPos = getPosition();
        Size widgetSize = m_widget->getSize();
        currentPos += Point{size.width/2, size.height/2};
        currentPos -= Point{widgetSize.width/2, widgetSize.height/2};
        currentPos.x = (currentPos.x < 0) ? 0 : currentPos.x;
        currentPos.y = (currentPos.y < 0) ? 0 : currentPos.y;
        m_widgetPos = currentPos;
        m_widget->setPosHint(currentPos);
    }
    
    bool Cell::onClickEvent(Mouse::Button btn, const Point& pos)
    {
        Point wPos = m_widget->getPosition();
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
    
    bool Cell::onReleaseEvent(Mouse::Button btn, const Point& pos)
    {
        if (m_clicked)
        {
            m_widget->handleReleaseEvent(btn, pos);
            return true;
        }
        return false;
    }
    
    bool Cell::onKeyEvent(Event::Type type, int32 keycode)
    {
        if (m_clicked)
        {
            m_widget->handleKeyEvent(type, keycode);
            return true;
        }
        return false;
    }
    
    bool Cell::onHoverEvent(const Point& pos)
    {
        Point wPos = m_widget->getPosition();
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
            }else if (m_hover)
            {
                m_widget->handleLeaveEvent();
                m_hover = false;
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
