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
    /** MenuBar class implementation.

        Collection of menus that forms a menu bar.
    */
    class MenuBar : public IterableSequence<Ref<Menu>>
    {
    public:
        /** Constructs Menu with default values.

            @return     default initialized Menu
        */
        MenuBar() = default;

        /** Constructs Menu with menus.

            @param menus    collection of menus to add
            @return         initialized Menu
        */
        MenuBar(const std::initializer_list<Ref<Menu>>& menus);

        /** Destructor.

        */
        virtual ~MenuBar() = default;

        /** Function for adding a menu.

            @param menu     menu to add
        */
        void addMenu(const Ref<Menu>& menu);

        /** Function for removing a menu.

            @param menu     menu to remove
        */
        void removeMenu(const Ref<Menu>& menu);

        /** Function for checking if the menu bar is empty.

            @return     true if empty, otherwise false
        */
        bool empty() const;
    };
}

#endif // PTK_MENU_MENUBAR_HPP
