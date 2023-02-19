
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
    // Event polling helpers.
    constexpr int WaitForEvents = -1;
    constexpr int PollEvents = 0;
    constexpr int WaitTimeoutForEvents(int ms) noexcept
    {
        return ms;
    }

    // Application class static definitions.
    Application* Application::s_Instance{nullptr};

    Application::Application(std::string_view name)
        : ApplicationBase(name)
    {
        init(name);
    }

    Application::Application(std::string_view name, int, char*[])
        : ApplicationBase(name)
    {
        init(name);
        // TODO: Check arguments.
    }

    bool Application::init(std::string_view name)
    {
        if (s_Instance)
            throw ApplicationError("Application already initialized");

        s_Instance = this;
        m_handle = Platform::ApplicationHandle::Make(this, name);
        PTK_ASSERT(m_handle, "Undefined Platform::ApplicationHandle");

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

        // Terminate if not already done.
        if (!m_terminated && isAutoTerminationAllowed())
        {
            PTK_INFO("Auto terminating Application...");
            m_handle->terminate();
            PTK_INFO("Application terminated.");
        }

        PTK_INFO("Destroyed Application");
    }

    void Application::terminate()
    {
        PTK_INFO("Terminating Application...");

        // macOS metal context gives access violation if no event polling has
        // happen before it is destroyed. It might also be a good idea to poll
        // existing events before terminating as well. Although it can also be
        // considered that "terminating" should terminate instantly. This is fine
        // for now since this function should allow for the ability to cleanup before
        // actually terminating.
        m_handle->pollEvents();

        closeHelper();
        m_handle->terminate();
        m_terminated = true;
        PTK_INFO("Application terminated.");
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
            {
                PTK_INFO("Added window \"{}\" with key {}.", window->getName(), id);
                m_handle->onWindowAdd(id, window);
            }
            else
            {
                PTK_WARN("Could not insert window \"{}\" with key {}. Either key or window already exists.",
                         window->getName(), id);
                return -1;
            }
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
        PTK_INFO("Removing window \"{}\"", it->second->getName());
        m_handle->onWindowRemove(it->first, it->second);
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
                fetchEvents(-1);

            m_runningHeadless = false;
        }

        return 0;
    }

    static int WindowEventFrame(Window* window)
    {
        window->runCommands();

        if (window->isContentValid())
            return WaitForEvents; // Window can wait indefinitely here.

        const std::size_t timeSinceDraw = window->timeSinceLastDraw();
        const std::size_t frameTime = 1000 / window->targetRefreshRate();

        if (timeSinceDraw >= frameTime)
        {
            window->drawContent();
            return WaitForEvents; // Window can wait indefinitely here.
        }

        int delay = static_cast<int>(frameTime) - timeSinceDraw; // Time before drawing should happen.
        return WaitTimeoutForEvents(delay);                      // Can only wait maximum on "delay" time.
    }

    int Application::standardMessageLoop()
    {
        PTK_ASSERT(!container().empty(), "No Window added to Application");
        PTK_INFO("Running Standard Message Loop");

        // Paint all visible windows and handle already pushed events.
        for (const auto& [key, window] : *this)
        {
            if (window->visible())
            {
                window->invalidate();
                window->draw();
            }

            window->runCommands();
        }

        // Maybe do some setup things here?

        // Standard message loop for now.
        int eventPollTime = -1;
        while (!container().empty() && !isClosed())
        {
            fetchEvents(eventPollTime);

            // Run Window functions and drawing.
            int nextPollTime = -1;
            for (const auto& pair : *this)
            {
                int delay = WindowEventFrame(pair.second);
                if (delay < eventPollTime || eventPollTime < 0)
                    nextPollTime = delay;
            }
            eventPollTime = nextPollTime;
        }

        return 0;
    }

    void Application::fetchEvents(int allowedTime)
    {
        if (allowedTime < 0)
        {
            // Waiting indefinitely is fine.
            m_handle->waitEvents();
        }
        else if (allowedTime == 0)
        {
            // Can not wait at all.
            m_handle->pollEvents();
        }
        else
        {
            // Can not wait for a specified amount of time.
            m_handle->waitEventsTimeout(allowedTime);
        }
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
            return true;

        if (onCloseRequest())
        {
            closeHelper();
            return true;
        }

        PTK_INFO("Application halted close request");
        return false;
    }

    void Application::closeHelper()
    {
        if (isClosed())
        {
            PTK_WARN("Application is already closed");
            return;
        }

        PTK_INFO("Application is closing...");
        onClose();
        closeWindows();
        clearWindows();
        m_handle->onApplicationClose();
        m_closed = true;
        PTK_INFO("Application closed.");
    }

    Application* Application::Get()
    {
        PTK_ASSERT(s_Instance, "Application is not initialized");
        return s_Instance;
    }

    void Application::clearWindows()
    {
        for (auto it = cbegin(); it != cend(); ++it)
        {
            PTK_INFO("Removing window \"{}\"", it->second->getName());
            m_handle->onWindowRemove(it->first, it->second);
        }

        container().clear();
    }

    void Application::closeWindows()
    {
        auto it = cbegin();
        std::size_t total{container().size()};
        while (it != cend())
        {
            PTK_INFO("Closing window \"{}\"", it->second->getName());
            it->second->close();

            // Window close might remove itself from the application
            // and alter the container (invalidating the iterator).
            if (total != container().size())
            {
                it = cbegin();
                total = container().size();
            }
            else
                ++it;
        }
    }

} // namespace pTK
