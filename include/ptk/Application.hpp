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

// C++ Headers
#include <cstdint>
#include <map>
#include <utility>

namespace pTK
{
    /** Application class implementation.

        This class is to manage windows and handle the whole
        application.

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

        /** Function for executing the application.

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

        */
        void removeAllWindows();

        /** Function for executing the application.

        */
        int run();

        /** Function for closing the application.

            Note: Should only be called if the application should be
                  stopped but not destructed yet.
        */
        void close();

    public:
        static Application* Get();
        static Application* s_Instance;

    private:
        bool init();
        void eraseWindow(const_iterator it);
    };
} // namespace pTK

#endif // PTK_APPLICATION_HPP
