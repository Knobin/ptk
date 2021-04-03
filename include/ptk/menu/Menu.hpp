//
//  menu/Menu.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENU_HPP
#define PTK_MENU_MENU_HPP

// pTK Headers
#include "ptk/menu/MenuItem.hpp"
#include "ptk/Core.hpp"
#include "ptk/util/IterableContainer.hpp"

// C++ Headers
#include <vector>

namespace pTK
{
    /** Menu class implementation.

        Collection of menu items that forms a menu.
    */
    class Menu : public MenuItem, public IterableSequence<Ref<MenuItem>>
    {
    public:
        /** Constructs Menu with name and items.

            @param name         displayed text
            @param items        optional list with menu items
            @return             initialized Menu
        */
        explicit Menu(const std::string& name, const std::initializer_list<Ref<MenuItem>>& items = {});

        /** Destructor.

        */
        virtual ~Menu() = default;

        /** Function for retrieving the name of the menu.

            @return name
        */
        [[nodiscard]] const std::string& name() const;

        /** Function for renaming the menu.

            @param name     new name of the menu
        */
        void rename(const std::string& name);

        /** Function for adding a menu item.

            @param item     menu item to add
        */
        void addItem(const Ref<MenuItem>& item);

        /** Function for removing a menu item.

            @param item     menu item to remove
        */
        void removeItem(const Ref<MenuItem>& item);

    private:
        std::string m_name;
    };
}

#endif // PTK_MENU_MENU_HPP
