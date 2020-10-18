//
//  platform/win/Platform_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "Application_unix.hpp"
#include "ptk/core/Exception.hpp"

namespace pTK
{
    static Display *s_display = nullptr;

    Application_unix::Application_unix()
        : ApplicationBase()
    {
        if (!init())
            throw PlatformError("Failed to create Application_unix");
    }

    bool Application_unix::init()
    {
        XInitThreads();
        s_display = XOpenDisplay(nullptr);

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

        while (true) 
        {
            window->forceDrawAll();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            window->handleEvents();
            // window->pollEvents();
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
}