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
        : Widget()
    {
        setSizePolicy(SizePolicy::Type::Expanding);
        initCallbacks();
    }

    WidgetContainer::~WidgetContainer()
    {
        for (auto& item : *this)
            item->setParent(nullptr);
    }

    void WidgetContainer::add(const Ref<Widget>& widget)
    {
        auto it = std::find(m_holder.cbegin(), m_holder.cend(), widget);

        if (it == m_holder.cend())
        {
            m_holder.push_back(widget);
            widget->setParent(this);
            onAdd(widget);
            draw();
        }
    }

    void WidgetContainer::remove(const Ref<Widget>& widget)
    {
        auto it = std::find(m_holder.cbegin(), m_holder.cend(), widget);

        if (it != m_holder.cend())
        {
            widget->setParent(nullptr);
            if (m_currentHoverWidget == widget.get())
                m_currentHoverWidget = nullptr;
            if (m_lastClickedWidget == widget.get())
                m_lastClickedWidget = nullptr;

            onRemove(widget);
            m_holder.erase(it);
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
        drawChildren(canvas);
    }

    bool WidgetContainer::updateChild(Widget* widget)
    {
        if (!m_busy)
        {
            m_busy = true;

            auto it = std::find_if(m_holder.cbegin(), m_holder.cend(), [&widget](const auto& entry) {
                return entry.get() == widget;
            });

            if (it != m_holder.cend())
            {
                onChildUpdate(static_cast<size_type>(it - m_holder.cbegin()));
                draw();
                m_busy = false;
                return true;
            }
            m_busy = false;
        }

        return false;
    }

    bool WidgetContainer::drawChild(Widget* widget)
    {
        if (!m_busy)
        {
            m_busy = true;
            
            auto it = std::find_if(m_holder.cbegin(), m_holder.cend(), [&widget](const auto& entry) {
                return entry.get() == widget;
             });

            if (it != m_holder.cend())
            {
                onChildDraw(static_cast<size_type>(it - m_holder.cbegin()));
                draw();
                m_busy = false;
                return true;
            }
            m_busy = false;
        }

        return false;
    }

    void WidgetContainer::onClickCallback(const ClickEvent& evt)
    {
        Widget *lastClicked{m_lastClickedWidget};
        bool found{false};
        const Point& pos{evt.pos};

        for (std::size_t i{0}; i < m_holder.size(); ++i)
        {
            const auto& item = m_holder.at(i);

            const Point startPos{item->getPosition()};
            const Size wSize{item->getSize()};
            const Point endPos{AddWithoutOverflow(startPos.x, static_cast<Point::value_type>(wSize.width)),
                                AddWithoutOverflow(startPos.y, static_cast<Point::value_type>(wSize.height))};

            if ((startPos.x <= pos.x) && (endPos.x >= pos.x))
            {
                if ((startPos.y <= pos.y) && (endPos.y >= pos.y))
                {
                    Widget* temp{item.get()}; // Iterator might change, when passing the event.
                    m_lastClickedWidget = temp;
                    found = true;
                    item->handleEvent<ClickEvent>(evt);
                    break;
                }
            }
        }

        if (lastClicked != nullptr && (lastClicked != m_lastClickedWidget || !found))
        {
            LeaveClickEvent lcEvent{evt.button, evt.pos};
            lastClicked->handleEvent<LeaveClickEvent>(lcEvent);
        }

        if (!found)
            m_lastClickedWidget = nullptr;
    }

    void WidgetContainer::onReleaseCallback(const ReleaseEvent& evt)
    {
        if (m_lastClickedWidget != nullptr)
            m_lastClickedWidget->handleEvent<ReleaseEvent>(evt);
    }

    void WidgetContainer::onKeyCallback(const KeyEvent& evt)
    {
        if (m_lastClickedWidget != nullptr)
            m_lastClickedWidget->handleEvent<KeyEvent>(evt);
    }

    void WidgetContainer::onInputCallback(const InputEvent& evt)
    {
        if (m_lastClickedWidget != nullptr)
            m_lastClickedWidget->handleEvent<InputEvent>(evt);
    }

    void WidgetContainer::onHoverCallback(const MotionEvent& evt)
    {
        const Point& pos{evt.pos};

        for (std::size_t i{0}; i < m_holder.size(); ++i)
        {
            const auto& item = m_holder.at(i);

            const Point startPos{item->getPosition()};
            const Size wSize{item->getSize()};
            const Point endPos{AddWithoutOverflow(startPos.x, static_cast<Point::value_type>(wSize.width)),
                                AddWithoutOverflow(startPos.y, static_cast<Point::value_type>(wSize.height))};

            if ((startPos.x <= pos.x) && (endPos.x >= pos.x))
            {
                if ((startPos.y <= pos.y) && (endPos.y >= pos.y))
                {
                    // Send Leave Event.
                    if (m_currentHoverWidget != item.get() || m_currentHoverWidget == nullptr)
                    {
                        Widget* temp{item.get()}; // Iterator might change, when passing the event.

                        if (m_currentHoverWidget != nullptr)
                        {
                            LeaveEvent lEvent{evt.pos};
                            m_currentHoverWidget->handleEvent<LeaveEvent>(lEvent);
                        }

                        // New current hovered Widget.
                        m_currentHoverWidget = temp;

                        // Fire Enter event on this and on to child.
                        EnterEvent entEvent{evt.pos};
                        m_currentHoverWidget->handleEvent<EnterEvent>(entEvent);
                    }

                    m_currentHoverWidget->handleEvent<MotionEvent>(evt);
                    return;
                }
            }
        }

        if (m_currentHoverWidget != nullptr)
        {
            LeaveEvent lEvent{evt.pos};
            m_currentHoverWidget->handleEvent<LeaveEvent>(lEvent);
        }

        // New current hovered Widget.
        m_currentHoverWidget = nullptr;
    }

    void WidgetContainer::onEnterCallback(const EnterEvent& evt)
    {
        if (m_currentHoverWidget != nullptr)
            m_currentHoverWidget->handleEvent<EnterEvent>(evt);
    }

    void WidgetContainer::onLeaveCallback(const LeaveEvent& evt)
    {
        if (m_currentHoverWidget != nullptr)
        {
            m_currentHoverWidget->handleEvent<LeaveEvent>(evt);

            // Reset current hovered Widget.
            m_currentHoverWidget = nullptr;
        }
    }

    void WidgetContainer::onScrollCallback(const ScrollEvent& evt)
    {
        if (m_currentHoverWidget != nullptr)
            m_currentHoverWidget->handleEvent<ScrollEvent>(evt);
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

        addListener<KeyEvent>([&](const KeyEvent& evt) { onKeyCallback(evt); return false; });
        addListener<InputEvent>([&](const InputEvent& evt) { onInputCallback(evt); return false; });
        addListener<MotionEvent>([&](const MotionEvent& evt) { onHoverCallback(evt); return false; });
        addListener<EnterEvent>([&](const EnterEvent& evt) { onEnterCallback(evt); return false; });
        addListener<LeaveEvent>([&](const LeaveEvent& evt) { onLeaveCallback(evt); return false; });
        // addListener<LeaveClickEvent>([&](const LeaveClickEvent& evt) { onLeaveClickEvent(evt); return false; });
        addListener<ScrollEvent>([&](const ScrollEvent& evt) { onScrollCallback(evt); return false; });
        addListener<ClickEvent>([&](const ClickEvent& evt) { onClickCallback(evt); return false; });
        addListener<ReleaseEvent>([&](const ReleaseEvent& evt) { onReleaseCallback(evt); return false; });


        addListener<LeaveClickEvent>([container = this](const LeaveClickEvent& evt){
            if (container->m_lastClickedWidget != nullptr)
            {
                container->m_lastClickedWidget->handleEvent<LeaveClickEvent>(evt);
                container->m_lastClickedWidget = nullptr;
            }
            return false;
        });
    }

} // namespace pTK
