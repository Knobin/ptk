//
//  menu/CheckboxMenuItem.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-14.
//

#ifndef PTK_MENU_CHECKBOXMENUITEM_HPP
#define PTK_MENU_CHECKBOXMENUITEM_HPP

// pTK Headers
#include "ptk/menu/NamedMenuItem.hpp"

namespace pTK
{
    /** CheckboxMenuItem class implementation.

        Defines a menu item with a checkbox next to it.
    */
    class PTK_API CheckboxMenuItem : public NamedMenuItem
    {
    public:
        /** Constructs CheckboxMenuItem with name and status.

            @param name     displayed text
            @param status   optional status, default Unchecked
            @return         initialized CheckboxMenuItem
        */
        explicit CheckboxMenuItem(const std::string& name, MenuItemStatus status = MenuItemStatus::Unchecked);

        /** Constructs CheckboxMenuItem with name, status and shortcut.

            @param name         displayed text
            @param status       status
            @param shortcut     keyboard shortcut
            @return             initialized CheckboxMenuItem
        */
        CheckboxMenuItem(const std::string& name, MenuItemStatus status, const Shortcut& shortcut);

        /** Destructor.

        */
        virtual ~CheckboxMenuItem() = default;

        /** Function for retrieving the toggle callbacks.

            @return toggle callbacks
        */
        [[nodiscard]] const std::map<std::string, std::function<void(MenuItemStatus)>>& toggleCallbacks() const;

        /** Function for adding a toggle callback.

            @param name     name of the callback
            @param func     callback function
        */
        void onToggle(const std::string& name, const std::function<void(MenuItemStatus)>& func);

        /** Function for checking if the checkbox is checked.

            @return true if checked, otherwise false
        */
        [[nodiscard]] bool checked() const;

        /** Function for setting the check status.

            @param checked      true for checked, otherwise false
        */
        void setChecked(bool checked);

        /** Function for setting the check status to true.

        */
        void check();

        /** Function for setting the check status to false.

        */
        void uncheck();

    private:
        void init();

    private:
        std::map<std::string, std::function<void(MenuItemStatus)>> m_toggleCallbacks;
    };

} // pTK

#endif // PTK_MENU_CHECKBOXMENUITEM_HPP
