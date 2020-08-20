//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"
#include "platform/Platform.hpp"

// C++ Headers
#include <thread>

namespace pTK
{
    Application::Application()
        : Singleton()
    {
        PTK_INIT_LOGGING();
        PTK_INIT_PLATFORM();
    }

    Application::Application(int, char* [])
        : Singleton()
    {
        PTK_INIT_LOGGING();
        PTK_INIT_PLATFORM();
        // TODO: Check arguments.
    }

    Application::~Application()
    {
        PTK_DESTROY_PLATFORM();
    }

    int Application::exec(pTK::Window* window)
    {
        PTK_ASSERT(window, "Window is nullptr");

        window->show();

        // Event loop.
        while (!window->shouldClose())
        {
            // Events
            //std::this_thread::sleep_for(std::chrono::milliseconds{m_waitTime});
            window->waitEvents();
            window->handleEvents();
        }

        window->hide();

        return 0;
    }

    void Application::delayPoll(uint duration)
    {
        m_waitTime = duration;
    }
}
