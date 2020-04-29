//
//  Application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// Local Headers
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
        /** Constructs Event with default values.
         
            @return        default initialized Transformable
        */
        Application();
        
        /** Constructs Event with default values.
         
            @param argc    argument count
            @param argv    arguments
            @return        default initialized Transformable
        */
        Application(int argc, char *argv[]);
        ~Application();

        /** Function for executing the application.
         
            @param window  start window
        */
        int exec(Window* window);
        
        /** Function for setting the delay between polling events.
         
            Default is currently 8ms, about 120 times / second.
         
            @param duration  time to wait
        */
        void delayPoll(std::chrono::duration<uint64> const& duration);
        
        // TODO: Menubar
        // TODO: Thread safe option
        // TODO: Support multiple windows
        // TODO: window id
        
    private:
        std::chrono::nanoseconds m_waitTime;
    };
}

#endif // PTK_APPLICATION_HPP
