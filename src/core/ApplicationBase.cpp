//
//  core/ApplicationBase.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2022-11-14.
//

// Local Headers
#include "../Log.hpp"

// pTK Headers
#include "ptk/core/ApplicationBase.hpp"

// C++ Headers
#include <algorithm>

namespace pTK
{
    ApplicationBase::ApplicationBase(std::string_view name)
        : SingleObject(),
          m_name{name}
    {
        PTK_INIT_LOGGING();
    }

    ApplicationBase::const_iterator ApplicationBase::firstByKeyOrWindow(int32_t key, Window* window) const
    {
        return std::find_if(m_holder.cbegin(), m_holder.cend(), [key, window](const auto& p) {
            return key == p.first || window == p.second;
        });
    }

    ApplicationBase::const_iterator ApplicationBase::firstByKey(int32_t key) const
    {
        return std::find_if(m_holder.cbegin(), m_holder.cend(), [key](const auto& p) {
            return key == p.first;
        });
    }

    ApplicationBase::const_iterator ApplicationBase::firstByWindow(Window* window) const
    {
        return std::find_if(m_holder.cbegin(), m_holder.cend(), [window](const auto& p) {
            return window == p.second;
        });
    }

    bool ApplicationBase::insert(int32_t key, Window* window)
    {
        m_holder.emplace_back(key, window);
        return true;
    }

    bool ApplicationBase::uniqueInsert(int32_t key, Window* window)
    {
        if (firstByKeyOrWindow(key, window) == m_holder.cend())
            return insert(key, window);

        return false;
    }
    bool ApplicationBase::eraseByKey(int32_t key)
    {
        auto it = firstByKey(key);
        if (it != m_holder.cend())
        {
            m_holder.erase(it);
            return true;
        }
        return false;
    }

    bool ApplicationBase::eraseByWindow(Window* window)
    {
        auto it = firstByWindow(window);
        if (it != m_holder.cend())
        {
            m_holder.erase(it);
            return true;
        }
        return false;
    }

    bool ApplicationBase::eraseByIter(const_iterator it)
    {
        if (it != m_holder.cend())
        {
            m_holder.erase(it);
            return true;
        }
        return false;
    }
} // namespace pTK