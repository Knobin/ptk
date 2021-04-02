//
//  menu/Menu.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENU_HPP
#define PTK_MENU_MENU_HPP

// pTK Headers
#include "ptk/menu/MenuItem.hpp"
#include "ptk/Core.hpp"
#include "ptk/util/IterableContainer.hpp"

// C++ Headers
#include <vector>

namespace pTK
{

    class Menu : public MenuItem, public IterableSequence<Ref<MenuItem>>
    {
    public:
        Menu(const std::string& name, const std::initializer_list<Ref<MenuItem>>& items = {})
            : MenuItem(MenuItemType::Menu, MenuItemStatus::Enabled), IterableSequence<Ref<MenuItem>>(items),
              m_name{name}
        {}
        virtual ~Menu() = default;

        [[nodiscard]] const std::string& name() const { return m_name; }
        void rename(const std::string& name)
        {
            m_name = name;
            handleEvent<MenuItemEvent::Update>();
        }

        void addItem(const Ref<MenuItem>& item)
        {
            container().push_back(item);
        }

        void removeItem(const Ref<MenuItem>& item)
        {
            for (auto it = cbegin(); it != cend(); ++it)
                if (item == (*it))
                    container().erase(it);
        }

    private:
        std::string m_name{};
    };
}

#endif // PTK_MENU_MENU_HPP
