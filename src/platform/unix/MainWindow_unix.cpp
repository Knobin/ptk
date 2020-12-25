//
//  platform/unix/MainWindow_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "MainWindow_unix.hpp"
#include "ptk/Window.hpp"
#include "Application_unix.hpp"
#include "ptk/core/Exception.hpp"
#include "../common/RasterContext.hpp"
#include "RasterPolicy_unix.hpp"

// X11 Headers
#include <X11/Xutil.h>
#include <X11/Xatom.h>

using App = pTK::Application_unix;

namespace pTK
{
    MainWindow_unix::MainWindow_unix(Window *window, const std::string& name, const Size& size, BackendType backend)
        : MainWindowBase(window, backend), m_lastSize{size}
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
        XSaveContext(display, m_window, App::Context(), reinterpret_cast<XPointer>(window));
        XFlush(display);

        m_atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
        if (!XSetWMProtocols(display, m_window, &m_atomWmDeleteWindow, 1))
        {
            PTK_WARN("Could not register WM_DELETE_WINDOW property");
        }

        RasterPolicy_unix policy{display, m_window, m_info};
        m_context = std::make_unique<RasterContext<RasterPolicy_unix>>(size, policy);


        m_lastPos = getWinPos();
        PTK_INFO("Initialized MainWindow_unix");
    }

    bool MainWindow_unix::close() 
    {
        XDestroyWindow(App::Display(), m_window);
        return true;
    }

    bool MainWindow_unix::show() 
    {
        XMapWindow(App::Display(), m_window);
        return true;
    }

    bool MainWindow_unix::hide() 
    {
        XUnmapWindow(App::Display(), m_window);
        return true;
    }

    bool MainWindow_unix::isHidden() const 
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(App::Display(), m_window, &xwa);
        return !(xwa.map_state == IsViewable);
    }

    bool MainWindow_unix::setPosHint(const Point& pos) 
    {
        if (pos != m_lastPos) 
        {
            Display *display{App::Display()};
            XMoveWindow(display, m_window, pos.x, pos.y);
            XFlush(display);
        }

        return true;
    }

    ContextBase *MainWindow_unix::getContext() const 
    {
        return m_context.get();
    }

    void MainWindow_unix::swapBuffers() 
    {
        /* sk_sp<SkImage> skimage = m_surface->makeImageSnapshot();
        sk_sp<SkData> filedata = skimage->encodeToData(SkEncodedImageFormat::kPNG, 80);
        std::ofstream file;
        file.open("data.png", std::ios::app | std::ios::binary);
        file.write(reinterpret_cast<const char*>(filedata->data()), filedata->size()); */

        m_context->swapBuffers();
    }

    Vec2f MainWindow_unix::getDPIScale() const 
    {
        // TODO
        return {1.0f, 1.0f};
    }

    bool MainWindow_unix::resize(const Size& size) 
    {
        bool status{false};

        if (size != m_context->getSize()) 
        {
            m_context->resize(size);
            status = true;
        }

        if (size != m_lastSize)
        {
            const unsigned int width{static_cast<unsigned int>(size.width)};
            const unsigned int height{static_cast<unsigned int>(size.height)};

            XResizeWindow(App::Display(), m_window, width, height);
            status = true;
        }
        
        return status;
    }

    bool MainWindow_unix::setLimits(const Size& min, const Size& max) 
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
    }

    bool MainWindow_unix::setTitle(const std::string& name) 
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

    bool MainWindow_unix::setIcon(int32 width, int32 height, byte* pixels)  
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

    void MainWindow_unix::notifyEvent() 
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

    Point MainWindow_unix::getWinPos() const 
    {
        int x{-1}, y{-1};
        ::Window child;

        XTranslateCoordinates(App::Display(), m_window, App::Root(), 0, 0, &x, &y, &child);
        return Point{static_cast<Point::value_type>(x), static_cast<Point::value_type>(y)};
    }

    Size MainWindow_unix::getWinSize() const 
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(App::Display(), m_window, &xwa);

        return Size{static_cast<Size::value_type>(xwa.width), static_cast<Size::value_type>(xwa.height)};
    }

    bool MainWindow_unix::minimize() 
    {
        Display *display{App::Display()};
        XIconifyWindow(display, m_window, m_info.screen);
        XFlush(display);
        return true;
    }

    bool MainWindow_unix::isMinimized() const 
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

    bool MainWindow_unix::restore() 
    {
        Display *display{App::Display()};
        XMapWindow(display, m_window);
        XFlush(display);
        return true;
    }

    bool MainWindow_unix::isFocused() const 
    {
        ::Window focusedWindow;
        int focusState;

        XGetInputFocus(App::Display(), &focusedWindow, &focusState);
        return m_window == focusedWindow;
    }

    bool MainWindow_unix::setScaleHint(const Vec2f& scale) 
    {
        // TODO
        return true;
    }

    std::pair<unsigned long, unsigned char*> MainWindow_unix::getWindowProperty(Atom property, Atom type) const
    {
        Atom realType;
        int realFormat;
        unsigned long left;
        std::pair<unsigned long, unsigned char*> data{};
        XGetWindowProperty(App::Display(), m_window, property, 0L, 2L, False, type, &realType, &realFormat, &data.first, &left, &data.second);
        return data;
    }

    ::Window MainWindow_unix::xWindow() const
    {
        return m_window;
    }

    Atom MainWindow_unix::deleteAtom() const
    {
        return m_atomWmDeleteWindow;
    }

    Size& MainWindow_unix::lastSize()
    {
        return m_lastSize;
    }

    Point& MainWindow_unix::lastPos()
    {
        return m_lastPos;
    }
}
