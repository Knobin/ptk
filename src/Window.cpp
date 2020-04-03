//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

// Local Headers
#include "ptk/Window.hpp"
#include "ptk/platform/Backend.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WindowEvent.hpp"
#include "ptk/Core.hpp"
#include "ptk/core/ContextBase.hpp"

// C++ Headers
#include <exception>
#include <memory>

namespace pTK
{
    Window::Window(const std::string& name, const Vec2u& size, Backend backend)
            : VBox(), Singleton(),
                m_winBackend{nullptr}, m_eventQueue{}, m_draw{false}, m_close{false}
    {
        // Set Widget properties.
        Sizable::setSize({static_cast<Size::value_type>(size.x), static_cast<Size::value_type>(size.y)});
        setName(name);
        Drawable::hide();

        m_winBackend = new BACKEND(this, name, size, backend);
    }

    void Window::onChildDraw(size_type)
    {
        postEvent(new Event{Event::Category::Window, Event::Type::WindowDraw});
    }

    void Window::handleEvents()
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

    void Window::sendEvent(Event *event)
    {
        handleEvent(event);
    }

    void Window::postEvent(Event *event)
    {
        m_eventQueue.push(std::unique_ptr<Event>(event));
    }

    Size Window::getContentSize() const
    {
        Size wSize{getSize()};
        Vec2f scale{getDPIScale()};
        wSize.width = static_cast<Size::value_type>(wSize.width * scale.x);
        wSize.height = static_cast<Size::value_type>(wSize.height * scale.y);
        return wSize;
    }

    Vec2f Window::getDPIScale() const
    {
        return m_winBackend->getDPIScale();
    }

    bool Window::shouldClose()
    {
        return m_close;
    }

    void Window::show()
    {
        Drawable::show();
        m_winBackend->show();
    }

    void Window::hide()
    {
        Drawable::hide();
        m_winBackend->hide();
    }

    void Window::onResize(const Size& size)
    {
        if (m_winBackend)
            m_winBackend->resize(size);
    }

    void Window::onLimitChange(const Size& min, const Size& max)
    {
        if (m_winBackend)
            m_winBackend->setLimits(min, max);
    }

    void Window::handleEvent(Event *event)
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

    void Window::handleKeyboardEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        KeyEvent* kEvent = static_cast<KeyEvent*>(event);
        handleKeyEvent(kEvent->type(), kEvent->get_keycode());
    }

    void Window::handleMouseEvent(Event* event)
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

    void Window::handleWindowEvent(Event* event)
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
            Vec2f scale{m_winBackend->getDPIScale()};
            Size cSize{static_cast<Size::value_type>(size.width * scale.x),
                       static_cast<Size::value_type>(size.height * scale.y)};

            setSize(size);
            m_winBackend->resize(size);
            refitContent(size);
            m_draw = true;
        }else if (type == Event::Type::WindowClose)
        {
            m_close = true;
        }
    }

    void Window::close()
    {
        m_winBackend->close();
    }

    void Window::pollEvents()
    {
        m_winBackend->pollEvents();
    }

    void Window::forceDrawAll()
    {
        m_winBackend->beginPaint();
        ContextBase *context = m_winBackend->getContext();
        SkCanvas* canvas = context->skCanvas();

        Color color = getBackground();
        context->clear(color);

        // Apply monitor scale.
        SkMatrix matrix;
        Vec2f scale{getDPIScale()};
        matrix.setScale(scale.x, scale.y);
        canvas->setMatrix(matrix);

        for (iterator it = begin(); it != end(); it++)
            (*it)->onDraw(canvas);

        canvas->flush();
        m_winBackend->swapbuffers();
        m_winBackend->endPaint();
    }
}