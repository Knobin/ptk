//
//  Application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// Local Headers
#include "ptk/core/platform/ApplicationBase.hpp"
#include "ptk/util/Singleton.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Types.hpp"

namespace pTK
{
    /** Application class implementation.

        This class is to manage windows and handle the whole
        application.

    */
    class Application : public Singleton
    {
    public:
        /** Constructs Application with default values.

            @return        default initialized Application
        */
        Application();

        /** Constructs Event with default values.

            @param argc    argument count
            @param argv    arguments
            @return        default initialized Transformable
        */
        Application(int argc, char *argv[]);

        /** Deconstructor for Application

        */
        virtual ~Application();

        /** Function for executing the application.

            @param window  start window
        */
        int exec(Window* window);

        
        /** Function for adding a window to the application.

            @param window   window
            @return         id
        */
        int32 addWindow(Window *window);
        
        /** Function for removing a window from the application with pointer.

            @param window   pointer to window
            @return         true if removed
        */
        bool removeWindow(Window *window);
        
        /** Function for removing a window from the application with id.

            @param key      window id
            @return         true if removed
        */
        bool removeWindow(int32 key);
        
        /** Function for executing the application.
         
        */
        int run();

    private:
        bool init();

    private:
        std::unique_ptr<ApplicationBase> m_appBase;
    };
}

#endif // PTK_APPLICATION_HPP
