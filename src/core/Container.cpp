//
//  core/Box.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-03.
//

// Local Headers
#include "ptk/core/Container.hpp"

namespace pTK
{
    Container::Container()
        : Widget(),
            m_holder{}, m_background{0xf5f5f5ff}, m_lastClickedWidget{nullptr},
            m_currentHoverWidget{nullptr}
    {
    }
    
    Container::Container(const type& item)
        : Widget(),
            m_holder{}, m_background{0xf5f5f5ff}, m_lastClickedWidget{nullptr},
            m_currentHoverWidget{nullptr}
    {
        add(item);
    }
    
    Container::Container(std::initializer_list<type> list)
        : Widget(),
            m_holder{}, m_background{0xf5f5f5ff}, m_lastClickedWidget{nullptr},
            m_currentHoverWidget{nullptr}
    {
        m_holder.reserve(list.size());
        for (const type& item : list)
            add(item);
    }
    
    Container::Container(const Container& rhs)
    {
        m_holder.reserve(rhs.size());
        for (auto it{rhs.cbegin()}; it != rhs.cend(); it++)
            add((*it));
    }
    
    Container::~Container()
    {
        forEach([](const Container::type& item){
            item->setParent(nullptr);
        });
    }
    
    Container& Container::operator=(const Container& rhs)
    {
        if (this == &rhs)
            return *this;
        
        m_holder.reserve(rhs.size());
        for (auto it{rhs.cbegin()}; it != rhs.cend(); it++)
            add((*it));
        
        return *this;
    }
    
    bool Container::add(const Ref<Widget>& widget)
    {
        if (find(widget.get()) == cend())
        {
            widget->setParent(this);
            m_holder.push_back(widget);
            return true;
        }
        return false;
    }
    
    void Container::remove(const Ref<Widget>& widget)
    {
        const_iterator it = find(widget.get());
        if (it != cend())
        {
            widget->setParent(nullptr);
            m_holder.erase(it);
        }
    }
    
    Container::type Container::at(unsigned int index)
    {
        if (index >= size())
            throw std::out_of_range("Index is out of range!");
        
        return m_holder.at(index);
    }
    
    Container::type Container::at(unsigned int index) const
    {
        if (index >= size())
            throw std::out_of_range("Index is out of range!");
        
        return m_holder.at(index);
    }
    
    Container::type Container::operator[](unsigned int index) const
    {
        if (index >= size())
            throw std::out_of_range("Index is out of range!");
        
        return m_holder[index];
    }
    
    Container::size_type Container::size() const
    {
        return m_holder.size();
    }
    
    bool Container::empty() const
    {
        return m_holder.empty();
    }
    
    Container::type Container::front() const
    {
        return m_holder.front();
    }
    
    Container::type Container::back() const
    {
        return m_holder.back();
    }
    
    Container::iterator Container::begin()
    {
        return m_holder.begin();
    }
    
    Container::iterator Container::end()
    {
        return m_holder.end();
        
    }
    
    Container::const_iterator Container::cbegin() const
    {
        return m_holder.cbegin();
    }
    
    Container::const_iterator Container::cend() const
    {
        return m_holder.cend();
        
    }
    
    Container::reverse_iterator Container::rbegin()
    {
        return m_holder.rbegin();
    }
    
    Container::reverse_iterator Container::rend()
    {
        return m_holder.rend();
    }
    
    Container::const_iterator Container::find(const std::string& name)
    {
        for (auto it{cbegin()}; it != cend(); ++it)
            if ((*it)->getName() == name)
                return it;
        
        return cend();
    }
    
    Container::const_iterator Container::find(const Widget* widget)
    {
        for (auto it{cbegin()}; it != cend(); ++it)
            if ((*it).get() == widget)
                return it;
        
        return cend();
    }
    
    Container::const_iterator Container::find(const Point& pos)
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
    
    Container::reverse_iterator Container::rfind(const Point& pos)
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
    
    void Container::forEach(const std::function<void(const Container::type& item)>& func) const
    {
        for (const_iterator it = cbegin(); it != cend(); it++)
            func((*it));
    }
    
    void Container::onDraw(SkCanvas* canvas)
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
        
        drawChildren(canvas);
    }
    
    void Container::drawChildren(SkCanvas* canvas)
    {
        for (iterator it = begin(); it != end(); it++)
            (*it)->onDraw(canvas);
    }
    
    bool Container::onClickEvent(MouseButton btn, const Point& pos)
    {
        for (auto it = begin(); it != end(); it++)
        {
            Point wPos = (*it)->getPosition();
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
    
    bool Container::onReleaseEvent(MouseButton btn, const Point& pos)
    {
        if (m_lastClickedWidget != nullptr)
        {
            m_lastClickedWidget->handleReleaseEvent(btn, pos);
            return true;
        }
        return false;
    }
    
    bool Container::onKeyEvent(EventType type, int32 keycode)
    {
        if (m_lastClickedWidget != nullptr)
        {
            m_lastClickedWidget->handleKeyEvent(type, keycode);
            return true;
        }
        return false;
    }
    
    bool Container::onHoverEvent(const Point& pos)
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
    
    bool Container::onEnterEvent()
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleEnterEvent();
            return true;
        }
        return false;
    }
    
    bool Container::onLeaveEvent()
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
    
    bool Container::onScrollEvent(const Vec2f& offset)
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleScrollEvent(offset);
            return true;
        }
        return false;
    }
    
    void Container::setBackground(const Color& color)
    {
        m_background = color;
    }
    
    const Color& Container::getBackground() const
    {
        return m_background;
    }
    
    Size Container::calculateMinSize() const
    {
        return Size(-1.0f, -1.0f);
    }
    
    Size Container::calculateMaxSize() const
    {
        return Size(-1.0f, -1.0f);
    }
}

