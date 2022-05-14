//
//  platform/unix/WindowHandle_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// pTK Headers
#include "ptk/platform/unix/WindowHandle_unix.hpp"
#include "ptk/platform/unix/ApplicationHandle_unix.hpp"
#include "ptk/platform//common/RasterContext.hpp"
#include "ptk/platform/unix/RasterPolicy_unix.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"

#ifdef PTK_OPENGL
    #include "ptk/platform/unix/GLContext_unix.hpp"
#endif // PTK_OPENGL

using App = pTK::ApplicationHandle_unix;

namespace pTK
{
    static float SystemDPI()
    {
        char *rString{XResourceManagerString(App::Display())};
        XrmInitialize();
        XrmDatabase db{XrmGetStringDatabase(rString)};

        if (rString)
        {
            char *type;
            XrmValue val;
            if (XrmGetResource(db, "Xft.dpi", "String", &type, &val) == True)
            {
                if (val.addr)
                {
                    const float dpi{static_cast<float>(std::atof(val.addr))};
                    return (dpi != 0.0f ? dpi : 96.0f);
                }
            }
        }

        return 96.0f;
    }

    static Size ScaleSize(const Size& size, const Vec2f& scale)
    {
        const float width{static_cast<float>(size.width)};
        const float height{static_cast<float>(size.height)};
        return {static_cast<Size::value_type>(width * scale.x),
                static_cast<Size::value_type>(height * scale.y)};
    }

    static std::unique_ptr<ContextBase> CreateContext([[maybe_unused]] WindowInfo::Backend backend, ::Window *window, const Size& size, [[maybe_unused]] XVisualInfo info)
    {
        std::unique_ptr<ContextBase> context{nullptr};
#ifdef PTK_OPENGL
        if (backend == WindowInfo::Backend::Hardware)
        {
            try {
                context = std::make_unique<GLContext_unix>(window, size);
            } catch (ContextError& err)
            {
                PTK_ERROR("Failed to create GLContext_unix with msg: {}", err.what());
            }

            if (context)
                return context;

            PTK_ERROR("Failed to create GLContext_unix");
        }
#endif // PTK_OPENGL

        RasterPolicy_unix policy{window, info};
        context = std::make_unique<RasterContext<RasterPolicy_unix>>(size, policy);

        if (!context)
            throw ContextError("Failed to create RasterContext with RasterPolicy_unix");

        return context;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    WindowHandle_unix::WindowHandle_unix(const std::string& name, const Size& size, const WindowInfo& flags)
        : WindowHandle(size) //, m_lastSize{size}
    {
        ::Window root{App::Root()};
        Display *display{App::Display()};
        int screenBitDepth{24};

        if (!XMatchVisualInfo(display, App::Screen(), screenBitDepth, TrueColor, &m_info))
            throw WindowError("No matching visual info");

        const unsigned long eventMask = ExposureMask | StructureNotifyMask |
                                        KeyPressMask | KeyReleaseMask |
                                        PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                                        FocusChangeMask;

        XSetWindowAttributes attr{};
        attr.event_mask = eventMask;
        attr.colormap = XCreateColormap(display, root, m_info.visual, AllocNone);
        const unsigned long attrMask = CWEventMask | CWColormap;

        const unsigned int width{static_cast<unsigned int>(size.width)};
        const unsigned int height{static_cast<unsigned int>(size.height)};
        m_window = XCreateWindow(display, root, 0, 0, width, height, 0, 24, InputOutput, m_info.visual, attrMask, &attr);

        if (!m_window)
            throw WindowError("Failed to create Window");

        setTitle(name);

        XMapWindow(display, m_window);
        XSaveContext(display, m_window, App::Context(), reinterpret_cast<XPointer>(this));
        XFlush(display);

        m_atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
        if (!XSetWMProtocols(display, m_window, &m_atomWmDeleteWindow, 1))
        {
            PTK_WARN("Could not register WM_DELETE_WINDOW property");
        }

        const float scale{SystemDPI()};
        PTK_INFO("System DPI is {}", scale);
        m_scale = Vec2f{scale / 96.0f, scale / 96.0f};
        const Size scaledSize{ScaleSize(size, m_scale)};

        m_context = CreateContext(flags.backend, &m_window, scaledSize, m_info);

        // m_lastPos = getWinPos();
        PTK_INFO("Initialized WindowHandle_unix");
    }

    WindowHandle_unix::~WindowHandle_unix()
    {
        PTK_INFO("Destroyed WindowHandle_unix");
    }

    bool WindowHandle_unix::close()
    {
        XDestroyWindow(App::Display(), m_window);
        m_window = x11::None;
        return true;
    }

    void WindowHandle_unix::show()
    {
        XMapWindow(App::Display(), m_window);
    }

    void WindowHandle_unix::hide()
    {
        XUnmapWindow(App::Display(), m_window);
    }

    bool WindowHandle_unix::isHidden() const
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(App::Display(), m_window, &xwa);
        return !(xwa.map_state == IsViewable);
    }

    void WindowHandle_unix::setPosHint(const Point& pos)
    {
        if (pos != getWinPos())
        {
            Display *display{App::Display()};
            XMoveWindow(display, m_window, pos.x, pos.y);
            XFlush(display);
        }
    }

    ContextBase *WindowHandle_unix::getContext() const
    {
        return m_context.get();
    }

    void WindowHandle_unix::swapBuffers()
    {
        m_context->swapBuffers();
    }

    Vec2f WindowHandle_unix::getDPIScale() const
    {
        return m_scale;
    }

    bool WindowHandle_unix::resize(const Size& size)
    {
        bool status{false};

        if (size != m_context->getSize())
        {
            const Size scaledSize{ScaleSize(size, m_scale)};
            m_context->resize(scaledSize);
            status = true;
        }

        if (size != getWinSize())
        {
            const unsigned int width{static_cast<unsigned int>(size.width)};
            const unsigned int height{static_cast<unsigned int>(size.height)};

            XResizeWindow(App::Display(), m_window, width, height);
            status = true;
        }

        return status;
    }

    /* bool WindowHandle_unix::setLimits(const Size& min, const Size& max)
    {
        XSizeHints *hints{XAllocSizeHints()};
        PTK_ASSERT(hints, "Unable to allocate memory for XSizeHints");

        hints->flags |= PMinSize | PMaxSize | PWinGravity;
        hints->win_gravity = StaticGravity;

        hints->min_width = min.width;
        hints->min_height = min.height;
        hints->max_width = max.width;
        hints->max_height = max.height;

        XSetWMNormalHints(App::Display(), m_window, hints);
        XFree(hints);

        XFlush(App::Display());
        return true;
    }*/

    bool WindowHandle_unix::setTitle(const std::string& name)
    {
        Display *display{App::Display()};
        const Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
        const Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
        const Atom net_wm_icon_name = XInternAtom(display, "_NET_WM_ICON_NAME", False);

        XChangeProperty(display, m_window, net_wm_name, utf8_string, 8, PropModeReplace, reinterpret_cast<const unsigned char*>(name.c_str()), static_cast<int>(name.size()));
        XChangeProperty(display, m_window, net_wm_icon_name, utf8_string, 8, PropModeReplace, reinterpret_cast<const unsigned char*>(name.c_str()),  static_cast<int>(name.size()));

        XFlush(display);
        return true;
    }

    bool WindowHandle_unix::setIcon(int32 width, int32 height, byte* pixels)
    {
        Display *display{App::Display()};
        const Atom net_wm_icon = XInternAtom(display, "_NET_WM_ICON", False);
        const std::size_t longCount{static_cast<std::size_t>(2 + (width * height))};
        std::unique_ptr<long[]> longData{std::make_unique<long[]>(longCount)};

        // Apparently width and height must be located in the beginning of the data.
        longData[0] = static_cast<long>(width);
        longData[1] = static_cast<long>(height);

        // Expects ARGB, pixel array is RGBA.
        for (std::size_t i{2}; i < longCount; ++i)
        {
            byte r{pixels[(i * 4)]};
            byte g{pixels[(i * 4) + 1]};
            byte b{pixels[(i * 4) + 2]};
            byte a{pixels[(i * 4) + 3]};

            longData[i] = (static_cast<long>(a) << 24) | (static_cast<long>(r) << 16) | (static_cast<long>(g) << 8) | static_cast<long>(b);
        }

        XChangeProperty(display, m_window, net_wm_icon, XA_CARDINAL, 32, PropModeReplace, reinterpret_cast<unsigned char*>(longData.get()), static_cast<int>(longCount));
        //XChangeProperty(display, m_window, net_wm_icon, cardinal, 32, PropModeReplace, (unsigned char*)longData.get(), longCount);
        XFlush(display);
        return true;
    }

    void WindowHandle_unix::notifyEvent()
    {
        // notifyEvent is not thread safe, so m_window can already be destroyed.
        if (m_window != x11::None)
        {
            Display *display{App::Display()};
            const Atom nullAtom{XInternAtom(display, "NULL", False)};

            XEvent event{ClientMessage};
            event.xclient.window = m_window;
            event.xclient.format = 32;
            event.xclient.message_type = nullAtom;


            XSendEvent(display, m_window, False, 0, &event);
            XFlush(display);
        }
    }

    Point WindowHandle_unix::getWinPos() const
    {
        int x{-1}, y{-1};
        ::Window child;

        XTranslateCoordinates(App::Display(), m_window, App::Root(), 0, 0, &x, &y, &child);
        return Point{static_cast<Point::value_type>(x), static_cast<Point::value_type>(y)};
    }

    Size WindowHandle_unix::getWinSize() const
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(App::Display(), m_window, &xwa);

        return Size{static_cast<Size::value_type>(xwa.width), static_cast<Size::value_type>(xwa.height)};
    }

    bool WindowHandle_unix::minimize()
    {
        Display *display{App::Display()};
        XIconifyWindow(display, m_window, m_info.screen);
        XFlush(display);
        return true;
    }

    bool WindowHandle_unix::isMinimized() const
    {
        uint32 state{WithdrawnState};
        Atom wm_state{XInternAtom(App::Display(), "WM_STATE", False)};
        auto property = getWindowProperty(wm_state, wm_state);
        if (property.second)
        {
            if (property.first >= 2)
            {
                state = *reinterpret_cast<uint32*>(property.second);
            }
            XFree(property.second);
        }

        return state == IconicState;
    }

    bool WindowHandle_unix::restore()
    {
        Display *display{App::Display()};
        XMapWindow(display, m_window);
        XFlush(display);
        return true;
    }

    bool WindowHandle_unix::isFocused() const
    {
        ::Window focusedWindow;
        int focusState;

        XGetInputFocus(App::Display(), &focusedWindow, &focusState);
        return m_window == focusedWindow;
    }

    bool WindowHandle_unix::setScaleHint(const Vec2f& scale)
    {
        if (m_scale != scale)
        {
            m_scale = scale;
            resize(getSize());
        }

        return true;
    }

    std::pair<unsigned long, unsigned char*> WindowHandle_unix::getWindowProperty(Atom property, Atom type) const
    {
        Atom realType;
        int realFormat;
        unsigned long left;
        std::pair<unsigned long, unsigned char*> data{};
        XGetWindowProperty(App::Display(), m_window, property, 0L, 2L, False, type, &realType, &realFormat, &data.first, &left, &data.second);
        return data;
    }

    ::Window WindowHandle_unix::xWindow() const
    {
        return m_window;
    }

    Atom WindowHandle_unix::deleteAtom() const
    {
        return m_atomWmDeleteWindow;
    }

    /*Size& WindowHandle_unix::lastSize()
    {
        return m_lastSize;
    }

    Point& WindowHandle_unix::lastPos()
    {
        return m_lastPos;
    }*/
}
