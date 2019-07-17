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
            m_background{0xf5f5f5ff}, m_lastClickedWidget{nullptr}, m_currentHoverWidget{nullptr}
    {
        
    }
    
    bool Box::add(const std::shared_ptr<Widget>& widget)
    {
        if (find(widget.get()) == cend())
        {
            std::shared_ptr<Cell> cell = std::make_shared<Cell>(widget);
            cell->setParent(this);
            push(cell);
            return true;
        }
        return false;
    }
    
    void Box::remove(const std::shared_ptr<Widget>& widget)
    {
        const_iterator it = find(widget.get());
        if (it != cend())
        {
            widget->setParent(nullptr);
            erase(it);
        }
    }
    
    Box::const_iterator Box::find(const std::string& name)
    {
        return findIf([name](const std::shared_ptr<Cell>& cell){
            if (cell->getWidget()->getName() == name)
                return true;
            
            return false;
        });
    }
    
    Box::const_iterator Box::find(const Widget* widget)
    {
        return findIf([widget](const std::shared_ptr<Cell>& cell){
            if (widget == cell.get())
                return true;
            
            return false;
        });
    }
    
    Box::const_iterator Box::find(const Position& pos)
    {
        return findIf([&pos](const std::shared_ptr<Cell>& cell){
            Position wPos = cell->getPosition();
            Size wSize = cell->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                    return true;
            
            return false;
        });
    }
    
    Box::const_reverse_iterator Box::rfind(const Position& pos)
    {
        return rfindIf([&pos](const std::shared_ptr<Cell>& cell){
            Position wPos = cell->getPosition();
            Size wSize = cell->getSize();
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
        for (iterator it = begin(); it != end(); it++)
            (*it)->onDraw(canvas);
    }
    
    bool Box::onClickEvent(MouseButton btn, const Position& pos)
    {
        for (auto it = begin(); it != end(); it++)
        {
            Position wPos = (*it)->getPosition();
            Size wSize = (*it)->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
            {
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                {
                    (*it)->handleClickEvent(btn, pos);
                    m_lastClickedWidget = (*it).get();
                    return true;
                }
            }
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
        for (auto it = begin(); it != end(); it++)
        {
            Position wPos = (*it)->getPosition();
            Size wSize = (*it)->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
            {
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                {
                    // Send Leave Event.
                    if (m_currentHoverWidget != (*it).get() || m_currentHoverWidget == nullptr)
                    {
                        if (m_currentHoverWidget != nullptr)
                            m_currentHoverWidget->handleLeaveEvent();
                        
                        // New current hovered Widget.
                        m_currentHoverWidget = (*it).get();
                        
                        // Fire Enter event on this and on to child.
                        handleEnterEvent();
                    }
                    
                    m_currentHoverWidget->handleHoverEvent(pos);
                    
                    return true;
                }
            }
        }
        
        if (m_currentHoverWidget != nullptr)
            m_currentHoverWidget->handleLeaveEvent();
        
        // New current hovered Widget.
        m_currentHoverWidget = nullptr;
        
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
            m_currentHoverWidget = nullptr;
            
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
    
    Size Box::calculateMinSize() const
    {
        return Size(-1.0f, -1.0f);
    }
    
    Size Box::calculateMaxSize() const
    {
        return Size(-1.0f, -1.0f);
    }
}

