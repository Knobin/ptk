//
//  core/WidgetContainer.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-18.
//

// Local Headers
#include "../include/Assert.hpp"

// pTK Headers
#include "ptk/core/ContextBase.hpp"
#include "ptk/core/WidgetContainer.hpp"
#include "ptk/util/Math.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
PTK_DISABLE_WARN_END()

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

    void WidgetContainer::add(const value_type& widget)
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

    void WidgetContainer::remove(const value_type& widget)
    {
        auto it = std::find(m_holder.cbegin(), m_holder.cend(), widget);

        if (it != m_holder.cend())
        {
            widget->setParent(nullptr);
            if (m_currentHoverWidget.ptr == widget.get())
                m_currentHoverWidget = {};
            if (m_lastClickedWidget.ptr == widget.get())
                m_lastClickedWidget = {};

            onRemove(widget);
            m_holder.erase(it);
            draw();
        }
    }

    void WidgetContainer::clear()
    {
        onClear();

        for (auto& item : m_holder)
            item->setParent(nullptr);

        m_holder.clear();

        m_lastClickedWidget = {};
        m_currentHoverWidget = {};
    }

    WidgetContainer::const_iterator WidgetContainer::findChildAtPos(const Point& pos) const
    {
        for (auto it = cbegin(); it != m_holder.cend(); ++it)
        {
            const auto& item = *it;

            const Point startPos{item->getPosition()};
            const Size wSize{item->getSize()};
            const Point endPos{Math::AddWithoutOverflow(startPos.x, static_cast<Point::value_type>(wSize.width)),
                               Math::AddWithoutOverflow(startPos.y, static_cast<Point::value_type>(wSize.height))};

            if ((startPos.x <= pos.x) && (endPos.x >= pos.x))
                if ((startPos.y <= pos.y) && (endPos.y >= pos.y))
                    return it;
        }

        return cend();
    }

    template <typename Func>
    static void DelayDeleteZone(const WidgetContainer::value_type& widget, Func func)
    {
        // Ref might be deleted, hence the copy. Might be costly on performance though.
        WidgetContainer::value_type copy{nullptr};
        if (widget.use_count() == 1)
            copy = widget;
        func();
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

    void WidgetContainer::onDraw(Canvas* canvas)
    {
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
        ContainerEntryPair lastClicked{m_lastClickedWidget};
        bool found{false};

        auto it = findChildAtPos(evt.pos);
        if (it != cend())
        {
            const auto work = [this, it, &found, &evt, dist = it - cbegin()]() {
                m_lastClickedWidget = {(*it).get(), static_cast<std::size_t>(dist)};
                found = true;
                (*it)->handleEvent<ClickEvent>(evt);
            };
            DelayDeleteZone(*it, work);
        }

        if (lastClicked.ptr != m_lastClickedWidget.ptr || !found)
        {
            if (validEntryPair(lastClicked))
            {
                const auto work = [lastClicked, &evt]() {
                    LeaveClickEvent lcEvent{evt.button, evt.value, evt.pos};
                    lastClicked.ptr->handleEvent<LeaveClickEvent>(lcEvent);
                };
                DelayDeleteZone(m_holder[lastClicked.index], work);
            }
        }

        if (!found)
            m_lastClickedWidget = {};
    }

    void WidgetContainer::onReleaseCallback(const ReleaseEvent& evt)
    {
        if (validEntryPair(m_lastClickedWidget))
        {
            const auto work = [this, &evt]() {
                this->m_lastClickedWidget.ptr->handleEvent<ReleaseEvent>(evt);
            };
            DelayDeleteZone(m_holder[m_lastClickedWidget.index], work);
        }
    }

    void WidgetContainer::onKeyCallback(const KeyEvent& evt)
    {
        if (validEntryPair(m_lastClickedWidget))
        {
            const auto work = [this, &evt]() {
                this->m_lastClickedWidget.ptr->handleEvent<KeyEvent>(evt);
            };
            DelayDeleteZone(m_holder[m_lastClickedWidget.index], work);
        }
    }

    void WidgetContainer::onInputCallback(const InputEvent& evt)
    {
        if (validEntryPair(m_lastClickedWidget))
        {
            const auto work = [this, &evt]() {
                this->m_lastClickedWidget.ptr->handleEvent<InputEvent>(evt);
            };
            DelayDeleteZone(m_holder[m_lastClickedWidget.index], work);
        }
    }

    void WidgetContainer::onHoverCallback(const MotionEvent& evt)
    {
        auto it = findChildAtPos(evt.pos);
        if (it != cend())
        {
            const auto work = [this, it, &evt, dist = it - cbegin()]() {
                // Send Leave Event.
                if (m_currentHoverWidget.ptr != (*it).get() || m_currentHoverWidget.ptr == nullptr)
                {
                    ContainerEntryPair temp{
                        (*it).get(), static_cast<std::size_t>(dist)}; // Iterator might change, when passing the event.

                    if (this->validEntryPair(m_currentHoverWidget))
                    {
                        const auto work2 = [evt, ptr = m_currentHoverWidget.ptr]() {
                            LeaveEvent lEvent{evt.pos};
                            ptr->handleEvent<LeaveEvent>(lEvent);
                        };
                        DelayDeleteZone(this->m_holder[m_currentHoverWidget.index], work2);
                    }

                    // New current hovered Widget.
                    m_currentHoverWidget = temp;

                    // Fire Enter event on this and on to child.
                    EnterEvent entEvent{evt.pos};
                    m_currentHoverWidget.ptr->handleEvent<EnterEvent>(entEvent);
                }

                m_currentHoverWidget.ptr->handleEvent<MotionEvent>(evt);
            };
            DelayDeleteZone(*it, work);
            return;
        }

        if (this->validEntryPair(m_currentHoverWidget))
        {
            const auto work = [evt, ptr = m_currentHoverWidget.ptr]() {
                LeaveEvent lEvent{evt.pos};
                ptr->handleEvent<LeaveEvent>(lEvent);
            };
            DelayDeleteZone(this->m_holder[m_currentHoverWidget.index], work);
        }

        // New current hovered Widget.
        m_currentHoverWidget = {};
    }

    void WidgetContainer::onEnterCallback(const EnterEvent& evt)
    {
        if (this->validEntryPair(m_currentHoverWidget))
        {
            const auto work = [evt, ptr = m_currentHoverWidget.ptr]() {
                ptr->handleEvent<EnterEvent>(evt);
            };
            DelayDeleteZone(this->m_holder[m_currentHoverWidget.index], work);
        }
    }

    void WidgetContainer::onLeaveCallback(const LeaveEvent& evt)
    {
        if (this->validEntryPair(m_currentHoverWidget))
        {
            const auto work = [this, evt, ptr = m_currentHoverWidget.ptr]() {
                ptr->handleEvent<LeaveEvent>(evt);

                // Reset current hovered Widget.
                m_currentHoverWidget = {};
            };
            DelayDeleteZone(this->m_holder[m_currentHoverWidget.index], work);
        }
    }

    void WidgetContainer::onScrollCallback(const ScrollEvent& evt)
    {
        if (this->validEntryPair(m_currentHoverWidget))
        {
            const auto work = [evt, ptr = m_currentHoverWidget.ptr]() {
                ptr->handleEvent<ScrollEvent>(evt);
            };
            DelayDeleteZone(this->m_holder[m_currentHoverWidget.index], work);
        }
    }

    void WidgetContainer::setBackground(const Color& color)
    {
        m_background = color;
    }

    const Color& WidgetContainer::getBackground() const
    {
        return m_background;
    }

    Widget* WidgetContainer::getSelectedWidget() const
    {
        return m_lastClickedWidget.ptr;
    }

    bool WidgetContainer::busy() const
    {
        return m_busy;
    }

    void WidgetContainer::drawBackground(Canvas* canvas) const
    {
        canvas->drawRect(getPosition(), getSize(), m_background);
    }

    void WidgetContainer::initCallbacks()
    {
        /*onKey([container = this](Event::Type type, KeyCode keycode){
            if (auto widget = container->m_currentHoverWidget)
                widget->handleKeyEvent(type, keycode);
            return false; // Do not remove callback, ever.
        });*/

        addListener<KeyEvent>([&](const KeyEvent& evt) {
            onKeyCallback(evt);
            return false;
        });
        addListener<InputEvent>([&](const InputEvent& evt) {
            onInputCallback(evt);
            return false;
        });
        addListener<MotionEvent>([&](const MotionEvent& evt) {
            onHoverCallback(evt);
            return false;
        });
        addListener<EnterEvent>([&](const EnterEvent& evt) {
            onEnterCallback(evt);
            return false;
        });
        addListener<LeaveEvent>([&](const LeaveEvent& evt) {
            onLeaveCallback(evt);
            return false;
        });
        // addListener<LeaveClickEvent>([&](const LeaveClickEvent& evt) { onLeaveClickEvent(evt); return false; });
        addListener<ScrollEvent>([&](const ScrollEvent& evt) {
            onScrollCallback(evt);
            return false;
        });
        addListener<ClickEvent>([&](const ClickEvent& evt) {
            onClickCallback(evt);
            return false;
        });
        addListener<ReleaseEvent>([&](const ReleaseEvent& evt) {
            onReleaseCallback(evt);
            return false;
        });

        addListener<LeaveClickEvent>([container = this](const LeaveClickEvent& evt) {
            if (container->validEntryPair(container->m_lastClickedWidget))
            {
                const auto work = [container, &evt]() {
                    container->m_lastClickedWidget.ptr->handleEvent<LeaveClickEvent>(evt);
                    container->m_lastClickedWidget = {};
                };
                DelayDeleteZone(container->m_holder[container->m_lastClickedWidget.index], work);
            }
            return false;
        });
    }

} // namespace pTK
