//
//  menu/MenuItemType.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-09.
//

#ifndef PTK_MENU_MENUITEMTYPE_HPP
#define PTK_MENU_MENUITEMTYPE_HPP

namespace pTK
{
    enum class MenuItemType : byte
    {
        Text = 0,   // Text item, can be pressed (button).
        Checkbox,   // Text with a checkbox, can be toggled.
        Separator,  // Usually a line, cannot be clicked and is "static".
        Menu        // Collections of Menus.
    };

    enum class MenuItemStatus : byte
     {
         Enabled = 0,    // Can be selected.
         Disabled,       // Can not be selected and is "greyed" out.
         Checked,        // Should only be combined with Type::Checkbox.
         Unchecked,      // Should only be combined with Type::Checkbox.
     };

    constexpr std::string_view MenuItemTypeToStr(const MenuItemType& type)
    {
        switch (type)
        {
            case MenuItemType::Text:
                return "Text";
            case MenuItemType::Checkbox:
                return "Checkbox";
            case MenuItemType::Separator:
                return "Separator";
            case MenuItemType::Menu:
               return "Menu";
        }
    }

    constexpr std::string_view MenuItemStatusToStr(const MenuItemStatus& status)
     {
         switch (status)
         {
             case MenuItemStatus::Enabled:
                 return "Enabled";
             case MenuItemStatus::Disabled:
                 return "Disabled";
             case MenuItemStatus::Checked:
                 return "Checked";
             case MenuItemStatus::Unchecked:
                 return "Unchecked";
         }
     }

    constexpr bool IsMenuItemStatusTypeCheckbox(MenuItemStatus status)
    {
        return (static_cast<bool>(status == MenuItemStatus::Checked) || static_cast<bool>(MenuItemStatus::Unchecked));
    }
}

#endif // PTK_MENU_MENUITEMTYPE_HPP
