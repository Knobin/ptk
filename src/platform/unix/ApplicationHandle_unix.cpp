//
//  platform/win/Platform_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// pTK Headers
#include "ptk/platform/unix/ApplicationHandle_unix.hpp"
#include "ptk/platform/unix/WindowHandle_unix.hpp"
#include "ptk/Application.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/events/KeyMap.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    struct AppUnixData
    {
        Display *display{nullptr};
        XContext xcontext{-1};
        int screen{-1};
        ::Window root;
        bool initialized{false};
        XIM xim;
        XIC xic;
    };

    static AppUnixData s_appData{};

    // ApplicationHandle_unix class static definitions.
    ApplicationHandle_unix ApplicationHandle_unix::s_Instance{};

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    void ApplicationHandle_unix::Init(const std::string&)
    {
        if (s_appData.initialized)
        {
            PTK_ERROR("App already initialized");
            return;
        }

        XInitThreads();

        s_appData.display = XOpenDisplay(nullptr);
        s_appData.xcontext = XUniqueContext();
        s_appData.screen = DefaultScreen(s_appData.display);
        s_appData.root = RootWindow(s_appData.display, s_appData.screen);
        s_appData.xim = XOpenIM(s_appData.display, 0, 0, 0);
        s_appData.xic = XCreateIC(s_appData.xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL);

        s_appData.initialized = true;
        PTK_INFO("Initialized ApplicationHandle_unix");
    }

    void ApplicationHandle_unix::Destroy()
    {
        XCloseDisplay(s_appData.display);
        PTK_INFO("Destroyed ApplicationHandle_unix");
    }

    ApplicationHandle_unix *ApplicationHandle_unix::Instance()
    {
        return &s_Instance;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    void ApplicationHandle_unix::pollEvents()
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

    void ApplicationHandle_unix::waitEvents()
    {
        XEvent event = {};
        XNextEvent(s_appData.display, &event);
        handleEvent(&event);
        pollEvents();
    }

    void ApplicationHandle_unix::waitEventsTimeout(uint ms)
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

    Display *ApplicationHandle_unix::Display()
    {
        return s_appData.display;
    }

    XContext ApplicationHandle_unix::Context()
    {
        return s_appData.xcontext;
    }

    ::Window ApplicationHandle_unix::Root()
    {
        return s_appData.root;
    }

    int ApplicationHandle_unix::Screen()
    {
        return s_appData.screen;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    static std::underlying_type<KeyEvent::Modifier>::type GetKeyModifiers(int state)
    {
        using utype = std::underlying_type<KeyEvent::Modifier>::type;
        utype mods = static_cast<utype>(KeyEvent::Modifier::NONE);

        if (state & ShiftMask)
            mods |= static_cast<utype>(KeyEvent::Modifier::Shift);

        if (state & ControlMask)
            mods |= static_cast<utype>(KeyEvent::Modifier::Control);

        if (state & Mod1Mask)
            mods |= static_cast<utype>(KeyEvent::Modifier::Alt);

        if (state & Mod4Mask)
            mods |= static_cast<utype>(KeyEvent::Modifier::Super);

        if (state & LockMask)
            mods |= static_cast<utype>(KeyEvent::Modifier::CapsLock);

        if (state % Mod2Mask)
            mods |= static_cast<utype>(KeyEvent::Modifier::NumLock);

        return mods;
    }

    void ApplicationHandle_unix::handleEvent(XEvent *event)
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
                Application::Get()->removeWindow(window); // Remove window from Application.
                break;
            }
            case ClientMessage:
            {
                if (auto uWindow = dynamic_cast<WindowHandle_unix*>(window->getHandle()))
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
            case KeyRelease:
            {
                auto mods = GetKeyModifiers(event->xkey.state);
                auto keysym = XLookupKeysym(&event->xkey, 0);
                pTK::Key key{KeyMap::KeyCodeToKey(keysym)};
                Event::Type type = (event->type == KeyPress) ? KeyEvent::Pressed : KeyEvent::Released;
                KeyEvent kEvt{type, key, mods};
                window->sendEvent(&kEvt);

                // Send Input event.
                if ((type == KeyEvent::Pressed) &&
                    (key != Key::Delete) && // Quick fix for now.
                    (key != Key::Backspace))
                {
                    char buffer[32];
                    KeySym ignore;
                    x11::Status return_status;
                    int count = {0};
                    count = Xutf8LookupString(s_appData.xic, &event->xkey, buffer, 32, &ignore, &return_status);
                    if (count)
                    {
                        PTK_INFO("INPUT EVENT: {} {}", buffer, count);
                        pTK::InputEvent::data_cont arr(new pTK::InputEvent::data_type[static_cast<std::size_t>(count)]);

                        for (std::size_t i{0}; i < count; ++i)
                            arr[i] = static_cast<uint32>(buffer[i]);

                        pTK::InputEvent input{arr, static_cast<std::size_t>(count), pTK::Text::Encoding::UTF32};
                        window->sendEvent(&input);
                    }
                }

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
                WindowHandle_unix *backend{static_cast<WindowHandle_unix*>(window->getHandle())};

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
