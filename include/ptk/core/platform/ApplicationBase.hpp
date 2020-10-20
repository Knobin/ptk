//
//  core/platform/ApplicationBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_CORE_PLATFORM_APPLICATIONBASE_HPP
#define PTK_CORE_PLATFORM_APPLICATIONBASE_HPP

// Local Headers
#include "ptk/Core.hpp"
#include "ptk/util/Singleton.hpp"
#include "ptk/Window.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    /** ApplicationBase class implementation.

        This is the implementation for the Application backend.
        An implementation of this class must exist for the target platform.
    */
    class ApplicationBase : public Singleton
    {
        // typedefs
        using container = std::map<int32, Window*>;
        
    public:
        /** Constructs ApplicationBase with default values.

            @return        default initialized ApplicationBase
        */
        ApplicationBase() = default;

        /** Deconstructor for ApplicationBase

        */
        virtual ~ApplicationBase() = default;

        /** Function for closing the application.
        
            This will close all the windows added to the
            application and terminate. Only use this when
            it is time to end the app.
        */
        virtual void close() = 0;

        /** Function for adding a window to the app.

            Note: returns -1 if an error occured.

            @param window       pointer to the window
            @return             window id
        */
        int32 addWindow(Window *window);

        /** Function for removing a window from the app.

            @param key    window id
            @return       true if removed, otherwise false
        */
        bool removeWindow(int32 key);
        
        /** Function for retrieving the size of the internal table.
         
            @return     table size
        */
        [[nodiscard]] container::size_type windowCount() const;

        /** Function for retrieving all windows from the app.

            @return    keys and windows
        */
        [[nodiscard]] const std::map<int32, Window*>& windows() const;

        /** Function for finding a Window by its key in the table.
         
            @param key  ptk window id
            @return     pointer to the window, may be nullptr
        */
        [[nodiscard]] Window *find(int32 key) const;
        
        /** Message loop for the application.
         
        */
        virtual int messageLoop() = 0;

        /** Function for polling all the window events.
        
        */
        virtual void pollEvents() = 0;
        
        /** Function for waiting for an event.
        
        */
        virtual void waitEvents() = 0;
        
        /** Function for waiting for an event with a timout.
            
            @param ms   max time to wait for an event
        */
        virtual void waitEventsTimeout(uint ms) = 0;

    private:
        /** Callback that will be called when a window is added to the
            application.
         
            @param key  ptk window id
        */
        virtual void onWindowAdd(int32 UNUSED(key)) {}
        
        /** Callback that will be called when a window is removed from the
            application.
         
            @param key  ptk window id
        */
        virtual void onWindowRemove(int32 UNUSED(key)) {}

    private:
        container m_mainWindows;
    };
}

#endif // PTK_CORE_PLATFORM_APPLICATIONBASE_HPP
