//
//  core/Box.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-03.
//

// Local Headers
#include "ptk/core/Box.hpp"

namespace pTK
{
    Box::Box()
    : ContainerType(), Widget(),
        m_layoutData{}, m_background{0xf5f5f5ff}, m_lastClickedWidget{nullptr}, m_currentHoverWidget{nullptr}
    {
        
    }
    
    bool Box::add(const std::shared_ptr<Widget>& widget)
    {
        if (ContainerType::find(widget) == end())
        {
            push(widget);
            m_layoutData.push({widget.get(), widget->getSize(), widget->getPosition()});
            widget->setParent(this);
            return true;
        }
        return false;
    }
    
    void Box::remove(const std::shared_ptr<Widget>& widget)
    {
        erase(widget);
        Container<layoutData>::const_iterator it = m_layoutData.findIf([widget](const layoutData& data){
            if (data.widget == widget.get())
                return true;
            return false;
        });
        m_layoutData.erase(it);
    }
    
    Box::const_iterator Box::find(const std::string& name)
    {
        return findIf([name](const std::shared_ptr<Widget>& widget){
            if (widget->getName() == name)
                return true;
            
            return false;
        });
    }
    
    Box::const_iterator Box::find(const Widget* widget)
    {
        return findIf([widget](const std::shared_ptr<Widget>& rhs){
            if (widget == rhs.get())
                return true;
            
            return false;
        });
    }
    
    Box::const_iterator Box::find(const Position& pos)
    {
        return findIf([&pos](const std::shared_ptr<Widget>& widget){
            Position wPos = widget->getPosition();
            Size wSize = widget->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                    return true;
            
            return false;
        });
    }
    
    Box::const_reverse_iterator Box::rfind(const Position& pos)
    {
        return rfindIf([&pos](const std::shared_ptr<Widget>& widget){
            Position wPos = widget->getPosition();
            Size wSize = widget->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                    return true;
            
            return false;
        });
    }
    
    void Box::onDraw(SkCanvas* canvas)
    {
        // Set Size and Position
        SkPoint pos{convertToSkPoint(getPosition())};
        SkPoint size{convertToSkPoint(getSize())};
        size += pos; // skia needs the size to be pos+size.
        
        // Set Color
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setARGB(m_background.a, m_background.r, m_background.g, m_background.b);
        
        // Draw Rect
        SkRect rect;
        rect.set(pos, size);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        canvas->drawRoundRect(rect, 0, 0, paint);
        
        drawWidgets(canvas);
    }
    
    void Box::drawWidgets(SkCanvas* canvas)
    {
        forEach([&canvas](const std::shared_ptr<Widget>& widget){
            widget->onDraw(canvas);
        });
    }
    
    bool Box::onClickEvent(MouseButton btn, const Position& pos)
    {
        auto it = rfind(pos);
        if (it != rend())
        {
            (*it)->handleClickEvent(btn, pos);
            m_lastClickedWidget.reset();
            m_lastClickedWidget = (*it);
            return true;
        }
        return false;
    }
    
    bool Box::onReleaseEvent(MouseButton btn, const Position& pos)
    {
        if (m_lastClickedWidget != nullptr)
        {
            m_lastClickedWidget->handleReleaseEvent(btn, pos);
            return true;
        }
        return false;
    }
    
    bool Box::onKeyEvent(EventType type, int32 keycode)
    {
        if (m_lastClickedWidget != nullptr)
        {
            m_lastClickedWidget->handleKeyEvent(type, keycode);
            return true;
        }
        return false;
    }
    
    bool Box::onHoverEvent(const Position& pos)
    {
        auto it = rfind(pos);
        if (it != rend())
        {
            // Send Leave Event.
            if (m_currentHoverWidget != (*it) || m_currentHoverWidget == nullptr)
            {
                if (m_currentHoverWidget != nullptr)
                    m_currentHoverWidget->handleLeaveEvent();
                
                // New current hovered Widget.
                m_currentHoverWidget.reset();
                m_currentHoverWidget = (*it);
                
                // Fire Enter event on this and on to child.
                handleEnterEvent();
            }
            
            m_currentHoverWidget->handleHoverEvent(pos);
            
            return true;
        }else
        {
            if (m_currentHoverWidget != nullptr)
                m_currentHoverWidget->handleLeaveEvent();
            
            // New current hovered Widget.
            m_currentHoverWidget.reset();
        }
        
        return false;
    }
    
    bool Box::onEnterEvent()
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleEnterEvent();
            return true;
        }
        return false;
    }
    
    bool Box::onLeaveEvent()
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleLeaveEvent();
            
            // Reset current hovered Widget.
            m_currentHoverWidget.reset();
            
            return true;
        }
        return false;
    }
    
    bool Box::onScrollEvent(const Vec2f& offset)
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleScrollEvent(offset);
            return true;
        }
        return false;
    }
    
    void Box::setBackground(const Color& color)
    {
        m_background = color;
    }
    
    const Color& Box::getBackground() const
    {
        return m_background;
    }
}

