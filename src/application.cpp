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

namespace pTK
{
    Application::Application(int argc, char *argv[])
    {

    }

    int Application::exec(Window* window)
    {
        // Render loop
        while (!window->should_close())
        {
            // Clear.
            glClear(GL_COLOR_BUFFER_BIT);

            // Poll Events
            window->update();

            // Sleep.
            std::this_thread::sleep_for(std::chrono::milliseconds(16));

            // Swap buffers.
            window->swap_buffers();
        }

        return 0;
    }
}