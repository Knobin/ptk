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
#include "ptk/menu/MenuItemEvent.hpp"
#include "ptk/menu/Shortcut.hpp"

namespace pTK
{
    class NamedMenuItem : public MenuItem, public MenuItemEvent::Click
    {
    public:
        NamedMenuItem() = delete;
        explicit NamedMenuItem(const std::string& name, const Shortcut& shortcut = {})
            : MenuItem(MenuItemType::Text, MenuItemStatus::Enabled), MenuItemEvent::Click(),
              m_shortcut{shortcut}, m_name{name}
        {}
        NamedMenuItem(const std::string& name, MenuItemType type, MenuItemStatus status, const Shortcut& shortcut = {})
            : MenuItem(type, status), MenuItemEvent::Click(), m_shortcut{shortcut}, m_name{name}
        {}
        virtual ~NamedMenuItem() = default;

        [[nodiscard]] const Shortcut& shortcut() const { return m_shortcut; }
        [[nodiscard]] const std::string& name() const { return m_name; }

        void notifyClick()
        {
            MenuItemEvent::Click::onEvent();
        }

        void rename(const std::string& name)
        {
            m_name = name;
            handleEvent<MenuItemEvent::Update>();
        }

    private:
        Shortcut m_shortcut{};
        std::string m_name{};
    };

} // pTK

#endif // PTK_MENU_NAMEDMENUITEM_HPP
