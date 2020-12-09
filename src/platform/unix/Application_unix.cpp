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
        std::map<::Window, std::pair<Window*, MainWindow_unix*>> windowMap{};
        Display *display{nullptr};
        XContext xcontext{-1};
    };

    static AppUnixData s_appData{};
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Application_unix::Application_unix()
        : ApplicationBase(),
            m_run{true}
    {
        if (!init())
            throw PlatformError("Failed to initialize unix application");
    }

    bool Application_unix::init()
    {
        XInitThreads();

        s_appData.display = XOpenDisplay(nullptr);
        s_appData.xcontext = XUniqueContext();

        return true;
    }

    Application_unix::~Application_unix()
    {
        XCloseDisplay(s_appData.display);
    }

    int Application_unix::messageLoop()
    {
        Window *window{windows().at(1)};
        PaintEvent evt{Point{0, 0}, window->getSize()};
        window->sendEvent(&evt);
        window->show();
        window->handleEvents();

        while (m_run) 
        {
            waitEvents();
            window->handleEvents();
        }
        // TODO
        return 0;
    }

    void Application_unix::close()
    {
        // TODO
        m_run = false;
    }

    Display *Application_unix::getDisplay()
    {
        return s_appData.display;
    }
    
    XContext Application_unix::getXContext()
    {
        return s_appData.xcontext;
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
        // TODO
        pollEvents();
    }

    void Application_unix::handleEvent(XEvent *event)
    {
        PTK_ASSERT(event, "Undefined XEvent!");

        Window *window{nullptr};
        if (XFindContext(s_appData.display, event->xany.window, s_appData.xcontext, reinterpret_cast<XPointer*>(&window)) != 0)
            return;
            
        switch (event->type)
        {
            case DestroyNotify:
            {
                Event evt{Event::Category::Window, Event::Type::WindowClose};
                window->handleEvents(); // Handle all events before sending close event.
                window->sendEvent(&evt);
                m_run = false;
                break;
            }
            case ClientMessage:
            {
                if (auto uWindow = dynamic_cast<MainWindow_unix*>(window->getBackend()))
                {
                    XClientMessageEvent *cEvent = reinterpret_cast<XClientMessageEvent*>(event);
                    if (cEvent && static_cast<Atom>(cEvent->data.l[0]) == uWindow->deleteAtom())
                        XDestroyWindow(s_appData.display, cEvent->window);
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
            default:
                break;
        }
    }

    void Application_unix::onWindowAdd(int32 key)
    {
        Window *pwindow{find(key)};
        MainWindow_unix *mainwindow{dynamic_cast<MainWindow_unix*>(pwindow->getBackend())};
        if (mainwindow)
        {
            PTK_INFO("Added MainWindow_unix to Application_unix");
            s_appData.windowMap.insert({mainwindow->xWindow(), {pwindow, mainwindow}});
        }    
    }

    void Application_unix::onWindowRemove(int32 key)
    {
        Window *pwindow{find(key)};
        MainWindow_unix *mainwindow{dynamic_cast<MainWindow_unix*>(pwindow->getBackend())};
        if (mainwindow)
        {
            auto it{s_appData.windowMap.find(mainwindow->xWindow())};
            if (it != s_appData.windowMap.end())
            {
                PTK_INFO("Removed MainWindow_unix from Application_unix");
                s_appData.windowMap.erase(it);
            }
        }
    }
}
