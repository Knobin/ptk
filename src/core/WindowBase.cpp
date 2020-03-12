//
//  core/BaseWindow.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-08.
//

// Local Headers
#include "ptk/core/WindowBase.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WindowEvent.hpp"
#include "ptk/Core.hpp"

// C++ Headers
#include <exception>

namespace pTK
{
    WindowBase::WindowBase(const std::string& name, const Vec2u& size)
            : VBox(), Singleton(),
              m_eventQueue{}, m_scale{1.0f, 1.0f}, m_draw{false}, m_close{false}
    {
        // Set Widget properties.
        Sizable::setSize({static_cast<Size::value_type>(size.x), static_cast<Size::value_type>(size.y)});
        setName(name);
    }

    void WindowBase::onChildDraw(size_type)
    {
        PTK_WARN("onChildDraw");
        postEvent(new Event{Event::Category::Window, Event::Type::WindowDraw});
    }

    void WindowBase::handleEvents()
    {
        SafeQueue<std::unique_ptr<Event>>::size_type eventCount = m_eventQueue.size();
        for (uint i = 0; i < eventCount; i++)
        {
            std::unique_ptr<Event> event = std::move(m_eventQueue.front());
            m_eventQueue.pop();

            handleEvent(event.get());
        }

        // Quick fix when previous event cause a draw event.
        if (!m_eventQueue.empty())
        {
            Ref<Event> event = std::move(m_eventQueue.front());
            if (event->category() == Event::Category::Window)
            {
                handleWindowEvent(event.get());
                m_eventQueue.pop();
            }
        }

        if (m_draw)
        {
            forceDrawAll();
            m_draw = false;
        }
    }

    void WindowBase::sendEvent(Event *event)
    {
        handleEvent(event);
    }

    void WindowBase::postEvent(Event *event)
    {
        m_eventQueue.push(std::unique_ptr<Event>(event));
    }

    Size WindowBase::getContentSize() const
    {
        Size wSize{getSize()};
        wSize.width = static_cast<Size::value_type>(wSize.width * m_scale.x);
        wSize.height = static_cast<Size::value_type>(wSize.height * m_scale.y);
        return wSize;
    }

    const Vec2f& WindowBase::getDPIScale() const
    {
        return m_scale;
    }

    bool WindowBase::shouldClose()
    {
        return m_close;
    }

    void WindowBase::setScale(const Vec2f& scale)
    {
        m_scale = scale;
    }

    void WindowBase::handleEvent(Event *event)
    {
        PTK_ASSERT(event, "Undefined Event");

        if (event->category() == Event::Category::Window)
            handleWindowEvent(event);
        else if (event->category() == Event::Category::Key)
            handleKeyboardEvent(event);
        else if (event->category() == Event::Category::Mouse)
            handleMouseEvent(event);
#ifdef PTK_DEBUG
        else
            PTK_WARN("Unknown event");
#endif
    }

    void WindowBase::handleKeyboardEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        KeyEvent* kEvent = static_cast<KeyEvent*>(event);
        handleKeyEvent(kEvent->type(), kEvent->get_keycode());
    }

    void WindowBase::handleMouseEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        Event::Type type = event->type();
        if (type == Event::Type::MouseMoved)
        {
            MotionEvent* mEvent = static_cast<MotionEvent*>(event);
            handleHoverEvent(mEvent->getPos());
        } else if (type == Event::Type::MouseButtonPressed || type == Event::Type::MouseButtonReleased)
        {
            ButtonEvent* bEvent = static_cast<ButtonEvent*>(event);
            Point pos{bEvent->getPos()};
            Mouse::Button btn = bEvent->getButton();
            if (type == Event::Type::MouseButtonPressed)
                handleClickEvent(btn, pos);
            else if (type == Event::Type::MouseButtonReleased)
                handleReleaseEvent(btn, pos);
        } else if (type == Event::Type::MouseScrolled)
        {
            ScrollEvent* sEvent = static_cast<ScrollEvent*>(event);
            handleScrollEvent(sEvent->getOffset());
        }
    }

    void WindowBase::handleWindowEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        Event::Type type = event->type();
        if (type == Event::Type::WindowDraw)
        {
            m_draw = true;
        }
        else if (type == Event::Type::WindowResize)
        {
            ResizeEvent* rEvent = (ResizeEvent*)event;
            Size size{rEvent->getSize()};
            Size cSize{static_cast<Size::value_type>(size.width * m_scale.x),
                       static_cast<Size::value_type>(size.height * m_scale.y)};

            setSize(size);
            refitContent(size);
            m_draw = true;
        }else if (type == Event::Type::WindowClose)
        {
            m_close = true;
        }
    }
}
