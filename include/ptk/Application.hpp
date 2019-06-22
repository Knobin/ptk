//
//  Application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// Local Headers
#include "ptk/Window.hpp"

namespace pTK
{
    /** Application class implementation.
     
     This class is to manage windows and handle the whole
     application.
     
     */
    class Application
    {
    public:
        /** Constructs Event with default values.
         
         @param argc    argument count
         @param argv    arguments
         @return        default initialized Transformable
         */
        Application(int argc, char *argv[]);
        ~Application() = default;

        /** Function for executing the application.
         
         @param window  start window
         */
        int exec(Window* window);
    };
}

#endif // PTK_APPLICATION_HPP
