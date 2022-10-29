//
//  platform/win/MenuBarUtil_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-16.
//

// Local Headers
#include "ptk/platform/win/MenuBarUtil_win.hpp"

// pTK Headers
#include "ptk/events/KeyMap.hpp"
#include "ptk/menu/NamedMenuItem.hpp"

// C++ Headers
#include <array>

namespace pTK::MenuBarUtil_win
{
    void SetNamedOrCheckboxItem(HMENU hmenu, MenuMap& menus, const Ref<MenuItem>& item, uint32_t menuId,
                                std::vector<ACCEL>& keys, bool isCheckbox)
    {
        auto* nItem = dynamic_cast<NamedMenuItem*>(item.get());
        if (!nItem)
            return;

        uint32_t id = MenuBarUtil_win::InsertMenuItemToMap(menus, item, menuId, false, nullptr);
        std::string menuStr{nItem->name()};

        if (auto accelData = MenuBarUtil_win::GetShortcutACCEL(nItem->shortcut()))
        {
            accelData->first.cmd = static_cast<WORD>(id);
            keys.push_back(accelData->first);
            menuStr += "\t" + accelData->second;
        }

        UINT statusFlag = MenuBarUtil_win::MenuItemStatusToFlag(nItem->status());
        if (isCheckbox)
        {
            nItem->onUpdate("Win32::Update", [nItem, id, hmenu]() {
                UINT checked = (nItem->status() == MenuItemStatus::Checked) ? MF_CHECKED : MF_UNCHECKED;
                UINT statusFlag = MenuBarUtil_win::MenuItemStatusToFlag(nItem->status());
                // CheckMenuItem(m_parent, m_id, MF_BYCOMMAND | checked | statusFlag);
                const std::string str =
                    nItem->name() + "\t" + MenuBarUtil_win::TranslateKeyCodesToShortcutStr(nItem->shortcut());
                ModifyMenu(hmenu, id, MF_BYCOMMAND | MF_STRING | checked | statusFlag, id, str.c_str());
                PTK_INFO("Win32::Update, id {}", id);
            });

            AppendMenu(hmenu, MF_STRING | statusFlag, id, menuStr.c_str());
        }
        else
        {
            nItem->onUpdate("Win32::Update", [nItem, id, hmenu]() {
                UINT statusFlag = MenuBarUtil_win::MenuItemStatusToFlag(nItem->status());
                const std::string str =
                    nItem->name() + "\t" + MenuBarUtil_win::TranslateKeyCodesToShortcutStr(nItem->shortcut());
                ModifyMenu(hmenu, id, MF_BYCOMMAND | MF_STRING | statusFlag, id, str.c_str());
                PTK_INFO("Win32::Update, id {}", id);
            });

            AppendMenu(hmenu, MF_STRING | statusFlag, id, menuStr.c_str());
        }
    }

    void CreateMenuStructure(HMENU parent, MenuMap& menus, const pTK::Ref<pTK::Menu>& menu, uint32_t parentId,
                             std::vector<ACCEL>& keys)
    {
        HMENU currentMenu = ::CreateMenu();
        AppendMenu(parent, MF_POPUP, reinterpret_cast<UINT_PTR>(currentMenu), menu->name().c_str());
        uint32_t currentMenuId = MenuBarUtil_win::InsertMenuItemToMap(menus, menu, parentId, true, currentMenu);

        for (auto menuItemIt{menu->cbegin()}; menuItemIt != menu->cend(); ++menuItemIt)
        {
            switch ((*menuItemIt)->type())
            {
                case MenuItemType::Text:
                {
                    SetNamedOrCheckboxItem(currentMenu, menus, *menuItemIt, currentMenuId, keys, false);
                    break;
                }
                case MenuItemType::Checkbox:
                {
                    SetNamedOrCheckboxItem(currentMenu, menus, *menuItemIt, currentMenuId, keys, true);
                    break;
                }
                case MenuItemType::Separator:
                {
                    uint32_t id =
                        MenuBarUtil_win::InsertMenuItemToMap(menus, *menuItemIt, currentMenuId, false, nullptr);
                    InsertMenu(currentMenu, id, MF_SEPARATOR | MF_BYPOSITION, 0, nullptr);
                    // TODO: Check if separator is enabled and add a callback as well.
                    break;
                }
                case MenuItemType::Menu:
                {
                    Ref<Menu> rMenu = std::dynamic_pointer_cast<Menu>(*menuItemIt);
                    if (rMenu)
                        CreateMenuStructure(currentMenu, menus, rMenu, currentMenuId, keys);
#ifdef PTK_DEBUG
                    else
                        PTK_WARN("Could not cast MenuItem to Menu");
#endif // PTK_DEBUG
                    break;
                }
            }
        }
    }

    uint32_t InsertMenuItemToMap(MenuMap& menus, const Ref<MenuItem>& menuItem, uint32_t parentId, bool isMenu,
                                 HMENU hmenu)
    {
        uint32_t uniqueId{1};
        for (const auto& it : menus)
        {
            if (it.first == uniqueId)
                ++uniqueId;
        }

        menus.insert({uniqueId, {menuItem, parentId, isMenu, hmenu}});
        return uniqueId;
    }

    Ref<MenuItem> FindMenuItemById(const MenuMap& menuItems, uint32_t id)
    {
        MenuMap::const_iterator it{menuItems.find(id)};
        if (it != menuItems.cend())
            return std::get<0>(it->second);

        return nullptr;
    }

    std::string TranslateKeyCodeToShortcutStr(const Key& key)
    {
        std::string str{};

        // Find KeyCode in global map.
        if (KeyMap::KeyExists(key))
        {
            constexpr std::array<Key, 2> altKeys{Key::LeftAlt, Key::RightAlt};
            constexpr std::array<Key, 2> ctrlKeys{Key::LeftControl, Key::RightControl};
            constexpr std::array<Key, 2> shiftKeys{Key::LeftShift, Key::RightShift};

            constexpr std::pair<std::string_view, std::array<Key, 2>> alt("Alt", altKeys);
            constexpr std::pair<std::string_view, std::array<Key, 2>> ctrl("Ctrl", ctrlKeys);
            constexpr std::pair<std::string_view, std::array<Key, 2>> shift("Shift", shiftKeys);

            constexpr std::array<std::pair<std::string_view, std::array<Key, 2>>, 3> shortcutKeys{alt, ctrl, shift};
            const auto foundKey = std::find_if(shortcutKeys.cbegin(), shortcutKeys.cend(), [&](const auto& pair) {
                const auto& arr = pair.second;
                auto found = std::find(std::begin(arr), std::end(arr), key);
                return found != std::end(arr);
            });

            if (foundKey != shortcutKeys.cend())
                str = foundKey->first;
            else if (IsKeyCodeAlpha(key))
                str = KeyCodeToAlpha(key);
        }

        return str;
    }

    std::optional<std::pair<ACCEL, std::string>> GetShortcutACCEL(const Shortcut& shortcut)
    {
        uint8_t virt{FVIRTKEY};
        std::string shortcutStr{};

        // static const std::map<int32, KeyCode> s_keyMap{InitKeyCodes()};
        for (const KeyCode& key : shortcut.modifiers())
        {
            if (KeyMap::KeyExists(key))
            {
                std::string str{TranslateKeyCodeToShortcutStr(key)};
                if (!str.empty())
                {
                    constexpr std::pair<std::string_view, int32_t> alt("Alt", FALT);
                    constexpr std::pair<std::string_view, int32_t> ctrl("Ctrl", FCONTROL);
                    constexpr std::pair<std::string_view, int32_t> shift("Shift", FSHIFT);

                    constexpr std::array<std::pair<std::string_view, int32_t>, 3> virts{alt, ctrl, shift};
                    const auto foundVirt = std::find_if(virts.cbegin(), virts.cend(), [&](const auto& pair) {
                        return pair.first == str;
                    });

                    if (foundVirt != virts.cend())
                    {
                        // Multiple virts are supported.
                        virt |= static_cast<uint8_t>(foundVirt->second);
                        virt |= static_cast<uint8_t>(std::get<1>(*foundVirt));
                        shortcutStr += (!shortcutStr.empty() ? "+" : "") + std::string{foundVirt->first};
                    }
                }
            }
        }

        if ((!shortcutStr.empty()) && (shortcut.key() != pTK::KeyCode::Unknown && IsKeyCodeAlpha(shortcut.key())))
        {
            ACCEL accel{};
            accel.key = static_cast<WORD>(KeyMap::KeyToKeyCode(shortcut.key()));
            if (virt != 0)
                accel.fVirt = virt;
            return std::pair{accel, shortcutStr + (!shortcutStr.empty() ? "+" : "") + KeyCodeToAlpha(shortcut.key())};
        }

        return std::nullopt;
    }

    std::string TranslateKeyCodesToShortcutStr(const Shortcut& shortcut)
    {
        std::string str{};
        std::string key = (shortcut.key() == pTK::KeyCode::Unknown) ? "" : std::string{KeyCodeToAlpha(shortcut.key())};

        for (auto it{shortcut.modifiers().cbegin()}; it != shortcut.modifiers().cend(); ++it)
        {
            const std::string keyStr = TranslateKeyCodeToShortcutStr(*it);
            str += (!str.empty() ? "+" : "") + keyStr;
        }

        return str + (!str.empty() ? "+" : "") + key;
    }

} // namespace pTK::MenuBarUtil_win
