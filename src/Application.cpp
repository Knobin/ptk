//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"

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
        
        // Render loop.
        while (!window->shouldClose())
        {
            // Poll Events.
            window->pollEvents();

            // Sleep.
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
        }

        return 0;
    }
}
