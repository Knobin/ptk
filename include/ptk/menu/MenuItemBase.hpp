//
//  menu/MenuItemBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

#ifndef PTK_MENU_MENUITEMBASE_HPP
#define PTK_MENU_MENUITEMBASE_HPP

// Local Headers
#include "ptk/core/Types.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    class MenuItemBase
    {
    public:
        MenuItemBase() = default;
        explicit MenuItemBase(const std::string& name) : m_name{name} {}

        void rename(const std::string& name) { m_name = name; }
        [[nodiscard]] const std::string& name() const { return m_name; }

    private:
        std::string m_name;
    };
}

#endif // PTK_MENU_MENUITEMBASE_HPP
