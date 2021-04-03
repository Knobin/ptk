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
    /** MenuItemSeparator class implementation.

        Usually a line.
    */
    class MenuItemSeparator : public MenuItem
    {
    public:
        /** Constructs MenuItemSeparator with type and status.

            @return         default initialized MenuItemSeparator
        */
        MenuItemSeparator()
            : MenuItem(MenuItemType::Separator, MenuItemStatus::Enabled)
        {}

        /** Destructor.

        */
        virtual ~MenuItemSeparator() = default;
    };

    /** Function for creating a MenuItemSeparator.

        @return Ref of MenuItemSeparator
    */
    Ref<MenuItemSeparator> CreateSeparator()
    {
        return Create<MenuItemSeparator>();
    }

} // pTK

#endif // PTK_MENU_MENUITEMSEPARATOR_HPP
