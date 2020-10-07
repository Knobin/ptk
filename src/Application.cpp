//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"
#include "platform/Platform.hpp"

// C++ Headers
#include <thread>

namespace pTK
{
    Application::Application()
        : Singleton()
    {
        init();
    }

    Application::Application(int, char* [])
        : Singleton()
    {
        init();
        // TODO: Check arguments.
    }

    bool Application::init()
    {
        PTK_INIT_LOGGING();
        m_appBase = std::make_unique<PTK_APPLICATION_TYPE>();
        
        return true;
    }

    Application::~Application()
    {

    }

    int Application::exec(Window *window)
    {
        PTK_ASSERT(window, "Undefined window");
        
        addWindow(window);
        return run();
    }

    int32 Application::addWindow(Window *window)
    {
        return m_appBase->addWindow(window);
    }

    bool Application::removeWindow(Window *window)
    {
        int32 key{-1};
        for (const std::pair<int32, Window*>& pair : m_appBase->windows())
            if (pair.second == window)
                key = pair.first;
        
        if (key > -1)
            return m_appBase->removeWindow(key);
        
        return false;
        
    }

    bool Application::removeWindow(int32 key)
    {
        return m_appBase->removeWindow(key);
    }

    int Application::run()
    {
        if (m_appBase->windowCount() == 0)
        {
            PTK_FATAL("No Window added to Application");
            return 0;
        }
        
        // Maybe do some setup things here?
        
        
        return m_appBase->messageLoop();
    }

}
