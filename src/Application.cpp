
//
//  Application.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/core/Exception.hpp"

namespace pTK
{
    // Application class static definitions.
    Application* Application::s_Instance{nullptr};

    Application::Application(std::string_view name)
        : PTK_APPLICATION_HANDLE_T(name)
    {
        init();
    }

    Application::Application(std::string_view name, int, char*[])
        : PTK_APPLICATION_HANDLE_T(name)
    {
        init();
        // TODO: Check arguments.
    }

    bool Application::init()
    {
        if (s_Instance)
            throw ApplicationError("Application already initialized");
        s_Instance = this;

        PTK_INFO("Initialized Application");
        return true;
    }

    Application::~Application()
    {
        // Close all attached Windows.
        for (const auto& it : *this)
        {
            Window* window{it.second};
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

    int Application::exec(Window* window)
    {
        PTK_ASSERT(window, "Undefined window");

        addWindow(window);
        return run();
    }

    Application::key_type Application::addWindow(Window* window)
    {
        key_type id{1};
        for (const auto& p : *this)
            if (p.first == id)
                ++id;

        try
        {
            if (uniqueInsert(id, window))
                onWindowAdd(id, window);
            else
                return -1;
        }
        catch (const std::exception&)
        {
            return -1;
        }

        return id;
    }

    bool Application::removeWindow(Window* window)
    {
        auto it{firstByWindow(window)};
        if (it != cend())
        {
            eraseWindow(it);
            return true;
        }

        return false;
    }

    bool Application::removeWindow(key_type key)
    {
        auto it{firstByKey(key)};
        if (it != cend())
        {
            eraseWindow(it);
            return true;
        }

        return false;
    }

    void Application::eraseWindow(const_iterator it)
    {
        onWindowRemove(it->first, it->second);
        eraseByIter(it);
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
        for (const auto& [key, window] : *this)
        {
            if (window->visible())
            {
                // window->postEvent<PaintEvent>(Point{0, 0}, window->getSize());
                window->invalidate();
            }
                
            window->handleEvents();
        }

        // Maybe do some setup things here?

        // Standard message loop for now.
        while (!container().empty())
        {
            waitEvents();
            for (const auto& pair : *this)
                pair.second->handleEvents();
        }

        return 0;
    }

    void Application::close()
    {
        onClose();
        removeAllWindows();
    }

    Application* Application::Get()
    {
        PTK_ASSERT(s_Instance, "Application is not initialized");
        return s_Instance;
    }

    void Application::removeAllWindows()
    {
        for (auto it = cbegin(); it != cend();)
            onWindowRemove(it->first, it->second);

        container().clear();
    }

} // namespace pTK
