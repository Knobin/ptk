//
//  menu/MenuItemBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
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
        MenuItem(const std::string& name, const std::function<void()>& func = nullptr)
            : MenuItemBase(name),  m_callback{func}
        {

        }

        void onClick(const std::function<void()>& func) { m_callback = func; }

        void callClick()
        {
            if (m_callback)
                m_callback();
        }

        [[nodiscard]] std::string_view typeName() const override { return "MenuItem"; }

    private:
        std::function<void()> m_callback;
    };
}

#endif // PTK_MENU_MENUITEM_HPP
