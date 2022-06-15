//
//  Application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// pTK Headers
#include "ptk/util/SingleObject.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Types.hpp"
#include "ptk/util/IterableContainer.hpp"

// C++ Headers
#include <vector>
#include <utility>

namespace pTK
{
    /** Application class implementation.

        This class is to manage windows and handle the whole
        application.

    */
    class PTK_API Application : public IterableAssociative<int32, Window*>, public SingleObject
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
        Application(const std::string& name);

        /** Constructs Event with default values.

            @param name    application name
            @param argc    argument count
            @param argv    arguments
            @return        default initialized Transformable
        */
        Application(const std::string& name, int argc, char *argv[]);

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
        
        void removeAllWindows();
        
        [[nodiscard]] Window *findByKey(int32 key) const;
        
        /** Function for executing the application.
         
        */
        int run();
        
        void close();
        
    public:
        static Application *Get();
        static Application *s_Instance;

    private:
        bool init(const std::string& name);
    };
}

#endif // PTK_APPLICATION_HPP
