//
//  application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// Local Headers
#include "ptk/window.hpp"

namespace pTK
{
    class Application
    {
    public:
        Application(int argc, char *argv[]);
        ~Application() = default;

        int exec(Window* window);

    };
}

#endif // PTK_APPLICATION_HPP
