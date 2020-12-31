//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "ptk/Application.hpp"
#include "platform/Platform.hpp"
#include "ptk/core/Exception.hpp"

// C++ Headers
#include <thread>

namespace pTK
{
    static Application *s_app{nullptr};

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
        if (s_app)
            throw ApplicationError("Application already initialized");
        s_app = this;
        
        PTK_INIT_LOGGING();
        m_appBase = std::make_unique<PTK_APPLICATION_TYPE>();
        
        PTK_INFO("Initialized Application");
        return true;
    }

    Application::~Application()
    {
        // Close all attached Windows.
        auto cont = m_appBase->windows();
        for (auto it = cont.cbegin(); it != cont.cend(); ++it)
        {
            Window *window{it->second};
            Event evt{Event::Category::Window, Event::Type::WindowClose};
            window->handleEvents(); // Handle all events before sending the close event.
            window->sendEvent(&evt);
        }
        
        // The close event might cause the Window to remove itself from the Application
        // through the Application::Get function.
        // Otherwise, remove them here.
        m_appBase->removeAllWindows();
        
        PTK_INFO("Destroyed Application");
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
        return m_appBase->removeWindow(window);
    }

    bool Application::removeWindow(int32 key)
    {
        return m_appBase->removeWindow(key);
    }

    int Application::run()
    {
        // Currently not supporting running the app without a window.
        if (m_appBase->windowCount() == 0)
        {
            PTK_FATAL("No Window added to Application");
            return -1;
        }
        
        // Maybe do some setup things here?
        
        // Standard message loop for now.
        while (m_appBase->windowCount() > 0)
        {
            m_appBase->waitEvents();
            for (const auto& pair : m_appBase->windows())
                pair.second->handleEvents();
        }
        
        return 0;
    }

    void Application::close()
    {
        m_appBase->removeAllWindows();
    }

    Application *Application::Get()
    {
        PTK_ASSERT(s_app, "Application is not initialized");
        return s_app;
    }

}
