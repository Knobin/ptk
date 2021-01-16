//
//  menu/MenuItemBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUITEMBASE_HPP
#define PTK_MENU_MENUITEMBASE_HPP

// pTK Headers
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
        virtual ~MenuItemBase() = default;

        void rename(const std::string& name) { m_name = name; }
        [[nodiscard]] const std::string& name() const { return m_name; }

        [[nodiscard]] virtual std::string_view typeName() const = 0;

    private:
        std::string m_name;
    };
}

#endif // PTK_MENU_MENUITEMBASE_HPP
