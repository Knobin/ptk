//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"
#include "ptk/util/Time.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <thread>
#include <iostream>

namespace pTK
{
    Application::Application(int argc, char *argv[])
    {
        std::cout << "Arguments:\n";
        for (int i = 0; i < argc; i++)
        {
            std::cout << i << ": " << argv[i] << "\n";
        }
    }

    int Application::exec(pTK::Window* window)
    {
        window->show();
        
        Time time;
        
        // Render loop.
        while (!window->shouldClose())
        {
            // Events
            window->pollEvents();
            window->handleEvents();

            // Should not really be here, since we are waiting for events.
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
            
            //PTK_INFO("loop time: {0:d}ms", time.milliseconds());
            time.reset();
        }

        return 0;
    }
}
