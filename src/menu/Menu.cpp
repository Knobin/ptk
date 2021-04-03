//
//  menu/Menu.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// pTK Headers
#include "ptk/menu/Menu.hpp"

namespace pTK
{
    Menu::Menu(const std::string& name, const std::initializer_list<Ref<MenuItem>>& items)
        : MenuItem(MenuItemType::Menu, MenuItemStatus::Enabled), IterableSequence<Ref<MenuItem>>(items),
            m_name{name}
    {}

    [[nodiscard]] const std::string& Menu::name() const
    {
        return m_name;
    }

    void Menu::rename(const std::string& name)
    {
        m_name = name;
        handleEvent<MenuItemEvent::Update>();
    }

    void Menu::addItem(const Ref<MenuItem>& item)
    {
        container().push_back(item);
    }

    void Menu::removeItem(const Ref<MenuItem>& item)
    {
        for (auto it = cbegin(); it != cend(); ++it)
            if (item == (*it))
                container().erase(it);
    }

} // pTK
