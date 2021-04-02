//
//  menu/MenuBar.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUBAR_HPP
#define PTK_MENU_MENUBAR_HPP

// pTK Headers
#include "ptk/menu/Menu.hpp"
#include "ptk/menu/MenuItem.hpp"
#include "ptk/util/IterableContainer.hpp"

// C++ Headers
#include <vector>

namespace pTK
{
    class MenuBar : public IterableSequence<Ref<Menu>>
    {
    public:
        MenuBar() = default;
        MenuBar(const std::initializer_list<Ref<Menu>>& menus)
            : IterableSequence<Ref<Menu>>(menus)
        {

        }
        virtual ~MenuBar() = default;

        void addMenu(const Ref<Menu>& menu)
        {
            container().push_back(menu);
        }

        void removeMenu(const Ref<Menu>& menu)
        {
            for (auto it = cbegin(); it != cend(); ++it)
                if (menu == (*it))
                    container().erase(it);
        }
    };
}

#endif // PTK_MENU_MENUBAR_HPP
