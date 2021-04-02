//
//  menu/NamedMenuItem.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-14.
//

#ifndef PTK_MENU_NAMEDMENUITEM_HPP
#define PTK_MENU_NAMEDMENUITEM_HPP

// pTK Headers
#include "ptk/menu/MenuItem.hpp"
#include "ptk/menu/Shortcut.hpp"

namespace pTK
{
    class NamedMenuItem : public MenuItem
    {
    public:
        NamedMenuItem() = delete;
        explicit NamedMenuItem(const std::string& name, const Shortcut& shortcut = {})
            : MenuItem(MenuItemType::Text, MenuItemStatus::Enabled),
              m_shortcut{shortcut}, m_name{name}
        {}
        NamedMenuItem(const std::string& name, MenuItemType type, MenuItemStatus status, const Shortcut& shortcut = {})
            : MenuItem(type, status), m_shortcut{shortcut}, m_name{name}
        {}
        virtual ~NamedMenuItem() = default;

        [[nodiscard]] const Shortcut& shortcut() const { return m_shortcut; }
        [[nodiscard]] const std::string& name() const { return m_name; }

        void notifyClick()
        {
            for (auto& it : m_clickCallbacks)
                it.second();
        }

        void rename(const std::string& name)
        {
            m_name = name;
            handleEvent<MenuItemEvent::Update>();
        }

        void removeCallback(const std::string& name) override
        {
            auto updateIt = m_clickCallbacks.find(name);
            if (updateIt != m_clickCallbacks.end())
                m_clickCallbacks.erase(updateIt);

            MenuItem::removeCallback(name);
        }

        void onClick(const std::string& name, const std::function<void()>& func) {
            m_clickCallbacks[name] = func;
        }

    private:
        std::map<std::string, std::function<void()>> m_clickCallbacks;
        Shortcut m_shortcut{};
        std::string m_name{};
    };

} // pTK

#endif // PTK_MENU_NAMEDMENUITEM_HPP
