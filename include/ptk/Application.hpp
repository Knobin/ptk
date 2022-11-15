//
//  Application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/platform/Platform.hpp"

namespace pTK
{
    /** Application class implementation.

        This class is for adding/removing windows and to
        setup the Application and its runtime environment.

        The actual event handling functions are in the
        inherited class.
    */
    class PTK_API Application : public PTK_APPLICATION_HANDLE_T
    {
    public:
        /** Constructs Application with default values.

            @return        default initialized Application
        */
        Application() = delete;

        /** Constructs Application with default values.

            @param name    application name
            @return        default initialized Application
        */
        explicit Application(std::string_view name);

        /** Constructs Event with default values.

            @param name    application name
            @param argc    argument count
            @param argv    arguments
            @return        default initialized Transformable
        */
        Application(std::string_view name, int argc, char* argv[]);

        /** Destructor for Application

        */
        virtual ~Application();

        /** Function for executing the application with a single window.

            The Window should not have been added with addWindow() before this call.

            @param window  start window
        */
        int exec(Window* window);

        /** Function for adding a window to the application.

            @param window   window
            @return         id
        */
        key_type addWindow(Window* window);

        /** Function for removing a window from the application with pointer.

            @param window   pointer to window
            @return         true if removed
        */
        bool removeWindow(Window* window);

        /** Function for removing a window from the application with id.

            @param key      window id
            @return         true if removed
        */
        bool removeWindow(key_type key);

        /** Function for removing all windows in the application.

            Internally calls removeWindow() on all windows.
        */
        void removeAllWindows();

        /** Function for executing the application.

            Runs the application with the already added windows.
            Note: Windowless application is not currently supported.
        */
        int run();

        /** Function for closing the application.

            Note: Should only be called if the application should be
                  stopped but not destructed yet.
        */
        void close();

    public:
        /** Function for retrieving a pointer to the Application.

            Should only be called after the Application has been created.
            Otherwise, no instance exists and is nullptr.

            @return     pointer to application instance
        */
        static Application* Get();

    private:
        /** Function for initializing the Application.

            Should only be called once when the Application has been created.
            More than one call to this function will result in a ApplicationError
            exception being thrown.

            @return     status
        */
        bool init();

        /** Helper function for removing a window from the Application.

            Calls the callback for removing a window and the removes it.

            @param it   iterator to be erased
        */
        void eraseWindow(const_iterator it);

    private:
        static Application* s_Instance;
    };
} // namespace pTK

#endif // PTK_APPLICATION_HPP
