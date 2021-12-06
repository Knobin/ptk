//
//  core/WidgetContainer.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

// pTK Headers
#include "ptk/core/WidgetContainer.hpp"

namespace pTK
{
    WidgetContainer::WidgetContainer()
        : IterableSequence<Ref<Widget>>(), Widget()
    {
        initCallbacks();
    }

    WidgetContainer::~WidgetContainer()
    {
        for (auto& item : *this)
            item->setParent(nullptr);
    }

    void WidgetContainer::add(const Ref<Widget>& widget)
    {
        if (std::find(cbegin(), cend(), widget) == cend())
        {
            container().push_back(widget);
            widget->setParent(this);
            onAdd(widget);
            draw();
        }
    }

    void WidgetContainer::remove(const Ref<Widget>& widget)
    {
        auto it = std::find(cbegin(), cend(), widget);
        if (it != cend())
        {
            widget->setParent(nullptr);
            onRemove(widget);
            container().erase(it);
            draw();
        }
    }

    void WidgetContainer::setPosHint(const Point& pos)
    {
        const Point deltaPos{pos - getPosition()};
        for (auto& item : *this)
        {
            Point wPos{item->getPosition()};
            wPos += deltaPos;
            item->setPosHint(wPos);
        }

        Widget::setPosHint(pos);
    }

    void WidgetContainer::onDraw(SkCanvas* canvas)
    {
        PTK_ASSERT(canvas, "Canvas is undefined");
        
        drawBackground(canvas);
        for (auto& item : *this)
            item->onDraw(canvas);
    }

    bool WidgetContainer::updateChild(Widget* widget)
    {
        if (!m_busy)
        {
            m_busy = true;
            WidgetContainer::const_iterator it{std::find_if(cbegin(), cend(), [&](WidgetContainer::const_reference item) {
                return item.get() == widget;
            })};
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

    bool WidgetContainer::drawChild(Widget* widget)
    {
        if (!m_busy)
        {
            m_busy = true;
            WidgetContainer::const_iterator it{std::find_if(cbegin(), cend(), [&](WidgetContainer::const_reference item) {
                return item.get() == widget;
            })};
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

    void WidgetContainer::onClickEvent(Mouse::Button btn, const Point& pos)
    {
        
        Widget *lastClicked{m_lastClickedWidget};
        bool found{false};
        
        for (auto& item : *this)
        {
            const Point startPos{item->getPosition()};
            const Size wSize{item->getSize()};
            const Point endPos{AddWithoutOverflow(startPos.x, static_cast<Point::value_type>(wSize.width)),
                                AddWithoutOverflow(startPos.y, static_cast<Point::value_type>(wSize.height))};
            
            if ((startPos.x <= pos.x) && (endPos.x >= pos.x))
            {
                if ((startPos.y <= pos.y) && (endPos.y >= pos.y))
                {
                    Widget* temp{item.get()}; // Iterator might change, when passing the event.

                    item->handleClickEvent(btn, pos);
                    m_lastClickedWidget = temp;
                    found = true;
                }
            }
        }

        if (lastClicked != nullptr && (lastClicked != m_lastClickedWidget || !found))
            lastClicked->handleLeaveClickEvent();

        if (!found)
            m_lastClickedWidget = nullptr;
    }

    void WidgetContainer::onReleaseEvent(Mouse::Button btn, const Point& pos)
    {
        if (m_lastClickedWidget != nullptr)
            m_lastClickedWidget->handleReleaseEvent(btn, pos);
    }

    void WidgetContainer::onKeyEvent(Event::Type type, KeyCode keycode, byte modifier)
    {
        if (m_lastClickedWidget != nullptr)
            m_lastClickedWidget->handleKeyEvent(type, keycode, modifier);
    }

    void WidgetContainer::onKeyInput(KeyCode keycode, uint32 data, Text::Encoding encoding, byte modifier)
    {
        if (m_lastClickedWidget != nullptr)
            m_lastClickedWidget->handleKeyInput(keycode, data, encoding, modifier);
    }

    void WidgetContainer::onHoverEvent(const Point& pos)
    {
        for (auto& it : *this)
        {
            const Point startPos{it->getPosition()};
            const Size wSize{it->getSize()};
            const Point endPos{AddWithoutOverflow(startPos.x, static_cast<Point::value_type>(wSize.width)),
                                AddWithoutOverflow(startPos.y, static_cast<Point::value_type>(wSize.height))};
            
            if ((startPos.x <= pos.x) && (endPos.x >= pos.x))
            {
                if ((startPos.y <= pos.y) && (endPos.y >= pos.y))
                {
                    // Send Leave Event.
                    if (m_currentHoverWidget != it.get() || m_currentHoverWidget == nullptr)
                    {
                        Widget* temp{it.get()}; // Iterator might change, when passing the event.

                        if (m_currentHoverWidget != nullptr)
                            m_currentHoverWidget->handleLeaveEvent();

                        // New current hovered Widget.
                        m_currentHoverWidget = temp;

                        // Fire Enter event on this and on to child.
                        handleEnterEvent();
                    }

                    m_currentHoverWidget->handleHoverEvent(pos);
                    return;
                }
            }
        }

        if (m_currentHoverWidget != nullptr)
            m_currentHoverWidget->handleLeaveEvent();

        // New current hovered Widget.
        m_currentHoverWidget = nullptr;
    }

    void WidgetContainer::onEnterEvent()
    {
        if (m_currentHoverWidget != nullptr)
            m_currentHoverWidget->handleEnterEvent();
    }

    void WidgetContainer::onLeaveEvent()
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleLeaveEvent();

            // Reset current hovered Widget.
            m_currentHoverWidget = nullptr;
        }
    }

    void WidgetContainer::onScrollEvent(const Vec2f& offset)
    {
        if (m_currentHoverWidget != nullptr)
            m_currentHoverWidget->handleScrollEvent(offset);
    }

    void WidgetContainer::setBackground(const Color& color)
    {
        m_background = color;
    }

    const Color& WidgetContainer::getBackground() const
    {
        return m_background;
    }

    Widget *WidgetContainer::getSelectedWidget() const
    {
        return m_lastClickedWidget;
    }

    bool WidgetContainer::busy() const
    {
        return m_busy;
    }

    void WidgetContainer::drawBackground(SkCanvas *canvas) const
    {
        PTK_ASSERT(canvas, "Canvas is undefined");
        
        // Set Size and Point
        const SkPoint pos{convertToSkPoint(getPosition())};
        SkPoint size{convertToSkPoint(getSize())};
        size += pos; // skia needs the size to be pos+size.

        // Set Color
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setARGB(m_background.a, m_background.r, m_background.g, m_background.b);

        // Draw Rect
        SkRect rect{};
        rect.set(pos, size);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        canvas->drawRoundRect(rect, 0, 0, paint);
    }

    void WidgetContainer::initCallbacks()
    {
        /*onKey([container = this](Event::Type type, KeyCode keycode){
            if (auto widget = container->m_currentHoverWidget)
                widget->handleKeyEvent(type, keycode);
            return false; // Do not remove callback, ever.
        });*/

        onLeaveClick([container = this](){
            if (container->m_lastClickedWidget != nullptr)
            {
                container->m_lastClickedWidget->handleLeaveClickEvent();
                container->m_lastClickedWidget = nullptr;
            }
            return false;
        });
    }

} // namespace pTK
