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
