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

    bool WidgetContainer::onClickEvent(Mouse::Button btn, const Point& pos)
    {
        for (auto& item : *this)
        {
            const Point wPos{item->getPosition()};
            const Size wSize{item->getSize()};
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
            {
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
                {
                    Widget* temp{item.get()}; // Iterator might change, when passing the event.
                    bool status{item->handleClickEvent(btn, pos)};
                    m_lastClickedWidget = temp;
                    return status;
                }
            }
        }

        return false;
    }

    bool WidgetContainer::onReleaseEvent(Mouse::Button btn, const Point& pos)
    {
        if (m_lastClickedWidget != nullptr)
            return m_lastClickedWidget->handleReleaseEvent(btn, pos);

        return false;
    }

    bool WidgetContainer::onKeyEvent(Event::Type type, KeyCode keycode)
    {
        if (m_lastClickedWidget != nullptr)
            return m_lastClickedWidget->handleKeyEvent(type, keycode);

        return false;
    }

    bool WidgetContainer::onHoverEvent(const Point& pos)
    {
        for (auto& it : *this)
        {
            const Point wPos{it->getPosition()};
            const Size wSize{it->getSize()};
            if ((wPos.x <= pos.x) && (wPos.x + wSize.width >= pos.x))
            {
                if ((wPos.y <= pos.y) && (wPos.y + wSize.height >= pos.y))
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

    bool WidgetContainer::onEnterEvent()
    {
        if (m_currentHoverWidget != nullptr)
            return m_currentHoverWidget->handleEnterEvent();

        return false;
    }

    bool WidgetContainer::onLeaveEvent()
    {
        if (m_currentHoverWidget != nullptr)
        {
            bool status{m_currentHoverWidget->handleLeaveEvent()};

            // Reset current hovered Widget.
            m_currentHoverWidget = nullptr;

            return status;
        }
        return false;
    }

    bool WidgetContainer::onScrollEvent(const Vec2f& offset)
    {
        if (m_currentHoverWidget != nullptr)
            return m_currentHoverWidget->handleScrollEvent(offset);

        return false;
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

} // namespace pTK
