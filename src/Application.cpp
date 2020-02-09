//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"
#include "ptk/util/Clock.hpp"
#include "ptk/Core.hpp"

// C++ Headers
#include <thread>

namespace pTK
{
    Application::Application()
        : Singleton(), m_waitTime(8000000)
    {
        Log::init();
    }
    
    Application::Application(int, char* [])
        : Singleton(), m_waitTime(8000000)
    {
        Log::init();
        // TODO: Check arguments.
    }

    int Application::exec(pTK::WindowBase* window)
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
