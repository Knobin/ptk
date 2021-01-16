//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "platform/Platform.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/core/Exception.hpp"

// C++ Headers
#include <thread>

namespace pTK
{
    static Application *s_app{nullptr};
    static Ref<MenuBar> s_globalMenu{nullptr}; // Should be a "Menubar" pointer later.

    Application::Application()
        : IterableAssociative<int32, Window*>(), Singleton()
    {
        init();
    }

    Application::Application(int, char* [])
        : IterableAssociative<int32, Window*>(), Singleton()
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
        for (auto it = cbegin(); it != cend(); ++it)
        {
            Window *window{it->second};
            Event evt{Event::Category::Window, Event::Type::WindowClose};
            window->handleEvents(); // Handle all events before sending the close event.
            window->sendEvent(&evt);
        }
        
        // The close event might cause the Window to remove itself from the Application
        // through the Application::Get function.
        // Otherwise, remove them here.
        removeAllWindows();
        
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
        int32 id{1};
        for (const std::pair<const int32, Window*>& pair : *this)
        {
            if (pair.first == id)
            {
                ++id;
            }
        }

        try {
            container().insert({id, window});
            m_appBase->onWindowAdd({id, window});
        } catch (const std::exception&) {
            return -1;
        }

        return id;
    }

    bool Application::removeWindow(Window *window)
    {
        auto it{std::find_if(begin(), end(), [window](const auto& pair) {
            return pair.second == window;
        })};

        if (it != end())
        {
            m_appBase->onWindowRemove(*it);
            container().erase(it);
            return true;
        }

        return false;
    }

    bool Application::removeWindow(int32 key)
    {
        auto it{container().find(key)};
        if (it != container().end())
        {
            m_appBase->onWindowRemove(*it);
            container().erase(it);
            return true;
        }

        return false;
    }

    int Application::run()
    {
        // Currently not supporting running the app without a window.
        if (empty())
        {
            PTK_FATAL("No Window added to Application");
            return -1;
        }

        // Paint all visible windows and handle already pushed events.
        for (const auto&[key, window] : *this)
        {
            if (window->visible())
                window->postEvent<PaintEvent>(Point{0,0}, window->getSize());
            window->handleEvents();
        }

        // Maybe do some setup things here?
        
        // Standard message loop for now.
        while (!empty())
        {
            m_appBase->waitEvents();
            for (const auto& pair : *this)
                pair.second->handleEvents();
        }
        
        return 0;
    }

    void Application::close()
    {
        removeAllWindows();
    }

    void Application::setMenuBar(const Ref<MenuBar>& menubar)
    {
        s_globalMenu = menubar;
    }

    Ref<MenuBar> Application::menuBar() const
    {
        return s_globalMenu;
    }

    Application *Application::Get()
    {
        PTK_ASSERT(s_app, "Application is not initialized");
        return s_app;
    }

    void Application::removeAllWindows()
    {
        for (auto it = cbegin(); it != cend();)
            container().erase(it++);
    }

    Window *Application::findByKey(int32 key) const
    {
        auto it = container().find(key);
        return (it != container().end()) ? it->second : nullptr;
    }
}
