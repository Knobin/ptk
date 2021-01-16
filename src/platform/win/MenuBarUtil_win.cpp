//
//  platform/win/MenuBarUtil_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-16.
//

// Local Headers
#include "MenuBarUtil_win.hpp"
#include "KeyMap_win.hpp"

namespace pTK::MenuBarUtil_win
{
    void CreateMenuStructure(HMENU parent, MenuMap& menus, const pTK::Ref<pTK::Menu>& menu, uint parentId, std::vector<ACCEL>& keys)
    {
        HMENU currentMenu = ::CreateMenu();
        AppendMenu(parent, MF_POPUP, reinterpret_cast<UINT_PTR>(currentMenu), menu->name().c_str());
        uint currentMenuId = MenuBarUtil_win::InsertMenuItemToMap(menus, menu, parentId, true, currentMenu);

        for (auto menuItemIt{menu->cbegin()}; menuItemIt != menu->cend(); ++menuItemIt)
        {
            // Can either be Menu or MenuItem.
            const Ref<MenuItemBase>& menuItemPtr = *menuItemIt;
            if (menuItemPtr->typeName() == "Menu")
            {
                Ref<Menu> rMenu = std::dynamic_pointer_cast<Menu>(menuItemPtr);
                if (rMenu)
                    CreateMenuStructure(currentMenu, menus, rMenu, currentMenuId, keys);
#ifdef PTK_DEBUG
                else
                    PTK_WARN("Could not cast MenuItemBase to Menu");
#endif // PTK_DEBUG
            }
            else if (menuItemPtr->typeName() == "MenuItem")
            {
                if (auto ptr = dynamic_cast<MenuItem*>(menuItemPtr.get()))
                {
                    uint id = MenuBarUtil_win::InsertMenuItemToMap(menus, menuItemPtr, currentMenuId, false, nullptr);
                    std::string menuStr{ptr->name()};

                    if (!ptr->shortcutKeys().empty())
                    {
                        if (auto accelData = MenuBarUtil_win::GetShortcutACCEL(ptr->shortcutKeys()))
                        {
                            accelData->first.cmd = static_cast<WORD>(id);
                            keys.push_back(accelData->first);
                            menuStr += "\t" + accelData->second;
                        }
                    }

                    UINT statusFlag = MenuBarUtil_win::MenuItemStatusToFlag(ptr->status());
                    switch (ptr->type())
                    {
                        case MenuItem::Type::Text:
                        {
                            using Handler = MenuItemHandler_win;
                            Ref<Handler> handler = Create<Handler>(currentMenu, id);
                            ptr->setHandler(handler);
                            AppendMenu(currentMenu, MF_STRING | statusFlag, id, menuStr.c_str());
                            break;
                        }
                        case MenuItem::Type::Checkbox:
                        {
                            AppendMenu(currentMenu, MF_STRING | statusFlag, id, menuStr.c_str());
                            using CbHander = CheckboxMenuItemHandler_win;
                            bool checked = (ptr->status() == MenuItem::Status::Checked);
                            Ref<CbHander> handler = Create<CbHander>(currentMenu, id, checked);
                            ptr->setHandler(handler);
                            break;
                        }
                    }
                }
#ifdef PTK_DEBUG
                else
                    PTK_WARN("Could not cast MenuItemBase to MenuItem");
#endif // PTK_DEBUG
            }
#ifdef PTK_DEBUG
            else
                PTK_WARN("Unknown Menu class: {}", menuItemPtr->typeName());
#endif // PTK_DEBUG
        }
    }

    uint InsertMenuItemToMap(MenuMap& menus, const Ref<MenuItemBase>& menuItem, uint parentId, bool isMenu, HMENU hmenu)
    {
        uint uniqueId{1};
        for (const auto& it : menus)
        {
            if (it.first == uniqueId)
                ++uniqueId;
        }

        menus.insert({uniqueId, {menuItem, parentId, isMenu, hmenu}});
        return uniqueId;
    }

    Ref<MenuItemBase> FindMenuItemById(const MenuMap& menuItems, uint id)
    {
        MenuMap::const_iterator it{menuItems.find(id)};
        if (it != menuItems.cend())
            return std::get<0>(it->second);

        return nullptr;
    }

    std::string TranslateKeyCodeToShortcutStr(const KeyCode& key)
    {
        std::string str{};

        // Find KeyCode in global map.
        if (KeyMap_win::KeyExistsInMap(key))
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
            else
            if (IsKeyCodeAlpha(key))
                str = KeyCodeToAlpha(key);
        }

        return str;
    }

    std::optional<std::pair<ACCEL, std::string>> GetShortcutACCEL(const std::vector<KeyCode>& codes)
    {
        byte virt{FVIRTKEY};
        std::string shortcutStr{};
        WORD lastKey{0};

        // static const std::map<int32, KeyCode> s_keyMap{InitKeyCodes()};
        for (const KeyCode& key : codes)
        {
            if (KeyMap_win::KeyExistsInMap(key))
            {
                std::string str{TranslateKeyCodeToShortcutStr(key)};
                if (!str.empty())
                {
                    constexpr std::pair<std::string_view, int32> alt("Alt", FALT);
                    constexpr std::pair<std::string_view, int32> ctrl("Ctrl", FCONTROL);
                    constexpr std::pair<std::string_view, int32> shift("Shift", FSHIFT);

                    constexpr std::array<std::pair<std::string_view, int32>, 3> virts{alt, ctrl, shift};
                    const auto foundVirt = std::find_if(virts.cbegin(), virts.cend(),[&](const auto& pair) {
                        return pair.first == str;
                    });

                    if (foundVirt != virts.cend())
                    {
                        // Multiple virts are supported.
                        virt |= static_cast<byte>(foundVirt->second);
                        virt |= static_cast<byte>(std::get<1>(*foundVirt));
                        shortcutStr += (!shortcutStr.empty() ? "+" : "") + std::string{foundVirt->first};
                    }
                    else
                    {
                        // This is a keycode.
                        lastKey = static_cast<WORD>(KeyMap_win::TranslateKeyToKeyCode(key));
                    }
                }
            }
        }

        if (lastKey != 0)
        {
            ACCEL accel{};
            accel.key = lastKey;
            if (virt != 0)
                accel.fVirt = virt;
            return std::pair{accel, shortcutStr + (!shortcutStr.empty() ? "+" : "") + static_cast<char>(lastKey)};
        }

        return std::nullopt;
    }

    std::string TranslateKeyCodesToShortcutStr(const std::vector<KeyCode>& codes)
    {
        std::string str{};
        std::string key{};

        for (auto it{codes.cbegin()}; it != codes.cend(); ++it)
        {
            const std::string keyStr = TranslateKeyCodeToShortcutStr(*it);
            if (IsKeyCodeAlpha(*it))
                key = keyStr;
            else
                str += (!str.empty() ? "+" : "") + keyStr;
        }

        return str + (!str.empty() ? "+" : "") + key;
    }

} // namespace pTK