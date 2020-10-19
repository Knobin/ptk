//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

// Local Headers
#include "ptk/Window.hpp"
#include "platform/Platform.hpp"
#include "ptk/events/WindowEvent.hpp"

namespace pTK
{
    Window::Window(const std::string& name, const Size& size, BackendType backend)
        : VBox(), Singleton(), m_threadID{std::this_thread::get_id()}
    {
        // Set Widget properties.
        Sizable::setSize(size);
        setName(name);
        Drawable::hide();

        m_winBackend = std::make_unique<PTK_MAINWINDOW_TYPE>(this, name, size, backend);
    }

    void Window::onChildDraw(size_type)
    {
        postEvent<PaintEvent>(Point{0, 0}, getSize());
    }

    void Window::handleEvents()
    {
        m_eventQueue.lock();
        std::size_t eventCount{m_eventQueue.size()};
        m_eventQueue.unlock();
        
        while (eventCount > 0)
        {
            m_eventQueue.lock();
            std::unique_ptr<Event> event = std::move(m_eventQueue.front());
            m_eventQueue.pop();
            m_eventQueue.unlock();
            
            handleEvent(event.get());
            
            --eventCount;
            if (eventCount == 0)
            {
                m_eventQueue.lock();
                eventCount = m_eventQueue.size();
                m_eventQueue.unlock();
            }
        }

        if (m_draw)
        {
            forceDrawAll();
            m_draw = false;
        }
    }

    void Window::sendEvent(Event* event)
    {
        handleEvent(event);
    }

    Vec2f Window::getDPIScale() const
    {
        if (m_winBackend)
            return m_winBackend->getDPIScale();
        return Vec2f{};
    }

    bool Window::shouldClose() const
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

    bool Window::isHidden() const
    {
        return m_winBackend->isHidden();
    }

    void Window::onSizeChange(const Size& size)
    {
        refitContent(size);
        if (m_winBackend)
            m_winBackend->resize(size);
        m_draw = true;
        if (m_onResize)
            m_onResize(size);
    }

    void Window::onLimitChange(const Size& min, const Size& max)
    {
        if (m_winBackend)
            m_winBackend->setLimits(min, max);
    }

    // Window specific callbacks.
    void Window::onClose(const std::function<bool()>& callback)
    {
        m_onClose = callback;
    }

    void Window::onMove(const std::function<bool(const Point& pos)>& callback)
    {
        m_onMove = callback;
    }

    void Window::onResize(const std::function<bool(const Size& pos)>& callback)
    {
        m_onResize = callback;
    }

    void Window::onFocus(const std::function<bool()>& callback)
    {
        m_onFocus = callback;
    }

    void Window::onLostFocus(const std::function<bool()>& callback)
    {
        m_onLostFocus = callback;
    }

    void Window::onMinimize(const std::function<bool()>& callback)
    {
        m_onMinimize = callback;
    }

    void Window::onRestore(const std::function<bool()>& callback)
    {
        m_onRestore = callback;
    }

    void Window::handleEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");

        if (event->category == Event::Category::Window)
            handleWindowEvent(event);
        else if (event->category == Event::Category::Key)
            handleKeyboardEvent(event);
        else if (event->category == Event::Category::Mouse)
            handleMouseEvent(event);
#ifdef PTK_DEBUG
        else
            PTK_WARN("Unknown event");
#endif
    }

    void Window::handleKeyboardEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        KeyEvent* kEvent{static_cast<KeyEvent*>(event)};
        handleKeyEvent(kEvent->type, kEvent->keycode);
    }

    void Window::handleMouseEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        Event::Type type{event->type};
        if (type == Event::Type::MouseMoved)
        {
            MotionEvent* mEvent{static_cast<MotionEvent*>(event)};
            handleHoverEvent(mEvent->pos);
        }
        else if (type == Event::Type::MouseButtonPressed ||
                 type == Event::Type::MouseButtonReleased)
        {
            ButtonEvent* bEvent{static_cast<ButtonEvent*>(event)};
            Point pos{bEvent->pos};
            Mouse::Button btn{bEvent->button};
            if (type == Event::Type::MouseButtonPressed)
                handleClickEvent(btn, pos);
            else if (type == Event::Type::MouseButtonReleased)
                handleReleaseEvent(btn, pos);
        }
        else if (type == Event::Type::MouseScrolled)
        {
            ScrollEvent* sEvent{static_cast<ScrollEvent*>(event)};
            handleScrollEvent(sEvent->offset);
        }
    }

    void Window::handleWindowEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        Event::Type type{event->type};
        switch (type)
        {
            case Event::Type::WindowPaint:
            {
                m_draw = true;
                break;
            }
            case Event::Type::WindowResize:
            {
                ResizeEvent* rEvent{static_cast<ResizeEvent*>(event)};
                setSize(rEvent->size);
                m_draw = true;
                break;
            }
            case Event::Type::WindowMove:
            {
                MoveEvent* mEvent{static_cast<MoveEvent*>(event)};
                setPosHint(mEvent->pos);
                break;
            }
            case Event::Type::WindowScale:
            {
                ScaleEvent* sEvent{static_cast<ScaleEvent*>(event)};
                m_winBackend->setScaleHint(sEvent->scale);
                m_draw = true;
                break;
            }
            case Event::Type::WindowClose:
            {
                close();
                break;
            }
            case Event::Type::WindowFocus:
            {
                if (m_onFocus)
                    m_onFocus();
                break;
            }
            case Event::Type::WindowLostFocus:
            {
                if (m_onLostFocus)
                    m_onLostFocus();
                break;
            }
            case Event::Type::WindowMinimize:
            {
                minimize();
                break;
            }
            case Event::Type::WindowRestore:
            {
                restore();
                break;
            }
            default:
            {
                PTK_WARN("Unknown Window event");
                break;
            }
        }
    }

    void Window::close()
    {
        if (!m_close)
        {
            m_close = true;
            m_winBackend->close();
            if (m_onClose)
                m_onClose();
        }
    }

    void Window::forceDrawAll()
    {
        m_winBackend->beginPaint();
        ContextBase* context{m_winBackend->getContext()};
        sk_sp<SkSurface> surface = context->surface();
        SkCanvas* canvas{surface->getCanvas()};

        // Apply monitor scale.
        SkMatrix matrix{};
        Vec2f scale{m_winBackend->getDPIScale()};
        matrix.setScale(scale.x, scale.y);
        canvas->setMatrix(matrix);
    
        // Background.
        Size size{getSize()};
        SkRect rect{0, 0, static_cast<float>(size.width), static_cast<float>(size.height)};
        Color bg{getBackground()};
        SkPaint paint{};
        paint.setARGB(255, bg.r, bg.g, bg.b);
        canvas->drawRect(rect, paint);
        
        for (auto& widget : *this)
            widget->onDraw(canvas);

        surface->flushAndSubmit();
        m_winBackend->swapBuffers();
        m_winBackend->endPaint();
    }

    void Window::setPosHint(const Point& pos)
    {
        if (m_winBackend && m_winBackend->setPosHint(pos))
            if (m_onMove)
                m_onMove(pos);
    }

    Point Window::getWinPos() const
    {
        return m_winBackend->getWinPos();
    }

    Size Window::getWinSize() const
    {
        return m_winBackend->getWinSize();
    }

    void Window::setTitle(const std::string& name)
    {
        m_winBackend->setTitle(name);
    }

    bool Window::setIcon(const std::string& path)
    {
        // Load file.
        sk_sp<SkData> imageData{SkData::MakeFromFileName(path.c_str())};
        if (imageData)
        {
            // Convert file to image.
            sk_sp<SkImage> image{SkImage::MakeFromEncoded(imageData)};
            if (image)
            {
                // Read pixels in an RGBA format.
                const SkImageInfo imageInfo{
                    image->imageInfo().makeColorType(SkColorType::kRGBA_8888_SkColorType)};
                const size_t storageSize{imageInfo.computeMinByteSize()};
                std::unique_ptr<byte[]> pixelData{std::make_unique<byte[]>(storageSize)};

                if (image->readPixels(imageInfo, pixelData.get(), imageInfo.minRowBytes(), 0, 0))
                    return m_winBackend->setIcon(static_cast<int32>(image->width()),
                                          static_cast<int32>(image->height()), pixelData.get());
#ifdef PTK_DEBUG
                else
                {
                    PTK_WARN("Failed to convert image \"{}\" to a RGBA format", path);
                }
#endif
            }
#ifdef PTK_DEBUG
            else
            {
                PTK_WARN("Could not decode image \"{}\"", path);
            }
#endif
        }
#ifdef PTK_DEBUG
        else
        {
            PTK_WARN("Failed to open \"{}\"", path);
        }
#endif
        return false;
    }

    MainWindowBase* Window::getBackend() const
    {
        return m_winBackend.get();
    }

    void Window::minimize()
    {
        if (!m_minimized)
        {
            m_minimized = true;

            // Window might not be minimized when handling the event.
            // Minimize it if so.
            if (!m_winBackend->isMinimized())
                m_winBackend->minimize();

            Drawable::hide();
            if (m_onMinimize)
                m_onMinimize();
        }
    }

    void Window::restore()
    {
        if (m_minimized)
        {
            // Window might be minimized when handling the event.
            // Restore it if so.
            if (m_winBackend->isMinimized())
                m_winBackend->restore();

            m_minimized = false;
            Drawable::show();
            if (m_onRestore)
                m_onRestore();
        }
    }

    bool Window::isMinimized() const
    {
        return m_minimized;
    }

    bool Window::isFocused() const
    {
        return m_winBackend->isFocused();
    }

} // namespace pTK
