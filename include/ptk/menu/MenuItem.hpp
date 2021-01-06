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

// C++ Headers
#include <functional>

namespace pTK
{
    class MenuItem : public MenuItemBase
    {
    public:
        MenuItem() = default;

        MenuItem(const std::string& name, const std::string& shortcut = "")
            : MenuItemBase(name), m_shortcut{shortcut}, m_callback{nullptr}
        {

        }

        MenuItem(const std::string& name, const std::string& shortcut, const std::function<void()>& func = nullptr)
            : MenuItemBase(name), m_shortcut{shortcut}, m_callback{func}
        {

        }

        virtual ~MenuItem() = default;

        void onClick(const std::function<void()>& func) { m_callback = func; }

        void handleClick()
        {
            if (m_callback)
                m_callback();
        }

        [[nodiscard]] std::string_view typeName() const override { return "MenuItem"; }

    private:
        std::string m_shortcut;
        std::function<void()> m_callback;
    };
}

#endif // PTK_MENU_MENUITEM_HPP
