//
//  menu/MenuCollection.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_MENU_MENUCOLLECTION_HPP
#define PTK_MENU_MENUCOLLECTION_HPP

// Local Headers
#include "ptk/menu/Menu.hpp"

namespace pTK
{
    class MenuCollection
    {
    public:
        MenuCollection() = default;
        MenuCollection(const std::initializer_list<Ref<Menu>>& menus)
            : m_menus{menus}
        {

        }

        void add(const Ref<Menu>& menu)
        {
            m_menus.push_back(menu);
        }

    private:
        std::vector<Ref<Menu>> m_menus;
    };
}

#endif // PTK_MENU_MENUCOLLECTION_HPP
