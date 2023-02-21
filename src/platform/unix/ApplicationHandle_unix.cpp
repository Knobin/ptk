//
//  platform/win/Platform_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "ApplicationHandle_unix.hpp"
#include "../../Log.hpp"
#include "../../core/Assert.hpp"
#include "WindowHandle_unix.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/events/KeyMap.hpp"

// C++ Headers
#include <map>

//
// TODO(knobin): Go through this file and check that Window events are handled properly.
//
// There have been changes to the API that might not have been implemented for UNIX (x11)
// backend. For example, closing the application (using the app.close() function does not work).
//

namespace pTK::Platform
{
    namespace AppFactoryImpl
    {
        std::unique_ptr<ApplicationHandle> Make(ApplicationBase* base, std::string_view name)
        {
            return std::make_unique<ApplicationHandle_unix>(base, name);
        }
    } // namespace AppFactoryImpl

    Size& WindowLastSize(WindowHandle_unix* handle)
    {
        return handle->m_lastSize;
    }

    Point& WindowLastPos(WindowHandle_unix* handle)
    {
        return handle->m_lastPos;
    }

    struct AppUnixData
    {
        Display* display{nullptr};
        XContext xcontext{-1};
        int screen{-1};
        ::Window root;
        XIM xim;
        XIC xic;
    };

    static AppUnixData s_appData{};

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    ApplicationHandle_unix::ApplicationHandle_unix(ApplicationBase* base, std::string_view)
        : ApplicationHandle(base)
    {
        XInitThreads();

        s_appData.display = XOpenDisplay(nullptr);
        s_appData.xcontext = XUniqueContext();
        s_appData.screen = DefaultScreen(s_appData.display);
        s_appData.root = RootWindow(s_appData.display, s_appData.screen);
        s_appData.xim = XOpenIM(s_appData.display, 0, 0, 0);
        s_appData.xic = XCreateIC(s_appData.xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL);

        PTK_INFO("Initialized ApplicationHandle_unix");
    }

    ApplicationHandle_unix::~ApplicationHandle_unix()
    {
        XCloseDisplay(s_appData.display);
        PTK_INFO("Destroyed ApplicationHandle_unix");
    }

    void ApplicationHandle_unix::pollEvents()
    {
        XPending(s_appData.display);

        while (QLength(s_appData.display))
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

    void ApplicationHandle_unix::waitEventsTimeout(uint32_t ms)
    {
        ::Display* display{s_appData.display};
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

    Display* ApplicationHandle_unix::Display()
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

    void ApplicationHandle_unix::handleEvent(XEvent* event)
    {
        PTK_ASSERT(event, "Undefined XEvent!");

        WindowHandle_unix* handle{nullptr};
        if (XFindContext(s_appData.display, event->xany.window, s_appData.xcontext,
                         reinterpret_cast<XPointer*>(&handle)) != 0)
            return;

        switch (event->type)
        {
            case Expose:
            {
                PaintEvent evt{Point{0, 0}, handle->getSize()};
                handle->HandlePlatformEvent<PaintEvent>(evt);
                break;
            }
            case DestroyNotify:
            {
                if (auto win = dynamic_cast<Window*>(handle->window()))
                    Application::Get()->removeWindow(win); // Remove window from Application.
                break;
            }
            case ClientMessage:
            {
                XClientMessageEvent* cEvent = reinterpret_cast<XClientMessageEvent*>(event);
                if (cEvent && static_cast<Atom>(cEvent->data.l[0]) == handle->deleteAtom())
                    handle->HandlePlatformEvent<CloseEvent>({});
                break;
            }
            case ButtonPress:
            {
                switch (event->xbutton.button)
                {
                    case Button1:
                    {
                        // TODO(knobin): Check for Left mouse btn virtual key instead of -1.
                        ClickEvent evt{Mouse::Button::Left, -1, {event->xbutton.x, event->xbutton.y}};
                        handle->HandlePlatformEvent<ClickEvent>(evt);
                        break;
                    }
                    case Button4:
                    {
                        handle->HandlePlatformEvent<ScrollEvent>(ScrollEvent{{0.0f, 1.0f}});
                        break;
                    }
                    case Button5:
                    {
                        handle->HandlePlatformEvent<ScrollEvent>(ScrollEvent{{0.0f, -1.0f}});
                        break;
                    }
                }
                break;
            }
            case ButtonRelease:
            {
                if (event->xbutton.button == Button1)
                {
                    // TODO(knobin): Check for Left mouse btn virtual key instead of -1.
                    ReleaseEvent evt{Mouse::Button::Left, -1, {event->xbutton.x, event->xbutton.y}};
                    handle->HandlePlatformEvent<ReleaseEvent>(evt);
                }
                break;
            }
            case MotionNotify:
            {
                MotionEvent mEvt{{static_cast<Point::value_type>(event->xbutton.x),
                                  static_cast<Point::value_type>(event->xbutton.y)}};
                handle->HandlePlatformEvent<MotionEvent>(mEvt);
                break;
            }
            case KeyPress:
            case KeyRelease:
            {
                auto mods = GetKeyModifiers(static_cast<int>(event->xkey.state));
                auto keysym = XLookupKeysym(&event->xkey, 0);
                pTK::Key key{KeyMap::KeyCodeToKey(static_cast<int32_t>(keysym))};
                Event::Type type = (event->type == KeyPress) ? KeyEvent::Pressed : KeyEvent::Released;
                handle->HandlePlatformEvent<KeyEvent>({type, key, mods});

                // Send Input event.
                // TODO(knobin): Should these keys send an InputEvent?
                // Maybe add some blocklist to hinder these.
                if ((type == KeyEvent::Pressed) && (key != Key::Delete) && // Quick fix for now.
                    (key != Key::Backspace) && (key != Key::Enter))
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

                        for (std::size_t i{0}; i < static_cast<std::size_t>(count); ++i)
                            arr[i] = static_cast<uint32_t>(buffer[i]);

                        pTK::InputEvent input{arr, static_cast<std::size_t>(count), pTK::Text::Encoding::UTF32};
                        handle->HandlePlatformEvent<pTK::InputEvent>(input);
                    }
                }

                break;
            }
            case FocusIn:
            {
                if (!((event->xfocus.mode == NotifyGrab) || ((event->xfocus.mode == NotifyUngrab))))
                    handle->HandlePlatformEvent<FocusEvent>({});
                break;
            }
            case FocusOut:
            {
                if (!((event->xfocus.mode == NotifyGrab) || ((event->xfocus.mode == NotifyUngrab))))
                    handle->HandlePlatformEvent<LostFocusEvent>({});
                break;
            }
            case ConfigureNotify:
            {
                // Size change
                Size& wSize{WindowLastSize(handle)};
                if (static_cast<Size::value_type>(event->xconfigure.width) != wSize.width ||
                    static_cast<Size::value_type>(event->xconfigure.height) != wSize.height)
                {
                    wSize.width = static_cast<Size::value_type>(event->xconfigure.width);
                    wSize.height = static_cast<Size::value_type>(event->xconfigure.height);
                    handle->HandlePlatformEvent<ResizeEvent>(ResizeEvent{wSize});
                }

                // Position change
                Point& wPos{WindowLastPos(handle)};
                if (event->xconfigure.x != wPos.x || event->xconfigure.y != wPos.y)
                {
                    wPos.x = static_cast<Point::value_type>(event->xconfigure.x);
                    wPos.y = static_cast<Point::value_type>(event->xconfigure.y);
                    handle->HandlePlatformEvent<MoveEvent>(MoveEvent{wPos});
                }

                break;
            }
            default:
                break;
        }
    }
} // namespace pTK
