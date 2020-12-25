//
//  platform/win/Platform_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "Application_unix.hpp"
#include "MainWindow_unix.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/core/Event.hpp"

// C++ Headers
#include <map>

// X11 Headers
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

namespace pTK
{   
    // 
    // Will probably rework keyboard input at some point, this works for now.
    //

    static std::map<unsigned long, KeyCode> initKeyCodes() noexcept
    {
        std::map<unsigned long, KeyCode> map{};
        map[XK_space] = Key::Space; map[XK_Escape] = Key::Escape;
        map[XK_0] = Key::D0; map[XK_1] = Key::D1; map[XK_2] = Key::D2; map[XK_3] = Key::D3;
        map[XK_4] = Key::D4; map[XK_5] = Key::D5; map[XK_6] = Key::D6; map[XK_7] = Key::D7;
        map[XK_8] = Key::D8; map[XK_9] = Key::D9;
        map[XK_KP_0] = Key::D0; map[XK_KP_1] = Key::D1; map[XK_KP_2] = Key::D2;
        map[XK_KP_3] = Key::D3; map[XK_KP_4] = Key::D4; map[XK_KP_5] = Key::D5;
        map[XK_KP_6] = Key::D6; map[XK_KP_7] = Key::D7; map[XK_KP_8] = Key::D8;
        map[XK_KP_9] = Key::D9;
        map[XK_a] = Key::A; map[XK_b] = Key::B; map[XK_c] = Key::C; map[XK_d] = Key::D;
        map[XK_e] = Key::E; map[XK_f] = Key::F; map[XK_g] = Key::G; map[XK_h] = Key::H;
        map[XK_i] = Key::I; map[XK_j] = Key::J; map[XK_k] = Key::K; map[XK_l] = Key::L;
        map[XK_m] = Key::M; map[XK_n] = Key::N; map[XK_o] = Key::O; map[XK_p] = Key::P;
        map[XK_q] = Key::Q; map[XK_r] = Key::R; map[XK_s] = Key::S; map[XK_t] = Key::T;
        map[XK_u] = Key::U; map[XK_v] = Key::V; map[XK_w] = Key::W; map[XK_x] = Key::X;
        map[XK_y] = Key::Y; map[XK_z] = Key::Z;
        return map;
    }

    static Key translateKeyCode(const std::map<unsigned long, KeyCode> & map, unsigned long code)
    {
        std::map<unsigned long, KeyCode>::const_iterator it{map.find(code)};
        if (it != map.cend())
            return it->second;
        
        return Key::Unknown;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    struct AppUnixData
    {
        const std::map<unsigned long, KeyCode> keyMap{initKeyCodes()};
        Display *display{nullptr};
        XContext xcontext{-1};
        int screen{-1};
        ::Window root;
        bool initialized{false};
    };

    static AppUnixData s_appData{};

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Application_unix::Application_unix()
        : ApplicationBase()
    {
        if (!init())
            throw PlatformError("Failed to initialize unix application");
    }

    Application_unix::~Application_unix()
    {
        XCloseDisplay(s_appData.display);
        PTK_INFO("Destroyed Application_unix");
    }

    int Application_unix::messageLoop()
    {
        while (windowCount() > 0) 
        {
            waitEvents();
            for (const auto& pair : windows())
                pair.second->handleEvents();
        }

        return 0;
    }

    void Application_unix::close()
    {
        auto cont = windows();
        for (auto it = cont.cbegin(); it != cont.cend();)
        {
            Event evt{Event::Category::Window, Event::Type::WindowClose};
            it->second->handleEvents(); // Handle all events before sending close event.
            it->second->sendEvent(&evt);
            it->second->handleEvents();
            cont.erase(it++);
        }
    }

    void Application_unix::pollEvents()
    {
        XPending(s_appData.display);

        while(QLength(s_appData.display))
        {
            XEvent event = {};
            XNextEvent(s_appData.display, &event);
            handleEvent(&event);
        }

        XFlush(s_appData.display);
    }

    void Application_unix::waitEvents()
    {
        XEvent event = {};
        XNextEvent(s_appData.display, &event);
        handleEvent(&event);
        pollEvents();
    }

    void Application_unix::waitEventsTimeout(uint ms)
    {
        ::Display *display{s_appData.display};
        XEvent event = {};
        bool evtFound{false};

        timeval timeout{};
        timeout.tv_sec = ms / 1000;
        timeout.tv_usec = (ms * 1000) - (timeout.tv_sec * 1000000);

        if (XPending(display) == 0)
        {
            int fd{ConnectionNumber(display)};
            fd_set readset;
            FD_ZERO(&readset);
            FD_SET(fd, &readset);
            if (select(fd + 1, &readset, nullptr, nullptr, &timeout) > 0)
            {
                XNextEvent(display, &event);
                evtFound = true;
            }
        }
        else
        {
            XNextEvent(display, &event);
            evtFound = true;
        }
        
        if (evtFound)
        {
            handleEvent(&event);
            pollEvents();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Display *Application_unix::Display()
    {
        return s_appData.display;
    }
    
    XContext Application_unix::Context()
    {
        return s_appData.xcontext;
    }

    ::Window Application_unix::Root()
    {
        return s_appData.root;
    }

    int Application_unix::Screen()
    {
        return s_appData.screen;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    bool Application_unix::init()
    {
        if (s_appData.initialized) 
        {
            PTK_ERROR("App already initialized");
            return false;
        }

        XInitThreads();

        s_appData.display = XOpenDisplay(nullptr);
        s_appData.xcontext = XUniqueContext();
        s_appData.screen = DefaultScreen(s_appData.display);
        s_appData.root = RootWindow(s_appData.display, s_appData.screen);

        s_appData.initialized = true;
        PTK_INFO("Initialized Application_unix");
        return true;
    }

    void Application_unix::handleEvent(XEvent *event)
    {
        PTK_ASSERT(event, "Undefined XEvent!");

        Window *window{nullptr};
        if (XFindContext(s_appData.display, event->xany.window, s_appData.xcontext, reinterpret_cast<XPointer*>(&window)) != 0)
            return;
            
        switch (event->type)
        {
            case Expose:
            {
                window->postEvent<PaintEvent>(Point{0, 0}, window->getSize());
                break;
            }
            case DestroyNotify:
            {
                window->handleEvents(); // Handle all events before sending close event.
                removeWindow(window); // Remove window from Application.
                break;
            }
            case ClientMessage:
            {
                if (auto uWindow = dynamic_cast<MainWindow_unix*>(window->getBackend()))
                {
                    XClientMessageEvent *cEvent = reinterpret_cast<XClientMessageEvent*>(event);
                    if (cEvent && static_cast<Atom>(cEvent->data.l[0]) == uWindow->deleteAtom())
                    {
                        Event evt{Event::Category::Window, Event::Type::WindowClose};
                        window->handleEvents(); // Handle all events before sending close event.
                        window->sendEvent(&evt);
                    }
                }
                break;
            }
            case ButtonPress:
            {
                switch (event->xbutton.button)
                {
                    case Button1:
                    {
                        ButtonEvent bEvt{Event::Type::MouseButtonPressed, Mouse::Button::Left, {event->xbutton.x, event->xbutton.y}};
                        window->sendEvent(&bEvt);
                        break;
                    }
                    case Button4:
                    {
                        ScrollEvent sEvt{{0.0f, 1.0f}};
                        window->sendEvent(&sEvt);
                        break;
                    }
                    case Button5:
                    {
                        ScrollEvent sEvt{{0.0f, -1.0f}};
                        window->sendEvent(&sEvt);
                        break;
                    }
                }
                break;
            }
            case ButtonRelease:
            {
                if (event->xbutton.button == Button1)
                {
                    ButtonEvent bEvt{Event::Type::MouseButtonReleased, Mouse::Button::Left, {event->xbutton.x, event->xbutton.y}};
                    window->sendEvent(&bEvt);
                }
                break;
            }
            case MotionNotify:
            {
                MotionEvent mEvt{{static_cast<Point::value_type>(event->xbutton.x), 
                                    static_cast<Point::value_type>(event->xbutton.y)}};
                window->sendEvent(&mEvt);
                break;
            }
            case KeyPress:
            {
                KeyEvent kEvt{KeyEvent::Pressed, translateKeyCode(s_appData.keyMap, XLookupKeysym(&event->xkey, 0))};
                window->sendEvent(&kEvt);
                break;
            }
            case KeyRelease:
            {
                KeyEvent kEvt{KeyEvent::Released, translateKeyCode(s_appData.keyMap, XLookupKeysym(&event->xkey, 0))};
                window->sendEvent(&kEvt);
                break;
            }
            case FocusIn:
            {
                if (!((event->xfocus.mode == NotifyGrab) || ((event->xfocus.mode == NotifyUngrab))))
                {
                    Event evt{Event::Category::Window, Event::Type::WindowFocus};
                    window->sendEvent(&evt);
                }
                break;
            }
            case FocusOut:
            {
                if (!((event->xfocus.mode == NotifyGrab) || ((event->xfocus.mode == NotifyUngrab))))
                {
                    Event evt{Event::Category::Window, Event::Type::WindowLostFocus};
                    window->sendEvent(&evt);
                }
                break;
            }
            case ConfigureNotify:
            {
                MainWindow_unix *backend{static_cast<MainWindow_unix*>(window->getBackend())};

                // Size change
                Size& wSize{backend->lastSize()};
                if (event->xconfigure.width != wSize.width || event->xconfigure.height != wSize.height)
                {
                    wSize.width = static_cast<Size::value_type>(event->xconfigure.width);
                    wSize.height = static_cast<Size::value_type>(event->xconfigure.height);

                    ResizeEvent evt{wSize};
                    window->sendEvent(&evt);
                    window->forceDrawAll();

                }

                // Position change
                Point& wPos{backend->lastPos()};
                if (event->xconfigure.x != wPos.x || event->xconfigure.y != wPos.y)
                {
                    wPos.x = static_cast<Point::value_type>(event->xconfigure.x);
                    wPos.y = static_cast<Point::value_type>(event->xconfigure.y);

                    MoveEvent evt{wPos};
                    window->sendEvent(&evt);
                }

                break;
            }
            default:
                break;
        }
    }
}
