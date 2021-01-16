//
//  platform/win/MenuBarUtil_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-16.
//

#ifndef PTK_PlATFORM_WIN_MENUBARUTIL_HPP
#define PTK_PlATFORM_WIN_MENUBARUTIL_HPP

// Local Headers
#include "ptk/events/KeyCodes.hpp"
#include "ptk/menu/MenuItem.hpp"
#include "ptk/menu/Menu.hpp"

// C++ Headers
#include <map>
#include <optional>
#include <vector>

namespace pTK::MenuBarUtil_win
{
    using MenuMap = std::map<uint, std::tuple<Ref<MenuItemBase>, uint, bool, HMENU>>;

    void CreateMenuStructure(HMENU parent, MenuMap& menus, const pTK::Ref<pTK::Menu>& menu, uint parentId, std::vector<ACCEL>& keys);
    uint InsertMenuItemToMap(MenuMap& menus, const Ref<MenuItemBase>& menuItem, uint parentId, bool isMenu, HMENU hmenu);
    Ref<MenuItemBase> FindMenuItemById(const MenuMap& menuItems, uint id);
    std::string TranslateKeyCodeToShortcutStr(const KeyCode& key);
    std::optional<std::pair<ACCEL, std::string>> GetShortcutACCEL(const std::vector<KeyCode>& codes);
    std::string TranslateKeyCodesToShortcutStr(const std::vector<KeyCode>& codes);

    static constexpr UINT MenuItemStatusToFlag(const MenuItem::Status& status) noexcept
    {
        UINT flag = 0;
        switch (status)
        {
            case MenuItem::Status::Enabled:
                flag |= MF_ENABLED;
                break;
            case MenuItem::Status::Disabled:
                flag |= MF_DISABLED | MF_GRAYED;
                break;
            case MenuItem::Status::Checked:
                flag |= MF_CHECKED;
                break;
            case MenuItem::Status::Unchecked:
                flag |= MF_UNCHECKED;
                break;
        }
        return flag;
    }

    class MenuItemHandler_win : public MenuItemHandler
    {
    public:
        MenuItemHandler_win() = delete;
        MenuItemHandler_win(HMENU parent, uint id) : m_parent{parent}, m_id{id} {}
        virtual ~MenuItemHandler_win() = default;

        void update(MenuItem* item) override
        {
            MenuItemHandlerGuard guard{this};
            UINT statusFlag = MenuBarUtil_win::MenuItemStatusToFlag(item->status());
            const std::string str = item->name() + "\t" + MenuBarUtil_win::TranslateKeyCodesToShortcutStr(item->shortcutKeys());
            ModifyMenu(m_parent, m_id, MF_BYCOMMAND | MF_STRING | statusFlag, m_id, str.c_str());
            PTK_WARN("ModifyMenu");
        }

    private:
        HMENU m_parent{nullptr};
        uint m_id{0};
    };

    class CheckboxMenuItemHandler_win : public MenuItemHandler
    {
    public:
        CheckboxMenuItemHandler_win() = delete;
        CheckboxMenuItemHandler_win(HMENU parent, uint id, bool checked) : m_parent{parent}, m_id{id}, m_checked{checked} {}
        virtual ~CheckboxMenuItemHandler_win() = default;

        void update(MenuItem* item) override
        {
            MenuItemHandlerGuard guard{this};
            UINT checked = (item->status() == MenuItem::Status::Checked) ? MF_CHECKED : MF_UNCHECKED;
            UINT statusFlag = MenuBarUtil_win::MenuItemStatusToFlag(item->status());
            //CheckMenuItem(m_parent, m_id, MF_BYCOMMAND | checked | statusFlag);
            const std::string str = item->name() + "\t" + MenuBarUtil_win::TranslateKeyCodesToShortcutStr(item->shortcutKeys());
            ModifyMenu(m_parent, m_id, MF_BYCOMMAND | MF_STRING | checked | statusFlag, m_id, str.c_str());
        }

        void handleEvent(Window*, MenuItem* item) override
        {
            MenuItemHandlerGuard guard{this};
            m_checked = !m_checked;
            MenuItem::Status status =  (m_checked) ? MenuItem::Status::Checked : MenuItem::Status::Unchecked;
            item->setStatus(status);
            update(item);
        }

    private:
        HMENU m_parent{nullptr};
        uint m_id{0};
        bool m_checked{false};
    };

} // namespace pTK

#endif // PTK_PlATFORM_WIN_MENUBARUTIL_HPP