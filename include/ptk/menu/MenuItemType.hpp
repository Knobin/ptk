//
//  menu/MenuItemType.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-09.
//

#ifndef PTK_MENU_MENUITEMTYPE_HPP
#define PTK_MENU_MENUITEMTYPE_HPP

// pTK Headers
#include "ptk/Log.hpp"

// C++ Headers
#include <cstdint>
#include <string_view>

namespace pTK
{
    enum class MenuItemType : uint8_t
    {
        Text = 0,  // Text item, can be pressed (button).
        Checkbox,  // Text with a checkbox, can be toggled.
        Separator, // Usually a line, cannot be clicked and is "static".
        Menu       // Collections of Menus.
    };

    enum class MenuItemStatus : uint8_t
    {
        Enabled = 0, // Can be selected.
        Disabled,    // Can not be selected and is "greyed" out.
        Checked,     // Should only be combined with Type::Checkbox.
        Unchecked,   // Should only be combined with Type::Checkbox.
    };

    enum class MenuItemEvent : uint8_t
    {
        Update = 0, // Something updated.
        Status,     // Status changed.
    };

    constexpr std::string_view MenuItemTypeToStr(MenuItemType type) noexcept
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

        // All paths are covered, but MSVC complains anyways.
        PTK_ASSERT(false, "");
        return "";
    }

    constexpr std::string_view MenuItemStatusToStr(MenuItemStatus status) noexcept
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

    constexpr std::string_view MenuItemEventToStr(MenuItemEvent evt) noexcept
    {
        switch (evt)
        {
            case MenuItemEvent::Update:
                return "Update";
            case MenuItemEvent::Status:
                return "Status";
        }
    }

    constexpr bool IsMenuItemStatusTypeCheckbox(MenuItemStatus status) noexcept
    {
        return ((status == MenuItemStatus::Checked) || (status == MenuItemStatus::Unchecked));
    }
} // namespace pTK

#endif // PTK_MENU_MENUITEMTYPE_HPP
