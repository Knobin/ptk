//
//  application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/application.hpp"

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
        // Render loop.
        while (!window->should_close())
        {
            // Clear.
            glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // Poll Events.
            window->update();
            
            // Draw.
            window->draw();
            
            // Swap buffers.
            window->swap_buffers();

            // Sleep.
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        return 0;
    }
}
