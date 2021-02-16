//
//  menu/MenuItemSeparator.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-15.
//

#ifndef PTK_MENU_MENUITEMSEPARATOR_HPP
#define PTK_MENU_MENUITEMSEPARATOR_HPP

// pTK Headers
#include "ptk/menu/MenuItem.hpp"

namespace pTK
{
    class MenuItemSeparator : public MenuItem
    {
    public:
        MenuItemSeparator()
            : MenuItem(MenuItemType::Separator, MenuItemStatus::Enabled)
        {}
        virtual ~MenuItemSeparator() = default;
    };

} // pTK

#endif // PTK_MENU_MENUITEMSEPARATOR_HPP
