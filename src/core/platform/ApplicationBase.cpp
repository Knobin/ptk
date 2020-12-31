//
//  core/platform/ApplicationBase.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

// Local Headers
#include "ptk/core/platform/ApplicationBase.hpp"

namespace pTK
{
    int32 ApplicationBase::addWindow(Window *window)
    {
        int32 id{1};
        for (const std::pair<const int32, Window*>& pair : m_mainWindows)
        {
            if (pair.first == id)
            {
                ++id;
            }
        }

        try {
            m_mainWindows.insert({id, window});
            onWindowAdd(id);
        } catch (const std::exception&) {
            return -1;
        }

        return id;
    }

    bool ApplicationBase::removeWindow(int32 key)
    {
        auto it{m_mainWindows.find(key)};
        if (it != m_mainWindows.end())
        {
            onWindowRemove(key);
            m_mainWindows.erase(it);
            return true;
        }

        return false;
    }

    bool ApplicationBase::removeWindow(Window *window)
    {
        auto it{std::find_if(m_mainWindows.begin(), m_mainWindows.end(), [window](const auto& pair) {
            return pair.second == window;
        })};

        if (it != m_mainWindows.end())
        {
            onWindowRemove(it->first);
            m_mainWindows.erase(it);
            return true;
        }

        return false;
    }

    void ApplicationBase::removeAllWindows()
    {
        for (auto it = m_mainWindows.cbegin(); it != m_mainWindows.cend();)
        {
            Window *window{it->second};
            Event evt{Event::Category::Window, Event::Type::WindowClose};
            window->handleEvents(); // Handle all events before sending close event.
            window->sendEvent(&evt);
            window->handleEvents();
            m_mainWindows.erase(it++);
        }
    }

    const std::map<int32, Window*>& ApplicationBase::windows() const
    {
        return m_mainWindows;
    }

    ApplicationBase::container::size_type ApplicationBase::windowCount() const
    {
        return m_mainWindows.size();
    }

    Window *ApplicationBase::find(int32 key) const 
    { 
        auto it = m_mainWindows.find(key);
        return (it != m_mainWindows.end()) ? it->second : nullptr;
    }
}
