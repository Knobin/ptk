//
//  menu/Menu.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_MENU_MENU_HPP
#define PTK_MENU_MENU_HPP

// Local Headers
#include "ptk/menu/MenuItemBase.hpp"
#include "ptk/Core.hpp"

// C++ Headers
#include <initializer_list>

namespace pTK
{
    class Menu : public MenuItemBase
    {
    public:
        explicit Menu(const std::string& name = "")
            : MenuItemBase(name), m_menuItems{}
        {

        }

        Menu(const std::string& name, const std::initializer_list<Ref<MenuItemBase>>& items)
            : MenuItemBase(name), m_menuItems{items}
        {

        }

        void add(const Ref<MenuItemBase>& item)
        {
            m_menuItems.push_back(item);
        }

    private:
        std::vector<Ref<MenuItemBase>> m_menuItems;
    };
}

#endif // PTK_MENU_MENU_HPP
