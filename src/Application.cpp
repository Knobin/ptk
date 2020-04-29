//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"

#ifdef PTK_PLATFORM_WINDOWS
#include "platform/win/WinPlatform.hpp"
#define PTK_INIT_PLATFORM WinPlatform::init
#define PTK_DESTROY_PLATFORM WinPlatform::destroy
#else
#define PTK_INIT_PLATFORM()
#define PTK_DESTROY_PLATFORM()
#endif

// C++ Headers
#include <thread>

namespace pTK
{
    Application::Application()
        : Singleton(), m_waitTime(8000000)
    {
        PTK_INIT_LOGGING();
        PTK_INIT_PLATFORM();
    }
    
    Application::Application(int, char* [])
        : Singleton(), m_waitTime(8000000)
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
        
        // Render loop.
        while (!window->shouldClose())
        {
            // Events
            window->pollEvents();
            window->handleEvents();

            // To lower cpu usage, we wait.
            std::this_thread::sleep_for(m_waitTime);
        }

        window->hide();

        return 0;
    }
    
    void Application::delayPoll(std::chrono::duration<uint64> const& duration)
    {
        using namespace std::chrono;
        m_waitTime = duration_cast<nanoseconds>(duration);
    }
}
