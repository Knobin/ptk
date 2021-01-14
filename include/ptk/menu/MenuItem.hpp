//
//  menu/MenuItemBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUITEM_HPP
#define PTK_MENU_MENUITEM_HPP

// Local Headers
#include "ptk/menu/MenuItemBase.hpp"
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <functional>

namespace pTK
{
    class Window;

    class MenuItem : public MenuItemBase
    {
    public:
        MenuItem() = default;

        explicit MenuItem(const std::string& name, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_callback{func}
        {

        }

         MenuItem(const std::string& name, const std::initializer_list<KeyCode>& shortcut)
            : MenuItemBase(name), m_shortcutKeys{shortcut}
        {

        }

        MenuItem(const std::string& name, const std::initializer_list<KeyCode>& shortcut, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_shortcutKeys{shortcut}, m_callback{func}
        {

        }

        virtual ~MenuItem() = default;

        void onEvent(const std::function<void(Window*, MenuItem*)>& func) { m_callback = func; }

        void handleEvent(Window *window)
        {
            if (m_callback)
                m_callback(window, this);
        }

        [[nodiscard]] std::string_view typeName() const override { return "MenuItem"; }

        [[nodiscard]] const std::vector<KeyCode>& shortcutKeys() const { return m_shortcutKeys; }

    private:
        std::vector<KeyCode> m_shortcutKeys;
        std::function<void(Window*, MenuItem*)> m_callback{nullptr};
    };
}

#endif // PTK_MENU_MENUITEM_HPP
