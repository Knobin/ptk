//
//  menu/MenuBar.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUBAR_HPP
#define PTK_MENU_MENUBAR_HPP

// Local Headers
#include "ptk/menu/Menu.hpp"
#include "ptk/menu/MenuItem.hpp"
#include "ptk/util/IterableContainer.hpp"

// C++ Headers
#include <vector>

namespace pTK
{
class MenuBar : public IterableContainer<std::vector, Ref<Menu>>
    {
    public:
        MenuBar() = default;
        MenuBar(const std::initializer_list<Ref<Menu>>& menus)
            : IterableContainer<std::vector, Ref<Menu>>(menus)
        {

        }
        virtual ~MenuBar() = default;
    };
}

#endif // PTK_MENU_MENUBAR_HPP
