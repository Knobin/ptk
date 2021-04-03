//
//  menu/NamedMenuItem.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// pTK Headers
#include "ptk/menu/NamedMenuItem.hpp"

namespace pTK
{
    NamedMenuItem::NamedMenuItem(const std::string& name, const Shortcut& shortcut)
            : MenuItem(MenuItemType::Text, MenuItemStatus::Enabled),
              m_shortcut{shortcut}, m_name{name}
    {}

    NamedMenuItem::NamedMenuItem(const std::string& name, MenuItemType type, MenuItemStatus status, const Shortcut& shortcut)
            : MenuItem(type, status), m_shortcut{shortcut}, m_name{name}
    {}

    [[nodiscard]] const Shortcut& NamedMenuItem::shortcut() const { return m_shortcut; }
    [[nodiscard]] const std::string& NamedMenuItem::name() const { return m_name; }

    void NamedMenuItem::notifyClick()
    {
        for (auto& it : m_clickCallbacks)
            it.second();
    }

    void NamedMenuItem::rename(const std::string& name)
    {
        m_name = name;
        handleEvent<MenuItemEvent::Update>();
    }

    void NamedMenuItem::removeCallback(const std::string& name)
    {
        auto updateIt = m_clickCallbacks.find(name);
        if (updateIt != m_clickCallbacks.end())
        m_clickCallbacks.erase(updateIt);

        MenuItem::removeCallback(name);
    }

    void NamedMenuItem::onClick(const std::string& name, const std::function<void()>& func)
    {
        m_clickCallbacks[name] = func;
    }

} // pTK
