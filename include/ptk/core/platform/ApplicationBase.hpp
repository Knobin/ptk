//
//  core/platform/ApplicationBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_CORE_PLATFORM_APPLICATIONBASE_HPP
#define PTK_CORE_PLATFORM_APPLICATIONBASE_HPP

// pTK Headers
#include "ptk/Core.hpp"
#include "ptk/util/SingleObject.hpp"
#include "ptk/Window.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    /** ApplicationBase class implementation.

        This is the implementation for the Application backend.
        An implementation of this class must exist for the target platform.
    */
    class ApplicationBase : public SingleObject
    {
    public:
        /** Constructs ApplicationBase with default values.

            @return        default initialized ApplicationBase
        */
        ApplicationBase() = default;

        /** Deconstructor for ApplicationBase

        */
        virtual ~ApplicationBase() = default;

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
        
        virtual void onClose() {}

        /** Callback that will be called when a window is added to the
            application.
         
            @param key  ptk window id
        */
        virtual void onWindowAdd(const std::pair<int32, Window*> UNUSED(item)) {}
        
        /** Callback that will be called when a window is removed from the
            application.
         
            @param key  ptk window id
        */
        virtual void onWindowRemove(const std::pair<int32, Window*> UNUSED(item)) {}
    };
}

#endif // PTK_CORE_PLATFORM_APPLICATIONBASE_HPP
