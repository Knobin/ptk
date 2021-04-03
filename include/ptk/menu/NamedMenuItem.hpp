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
    /** NamedMenuItem class implementation.

        Regular menu item with text that can be clicked.
    */
    class NamedMenuItem : public MenuItem
    {
    public:
        /** Constructs NamedMenuItem with name and shortcut.

            @param name         name of the menu item
            @param shortcut     optional keyboard shortcut
            @return             initialized NamedMenuItem
        */
        explicit NamedMenuItem(const std::string& name, const Shortcut& shortcut = {});

        /** Constructs NamedMenuItem with name and shortcut.

            @param name         name of the menu item
            @param type         type of the menu item
            @param status       status of the menu item
            @param shortcut     optional keyboard shortcut
            @return             initialized NamedMenuItem
        */
        NamedMenuItem(const std::string& name, MenuItemType type, MenuItemStatus status, const Shortcut& shortcut = {});

        /** Destructor.

        */
        virtual ~NamedMenuItem() = default;

        /** Function for retrieving the shortcut of the menu item.

            @return shortcut
        */
        [[nodiscard]] const Shortcut& shortcut() const;

        /** Function for retrieving the name of the menu item.

            @return name
        */
        [[nodiscard]] const std::string& name() const;

        /** Function for notifying that the menu item has been clicked.

        */
        void notifyClick();

        /** Function for renaming the menu item.

            @param name     new name of the menu item
        */
        void rename(const std::string& name);

        /** Function for removing a callback by name.

            @param name     name of the callback
        */
        void removeCallback(const std::string& name) override;

        /** Function for adding a click callback.

            @param name     name of the callback
            @param func     callback function
        */
        void onClick(const std::string& name, const std::function<void()>& func);

    private:
        std::map<std::string, std::function<void()>> m_clickCallbacks;
        Shortcut m_shortcut{};
        std::string m_name{};
    };

} // pTK

#endif // PTK_MENU_NAMEDMENUITEM_HPP
