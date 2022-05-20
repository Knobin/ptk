//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/Application.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkData.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Window::Window(const std::string& name, const Size& size, const WindowInfo& flags)
        : PTK_WINDOW_HANDLE_T(name, size, flags), SingleObject(),
            m_threadID{std::this_thread::get_id()}
    {
        // Set Widget properties.
        setName(name);

        // m_handle = std::make_unique<PTK_WINDOW_HANDLE_T>(this, name, size, flags);
        PTK_INFO("Initialized Window");
    }

    Window::~Window()
    {
        // Remove the Window from the Application if it exists, in case if it is still there.
        if (auto app = Application::Get())
            app->removeWindow(this);

        PTK_INFO("Destroyed Window");
    }

    void Window::onChildDraw(size_type)
    {
        // onChildDraw is not a thread safe function...
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

        if (m_draw && !m_close)
        {
            forceDrawAll();
            m_draw = false;
        }
    }

    void Window::sendEvent(Event* event)
    {
        handleEvent(event);
    }

    bool Window::shouldClose() const
    {
        return m_close;
    }

    void Window::onSizeChange(const Size& size)
    {
        resize(size);
        refitContent(size);
        forceDrawAll();
    }

    /*void Window::onLimitChange(const Size& min, const Size& max)
    {
        setLimits(min, max);
        // m_handle.setLimits(min, max);
        // setLimits(min, max);
    }*/

    // Window specific callbacks.
    void Window::onClose(const std::function<bool()>& callback)
    {
        addListener<CloseEvent>(callback);
    }

    void Window::onMove(const std::function<bool(const Point& pos)>& callback)
    {
        addListener<MoveEvent>([&](const MoveEvent& evt){
            callback(evt.pos);
            return false;
        });
    }

    void Window::onResize(const std::function<bool(const Size& pos)>& callback)
    {
        addListener<ResizeEvent>([&](const ResizeEvent& evt){
            callback(evt.size);
            return false;
        });
    }

    void Window::onFocus(const std::function<bool()>& callback)
    {
        addListener<FocusEvent>(callback);
    }

    void Window::onLostFocus(const std::function<bool()>& callback)
    {
        addListener<LostFocusEvent>(callback);
    }

    void Window::onMinimize(const std::function<bool()>& callback)
    {
        addListener<MinimizeEvent>(callback);
    }

    void Window::onRestore(const std::function<bool()>& callback)
    {
        addListener<RestoreEvent>(callback);
    }

    void Window::handleEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");

        if (event->category == Event::Category::Window)
            handleWindowEvent(event);
        else if (event->category == Event::Category::Keyboard)
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

        if (event->type == Event::Type::KeyPressed || event->type == Event::Type::KeyReleased)
        {
            KeyEvent* kEvent{static_cast<KeyEvent*>(event)};
            triggerEvent<KeyEvent>(*kEvent);
        }
        else if (event->type == Event::Type::KeyInput)
        {
            InputEvent* iEvent{static_cast<InputEvent*>(event)};
            triggerEvent<InputEvent>(*iEvent);
        }
    }

    void Window::handleMouseEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        Event::Type type{event->type};
        if (type == Event::Type::MouseMoved)
        {
            MotionEvent* mEvent{static_cast<MotionEvent*>(event)};
            triggerEvent<MotionEvent>(*mEvent);
        }
        else if (type == Event::Type::MouseButtonPressed ||
                 type == Event::Type::MouseButtonReleased)
        {
            ButtonEvent* bEvent{static_cast<ButtonEvent*>(event)};
            Point pos{bEvent->pos};
            Mouse::Button btn{bEvent->button};
            if (type == Event::Type::MouseButtonPressed)
            {
                ClickEvent cEvt{btn, pos};
                triggerEvent<ClickEvent>(cEvt);
            }
            else if (type == Event::Type::MouseButtonReleased)
            {
                ReleaseEvent rEvt{ btn, pos };
                triggerEvent<ReleaseEvent>(rEvt);
            }
        }
        else if (type == Event::Type::MouseScrolled)
        {
            ScrollEvent* sEvent{static_cast<ScrollEvent*>(event)};
            triggerEvent<ScrollEvent>(*sEvent);
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
                // m_handle.setScaleHint(sEvent->scale);
                setScaleHint(sEvent->scale);
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
                //if (m_onFocus) // Nullptr for some reason.
                    //m_onFocus();
                break;
            }
            case Event::Type::WindowLostFocus:
            {
                //if (m_onLostFocus)
                    //m_onLostFocus();
                //handleLeaveClickEvent();
                // TODO: Fix handleLeaveClickEvent()
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

    /*
    void Window::close()
    {
        if (!m_close)
        {
            m_close = true;
            m_handle.close();
            if (m_onClose)
                m_onClose();
        }
    }
    */

    void Window::forceDrawAll()
    {
        // m_handle.beginPaint();
        beginPaint();
        // ContextBase* context{m_handle.getContext()};
        ContextBase* context{ getContext() };
        sk_sp<SkSurface> surface = context->surface();
        SkCanvas* canvas{surface->getCanvas()};

        // Apply monitor scale.
        SkMatrix matrix{};
        // Vec2f scale{m_handle.getDPIScale()};
        Vec2f scale{ getDPIScale() };
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
        //m_handle.swapBuffers();
        //m_handle.endPaint();
        swapBuffers();
        endPaint();
    }

    bool Window::setIconFromFile(const std::string& path)
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
                    return setIcon(static_cast<int32>(image->width()),
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

} // namespace pTK
