
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
        closeWindows();

        // The close event might cause the Window to remove itself from the Application
        // through the Application::Get function.
        // Otherwise, remove them here.
        clearWindows();

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
        PTK_ASSERT(window, "Undefined window");

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
        PTK_ASSERT(window, "Undefined window");

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
        PTK_ASSERT(key > 0, "Invalid key");

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

        if (container().empty() && !m_allowHeadless)
        {
            // No Windows and no permission for headless mode is not allowed.
            close();
        }
    }

    int Application::headlessMessageLoop()
    {
        PTK_ASSERT(m_allowHeadless, "Running headless is not allowed");
        PTK_ASSERT(container().empty(), "Window cannot be added to Application while in headless mode");

        // Runtime check for permission.
        if (!m_allowHeadless)
            return -1;

        // Application is in headless mode inside this scope.
        {
            m_runningHeadless = true;

            // Message loop.
            // m_allowHeadless might change and cannot be considered a constant
            // in this function (it must be true when entered the function, but
            // can be changed later in the runtime).
            PTK_INFO("Running Headless Message Loop");
            while (m_allowHeadless && container().empty() && !isClosed())
                fetchEvents();

            m_runningHeadless = false;
        }

        return 0;
    }

    int Application::standardMessageLoop()
    {
        PTK_ASSERT(!container().empty(), "No Window added to Application");
        PTK_INFO("Running Standard Message Loop");

        // Paint all visible windows and handle already pushed events.
        for (const auto& [key, window] : *this)
        {
            if (window->visible())
                window->invalidate();

            window->handleEvents();
        }

        // Maybe do some setup things here?

        // Standard message loop for now.
        while (!container().empty() && !isClosed())
        {
            fetchEvents();
            for (const auto& pair : *this)
                pair.second->handleEvents();
        }

        return 0;
    }

    void Application::fetchEvents()
    {
        // Waiting for events is default for now.
        waitEvents();

        // This function should check which function to use.
        // (pollEvents, waitEvents, waitEventsTimeout).

        // Application might implement a command queue in the future,
        // so a selection is needed.
    }

    int Application::run()
    {
        int returnValue{0};

        while (!isClosed())
        {
            // Run message loop and retrieve return value.
            if (m_allowHeadless && container().empty())
                returnValue = headlessMessageLoop();
            else if (!container().empty())
                returnValue = standardMessageLoop();
            else
            {
                PTK_WARN("Application is not allowed to run in headless mode and contains no windows.");
                break; // Can't run in headless and no Windows.
            }


            // If return value is non zero, indicate error and close application.
            // Otherwise check while condition again (window might have been added/removed).
            if (returnValue != 0)
                break;
        }

        return returnValue;
    }

    bool Application::close()
    {
        if (isClosed())
        {
            PTK_WARN("Application is already closed");
            return true;
        }

        if (onCloseRequest())
        {
            PTK_INFO("Application is closing...");
            onClose();
            closeWindows();
            clearWindows();
            onApplicationClose();
            m_closed = true;
            PTK_INFO("Application closed");
            return true;
        }

        PTK_INFO("Application halted close request");
        return false;
    }

    Application* Application::Get()
    {
        PTK_ASSERT(s_Instance, "Application is not initialized");
        return s_Instance;
    }

    void Application::clearWindows()
    {
        for (auto it = cbegin(); it != cend(); ++it)
            onWindowRemove(it->first, it->second);

        container().clear();
    }

    void Application::closeWindows()
    {
        auto it = cbegin();
        while (it != cend())
        {
            // Window close might remove itself from the application
            // and alter the container (invalidating the iterator).
            it->second->close();
            it = cbegin();
        }
    }

} // namespace pTK
