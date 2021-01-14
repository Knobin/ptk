//
//  menu/Menu.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENU_HPP
#define PTK_MENU_MENU_HPP

// Local Headers
#include "ptk/menu/MenuItemBase.hpp"
#include "ptk/Core.hpp"
#include "ptk/util/IterableContainer.hpp"

// C++ Headers
#include <vector>

namespace pTK
{
    class Menu : public MenuItemBase, public IterableSequence<Ref<MenuItemBase>>
    {
    public:
        explicit Menu(const std::string& name = "")
            : MenuItemBase(name), IterableSequence<Ref<MenuItemBase>>()
        {

        }

        Menu(const std::string& name, const std::initializer_list<Ref<MenuItemBase>>& items)
            : MenuItemBase(name), IterableSequence<Ref<MenuItemBase>>(items)
        {

        }

        virtual ~Menu() = default;

        [[nodiscard]] std::string_view typeName() const override { return "Menu"; }
    };
}

#endif // PTK_MENU_MENU_HPP
