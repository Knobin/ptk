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
    static Display *s_display = nullptr;
    static XContext s_xcontext = -1;
    static std::map<::Window, std::pair<Window*, MainWindow_unix*>> s_windowMap = {};

    Application_unix::Application_unix()
        : ApplicationBase(),
            m_run{true}
    {
        if (!init())
            throw PlatformError("Failed to create Application_unix");
    }

    bool Application_unix::init()
    {
        XInitThreads();
        s_display = XOpenDisplay(nullptr);
        s_xcontext = XUniqueContext();

        return true;
    }

    Application_unix::~Application_unix()
    {
        XCloseDisplay(s_display);
    }

    int Application_unix::messageLoop()
    {
        Window *window{windows().at(1)};
        PaintEvent evt{Point{0, 0}, window->getSize()};
        window->sendEvent(&evt);
        window->show();

        while (m_run) 
        {
            window->handleEvents();
            pollEvents();
        }
        // TODO
        return 0;
    }

    void Application_unix::close()
    {
        // TODO
    }

    Display *Application_unix::getDisplay()
    {
        return s_display;
    }
    
    XContext Application_unix::getXContext()
    {
        return s_xcontext;
    }

    void Application_unix::pollEvents()
    {
        XPending(s_display);

        bool close = false;
        while(QLength(s_display))
        {
            XEvent event = {};
            XNextEvent(s_display, &event);
            
            Window *window{nullptr};
            if (XFindContext(s_display, event.xany.window, s_xcontext, reinterpret_cast<XPointer*>(&window)) != 0)
                continue;
                
            switch (event.type)
            {
            case DestroyNotify:
            {
                // XDestroyWindowEvent *dEvent = reinterpret_cast<XDestroyWindowEvent*>(&event);
                Event evt{Event::Category::Window, Event::Type::WindowClose};
                window->handleEvents(); // Handle all events before sending close event.
                window->sendEvent(&evt);
                close = true; // TODO: Should not really be here.
                break;
            }
            case ClientMessage:
            {
                PTK_INFO("TEST SADSAD");
                if (auto uWindow = dynamic_cast<MainWindow_unix*>(window->getBackend()))
                {
                    XClientMessageEvent *cEvent = reinterpret_cast<XClientMessageEvent*>(&event);
                    if (static_cast<Atom>(cEvent->data.l[0]) == uWindow->deleteAtom())
                    {
                        XDestroyWindow(s_display, cEvent->window);
                    }
                }
                break;
            }
            case ButtonPress:
            {
                
                break;
            }
            default:
                break;
            }
        }

        if (close)
            m_run = false;

        XFlush(s_display);
    }

    void Application_unix::waitEvents()
    {
        // TODO
        pollEvents();
    }

    void Application_unix::waitEventsTimeout(uint ms)
    {
        // TODO
        pollEvents();
    }

    void Application_unix::onWindowAdd(int32 key)
    {
        Window *pwindow{find(key)};
        MainWindow_unix *mainwindow{dynamic_cast<MainWindow_unix*>(pwindow->getBackend())};
        if (mainwindow)
        {
            PTK_INFO("Added MainWindow_unix to Application_unix");
            s_windowMap.insert({mainwindow->xWindow(), {pwindow, mainwindow}});
        }    
    }

    void Application_unix::onWindowRemove(int32 key)
    {
        Window *pwindow{find(key)};
        MainWindow_unix *mainwindow{dynamic_cast<MainWindow_unix*>(pwindow->getBackend())};
        if (mainwindow)
        {
            auto it{s_windowMap.find(mainwindow->xWindow())};
            if (it != s_windowMap.end())
            {
                PTK_INFO("Removed MainWindow_unix from Application_unix");
                s_windowMap.erase(it);
            }
        }
    }
}
