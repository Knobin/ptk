//
//  core/Box.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

// Local Headers
#include "ptk/core/Box.hpp"

namespace pTK
{
    Box::Box()
        : Container(), Widget(),
            m_background{0xf5f5f5ff}, m_lastClickedWidget{nullptr},
            m_currentHoverWidget{nullptr}, m_busy{false}
    {
    }
    
    Box::~Box()
    {
        forEach([](const Container<Ref<Widget>>::type& item){
            item->setParent(nullptr);
        });
    }
    
    bool Box::add(const Ref<Widget>& widget)
    {
        if (Container::find(widget) == cend())
        {
            widget->setParent(this);
            Container<Ref<Widget>>::add(widget);
            onAdd(widget);
            draw();
            return true;
        }
        return false;
    }
    
    void Box::remove(const Ref<Widget>& widget)
    {
        const_iterator it = Container::find(widget);
        if (it != cend())
        {
            widget->setParent(nullptr);
            Container<Ref<Widget>>::remove(widget);
            onRemove(widget);
            draw();
        }
    }

    void Box::setPosHint(const Point& pos)
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
    
    void Box::onDraw(SkCanvas* canvas)
    {
        // Set Size and Point
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
        
        for (auto it = begin(); it != end(); ++it)
            (*it)->onDraw(canvas);
    }

    bool Box::updateChild(Widget* widget)
    {
        if (!m_busy)
        {
            m_busy = true;
            Box::const_iterator it = findRaw(widget);
            if (it != cend())
            {
                onChildUpdate(static_cast<size_type>(it - cbegin()));
                draw();
                m_busy = false;
                return true;
            }
        }
        
        return false;
    }
    
    bool Box::drawChild(Widget* widget)
    {
        if (!m_busy)
        {
            m_busy = true;
            Box::const_iterator it = findRaw(widget);
            if (it != cend())
            {
                onChildDraw(static_cast<size_type>(it - cbegin()));
                draw();
                m_busy = false;
                return true;
            }
        }

        return false;
    }

    Container<Ref<Widget>>::const_iterator Box::findRaw(const Widget* widget)
    {
        for (auto it{cbegin()}; it != cend(); ++it)
            if ((*it).get() == widget)
                return it;
        
        return cend();
    }
    
    Container<Ref<Widget>>::const_iterator Box::find(const Point& pos)
    {
        for (auto it{cbegin()}; it != cend(); ++it)
        {
            Point wPos = (*it)->getPosition();
            Size wSize = (*it)->getSize();
            
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                    return it;
        }
        
        return cend();
    }
    
    Container<Ref<Widget>>::reverse_iterator Box::rfind(const Point& pos)
    {
        for (auto it{rbegin()}; it != rend(); ++it)
        {
            Point wPos = (*it)->getPosition();
            Size wSize = (*it)->getSize();
            
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                    return it;
        }
        
        return rend();
    }
    
    bool Box::onClickEvent(Mouse::Button btn, const Point& pos)
    {
        for (auto it = begin(); it != end(); it++)
        {
            Point wPos = (*it)->getPosition();
            Size wSize = (*it)->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
            {
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                {
                    Widget *temp = (*it).get(); // Iterator might change, when passing the event.
                    bool status = (*it)->handleClickEvent(btn, pos);
                    m_lastClickedWidget = temp;
                    return status;
                }
            }
        }
        
        return false;
    }
    
    bool Box::onReleaseEvent(Mouse::Button btn, const Point& pos)
    {
        if (m_lastClickedWidget != nullptr)
            return m_lastClickedWidget->handleReleaseEvent(btn, pos);

        return false;
    }
    
    bool Box::onKeyEvent(Event::Type type, int32 keycode)
    {
        if (m_lastClickedWidget != nullptr)
            return m_lastClickedWidget->handleKeyEvent(type, keycode);

        return false;
    }
    
    bool Box::onHoverEvent(const Point& pos)
    {
        for (auto it = begin(); it != end(); it++)
        {
            Point wPos = (*it)->getPosition();
            Size wSize = (*it)->getSize();
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
            {
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                {
                    // Send Leave Event.
                    if (m_currentHoverWidget != (*it).get() || m_currentHoverWidget == nullptr)
                    {
                        Widget *temp = (*it).get(); // Iterator might change, when passing the event.

                        if (m_currentHoverWidget != nullptr)
                            m_currentHoverWidget->handleLeaveEvent();
                        
                        // New current hovered Widget.
                        m_currentHoverWidget = temp;
                        
                        // Fire Enter event on this and on to child.
                        handleEnterEvent();
                    }
                    
                    return m_currentHoverWidget->handleHoverEvent(pos);
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
            return m_currentHoverWidget->handleEnterEvent();
        
        return false;
    }
    
    bool Box::onLeaveEvent()
    {
        if (m_currentHoverWidget != nullptr)
        {
            bool status = m_currentHoverWidget->handleLeaveEvent();
            
            // Reset current hovered Widget.
            m_currentHoverWidget = nullptr;
            
            return status;
        }
        return false;
    }
    
    bool Box::onScrollEvent(const Vec2f& offset)
    {
        if (m_currentHoverWidget != nullptr)
            return m_currentHoverWidget->handleScrollEvent(offset);;
        
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

    bool Box::busy() const
    {
        return m_busy;
    }
}
