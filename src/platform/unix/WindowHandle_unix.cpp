//
//  platform/unix/WindowHandle_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "WindowHandle_unix.hpp"
#include "../../Log.hpp"
#include "../../core/Assert.hpp"
#include "ApplicationHandle_unix.hpp"

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"

// C++ Headers
#include <cstdint>
#include <limits>

namespace pTK::Platform
{
    using App = ApplicationHandle_unix;

    namespace WindowHandleFactoryImpl
    {
        std::unique_ptr<WindowHandle> Make(WindowBase* base, const std::string& name, const Size& size,
                                           const WindowInfo& info)
        {
            return std::make_unique<WindowHandle_unix>(base, name, size, info);
        }
    } // namespace WindowHandleFactoryImpl

    static float SystemDPI()
    {
        char* rString{XResourceManagerString(App::Display())};
        XrmInitialize();
        XrmDatabase db{XrmGetStringDatabase(rString)};

        if (rString)
        {
            char* type;
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

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    WindowHandle_unix::WindowHandle_unix(WindowBase* base, const std::string& name, const Size& size, const WindowInfo&)
        : WindowHandle(base) //, m_lastSize{size}
    {
        ::Window root{App::Root()};
        Display* display{App::Display()};
        int screenBitDepth{24};

        if (!XMatchVisualInfo(display, App::Screen(), screenBitDepth, TrueColor, &m_info))
            throw WindowError("No matching visual info");

        const unsigned long eventMask = ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                                        PointerMotionMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask;

        XSetWindowAttributes attr{};
        attr.event_mask = eventMask;
        attr.colormap = XCreateColormap(display, root, m_info.visual, AllocNone);
        const unsigned long attrMask = CWEventMask | CWColormap;

        const unsigned int width{static_cast<unsigned int>(size.width)};
        const unsigned int height{static_cast<unsigned int>(size.height)};
        m_window =
            XCreateWindow(display, root, 0, 0, width, height, 0, 24, InputOutput, m_info.visual, attrMask, &attr);

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

        m_lastPos = getPosition();
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

    bool WindowHandle_unix::setPosHint(const Point& pos)
    {
        if (pos != getPosition())
        {
            Display* display{App::Display()};
            XMoveWindow(display, m_window, pos.x, pos.y);
            XFlush(display);
            return true;
        }
        return false;
    }

    Vec2f WindowHandle_unix::getDPIScale() const
    {
        return m_scale;
    }

    bool WindowHandle_unix::resize(const Size& size)
    {
        bool status{false};

        if (size != getSize())
        {
            PTK_INFO("bool WindowHandle_unix::resize(const Size& size)");
            const unsigned int width{static_cast<unsigned int>(size.width)};
            const unsigned int height{static_cast<unsigned int>(size.height)};

            XResizeWindow(App::Display(), m_window, width, height);
            status = true;
        }

        return status;
    }

    void WindowHandle_unix::setLimits(const Size& min, const Size& max)
    {
        XSizeHints* hints{XAllocSizeHints()};
        PTK_ASSERT(hints, "Unable to allocate memory for XSizeHints");
        long err;
        XGetWMNormalHints(App::Display(), m_window, hints, &err);
        PTK_INFO("WindowHandle_unix: Trying to set new Window Limits, min: {}x{} & max: {}x{}", min.width, min.height,
                 max.width, max.height);
        PTK_INFO("WindowHandle_unix: Current Window Limits: min: {}x{} & max: {}x{}", hints->min_width,
                 hints->min_height, hints->max_width, hints->max_height);

        constexpr int int_max = std::numeric_limits<int>::max();

        const int min_width =
            (min.width > static_cast<Size::value_type>(int_max)) ? int_max : static_cast<int>(min.width);
        const int min_height =
            (min.height > static_cast<Size::value_type>(int_max)) ? int_max : static_cast<int>(min.height);

        if (hints->min_width != min_width || hints->min_height != min_height)
        {
            PTK_INFO("WindowHandle_unix: Setting Min Size: {}x{}", min_width, min_height);
            hints->flags |= PMinSize;
            hints->min_width = min_width;
            hints->min_height = min_height;
        }

        const int max_width =
            (max.width > static_cast<Size::value_type>(int_max)) ? int_max : static_cast<int>(max.width);
        const int max_height =
            (max.height > static_cast<Size::value_type>(int_max)) ? int_max : static_cast<int>(max.height);

        if (hints->max_width != max_width || hints->max_height != max_height)
        {
            PTK_INFO("WindowHandle_unix: Setting Max Size: {}x{}", max_width, max_height);
            hints->flags |= PMaxSize;
            hints->max_width = max_width;
            hints->max_height = max_height;
        }

        hints->flags |= PWinGravity;
        hints->win_gravity = StaticGravity;

        XSetWMNormalHints(App::Display(), m_window, hints);
        XFree(hints);

        XFlush(App::Display());
    }

    bool WindowHandle_unix::setTitle(const std::string& name)
    {
        Display* display{App::Display()};
        const Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
        const Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
        const Atom net_wm_icon_name = XInternAtom(display, "_NET_WM_ICON_NAME", False);

        XChangeProperty(display, m_window, net_wm_name, utf8_string, 8, PropModeReplace,
                        reinterpret_cast<const unsigned char*>(name.c_str()), static_cast<int>(name.size()));
        XChangeProperty(display, m_window, net_wm_icon_name, utf8_string, 8, PropModeReplace,
                        reinterpret_cast<const unsigned char*>(name.c_str()), static_cast<int>(name.size()));

        XFlush(display);
        return true;
    }

    bool WindowHandle_unix::setIcon(int32_t width, int32_t height, uint8_t* pixels)
    {
        Display* display{App::Display()};
        const Atom net_wm_icon = XInternAtom(display, "_NET_WM_ICON", False);
        const std::size_t longCount{static_cast<std::size_t>(2 + (width * height))};
        std::unique_ptr<long[]> longData{std::make_unique<long[]>(longCount)};

        // Apparently width and height must be located in the beginning of the data.
        longData[0] = static_cast<long>(width);
        longData[1] = static_cast<long>(height);

        // Expects ARGB, pixel array is RGBA.
        for (std::size_t i{2}; i < longCount; ++i)
        {
            uint8_t r{pixels[(i * 4)]};
            uint8_t g{pixels[(i * 4) + 1]};
            uint8_t b{pixels[(i * 4) + 2]};
            uint8_t a{pixels[(i * 4) + 3]};

            longData[i] = (static_cast<long>(a) << 24) | (static_cast<long>(r) << 16) | (static_cast<long>(g) << 8) |
                          static_cast<long>(b);
        }

        XChangeProperty(display, m_window, net_wm_icon, XA_CARDINAL, 32, PropModeReplace,
                        reinterpret_cast<unsigned char*>(longData.get()), static_cast<int>(longCount));
        // XChangeProperty(display, m_window, net_wm_icon, cardinal, 32, PropModeReplace, (unsigned
        // char*)longData.get(), longCount);
        XFlush(display);
        return true;
    }

    void WindowHandle_unix::notifyEvent()
    {
        // notifyEvent is not thread safe, so m_window can already be destroyed.
        if (m_window != x11::None)
        {
            Display* display{App::Display()};
            const Atom nullAtom{XInternAtom(display, "NULL", False)};

            XEvent event{ClientMessage};
            event.xclient.window = m_window;
            event.xclient.format = 32;
            event.xclient.message_type = nullAtom;

            XSendEvent(display, m_window, False, 0, &event);
            XFlush(display);
        }
    }

    Point WindowHandle_unix::getPosition() const
    {
        int x{-1}, y{-1};
        ::Window child;

        XTranslateCoordinates(App::Display(), m_window, App::Root(), 0, 0, &x, &y, &child);
        return Point{static_cast<Point::value_type>(x), static_cast<Point::value_type>(y)};
    }

    Size WindowHandle_unix::getSize() const
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(App::Display(), m_window, &xwa);

        return Size{static_cast<Size::value_type>(xwa.width), static_cast<Size::value_type>(xwa.height)};
    }

    bool WindowHandle_unix::minimize()
    {
        Display* display{App::Display()};
        XIconifyWindow(display, m_window, m_info.screen);
        XFlush(display);
        return true;
    }

    bool WindowHandle_unix::isMinimized() const
    {
        uint32_t state{WithdrawnState};
        Atom wm_state{XInternAtom(App::Display(), "WM_STATE", False)};
        auto property = getWindowProperty(wm_state, wm_state);
        if (property.second)
        {
            if (property.first >= 2)
            {
                state = *reinterpret_cast<uint32_t*>(property.second);
            }
            XFree(property.second);
        }

        return state == IconicState;
    }

    bool WindowHandle_unix::restore()
    {
        Display* display{App::Display()};
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

    void WindowHandle_unix::invalidate()
    {
        HandlePlatformEvent<PaintEvent>({{0, 0}, getSize()});
    }

    std::size_t WindowHandle_unix::targetRefreshRate()
    {
        // TODO(knobin): Read Monitor refresh rate here.
        return 60;
    }

    std::pair<unsigned long, unsigned char*> WindowHandle_unix::getWindowProperty(Atom property, Atom type) const
    {
        Atom realType;
        int realFormat;
        unsigned long left;
        std::pair<unsigned long, unsigned char*> data{};
        XGetWindowProperty(App::Display(), m_window, property, 0L, 2L, False, type, &realType, &realFormat, &data.first,
                           &left, &data.second);
        return data;
    }

    ::Window WindowHandle_unix::xWindow() const
    {
        return m_window;
    }

    XVisualInfo WindowHandle_unix::xVisualInfo() const
    {
        return m_info;
    }

    Atom WindowHandle_unix::deleteAtom() const
    {
        return m_atomWmDeleteWindow;
    }
} // namespace pTK
