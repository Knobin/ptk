
//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// Local Headers
#include "platform/Platform.hpp"
#include "platform/PlatformInit.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/core/Exception.hpp"

// C++ Headers
#include <thread>

namespace pTK
{
    // Application class static definitions.
    Application *Application::s_Instance{nullptr};

    Application::Application(const std::string& name)
        : IterableAssociative<int32, Window*>(), SingleObject()
    {
        init(name);
    }

    Application::Application(const std::string& name, int, char* [])
        : IterableAssociative<int32, Window*>(), SingleObject()
    {
        init(name);
        // TODO: Check arguments.
    }

    bool Application::init(const std::string& name)
    {
        if (s_Instance)
            throw ApplicationError("Application already initialized");
        s_Instance = this;

        PTK_INIT_LOGGING();
        AppInit(name);

        PTK_INFO("Initialized Application");
        return true;
    }

    Application::~Application()
    {
        // Close all attached Windows.
        for (const auto & it : *this)
        {
            Window *window{it.second};
            Event evt{Event::Category::Window, Event::Type::WindowClose};
            window->handleEvents(); // Handle all events before sending the close event.
            window->sendEvent(&evt);
        }

        // The close event might cause the Window to remove itself from the Application
        // through the Application::Get function.
        // Otherwise, remove them here.
        removeAllWindows();

        AppDestroy();
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
            AppInstance()->onWindowAdd({id, window});
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
            AppInstance()->onWindowRemove(*it);
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
            AppInstance()->onWindowRemove(*it);
            container().erase(it);
            return true;
        }

        return false;
    }

    int Application::run()
    {
        // Currently not supporting running the app without a window.
        if (container().empty())
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
        while (!container().empty())
        {
            AppInstance()->waitEvents();
            for (const auto& pair : *this)
                pair.second->handleEvents();
        }

        return 0;
    }

    void Application::close()
    {
        removeAllWindows();
    }

    Application *Application::Get()
    {
        PTK_ASSERT(s_Instance, "Application is not initialized");
        return s_Instance;
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
