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
    class Window;

    class MenuItem : public MenuItemBase
    {
    public:
        MenuItem() = default;

        explicit MenuItem(const std::string& name, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_callback{func}
        {

        }

         MenuItem(const std::string& name, const std::string& shortcut)
            : MenuItemBase(name), m_shortcut{shortcut}
        {

        }

        MenuItem(const std::string& name, const std::string& shortcut, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_shortcut{shortcut}, m_callback{func}
        {

        }

        virtual ~MenuItem() = default;

        void onClick(const std::function<void(Window*, MenuItem*)>& func) { m_callback = func; }

        void handleClick(Window *window)
        {
            if (m_callback)
                m_callback(window, this);
        }

        [[nodiscard]] std::string_view typeName() const override { return "MenuItem"; }

        [[nodiscard]] std::string shortcut() const { return m_shortcut; }

    private:
        std::string m_shortcut{};
        std::function<void(Window*, MenuItem*)> m_callback{nullptr};
    };
}

#endif // PTK_MENU_MENUITEM_HPP
