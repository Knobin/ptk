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
        
        */
        virtual void close() = 0;

        /** Function for adding a window to the app.

            Note: returns -1 if an error occured.

            @param window       pointer to the window
            @return         window id
        */
        int32 addWindow(Window *window);

        /** Function for removing a window from the app.

            @param key    window id
            @return       true if removed, otherwise false
        */
        bool removeWindow(int32 key);
        
        [[nodiscard]] container::size_type windowCount() const;

        /** Function for retrieving all windows from the app.

            @return    keys and windows
        */
        [[nodiscard]] const std::map<int32, Window*>& windows() const;

        [[nodiscard]] Window *find(int32 key) const;
        
        virtual int messageLoop() = 0;

        virtual void pollEvents() = 0;
        virtual void waitEvents() = 0;
        virtual void waitEventsTimeout(uint ms) = 0;

    private:
        virtual void onWindowAdd(int32) {}
        virtual void onWindowRemove(int32) {}

    private:
        container m_mainWindows;
    };
}

#endif // PTK_CORE_PLATFORM_APPLICATIONBASE_HPP
