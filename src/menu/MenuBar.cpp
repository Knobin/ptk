//
//  menu/MenuBar.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// pTK Headers
#include "ptk/menu/MenuBar.hpp"

namespace pTK
{
    MenuBar::MenuBar(const std::initializer_list<value_type>& menus)
        : m_holder{menus}
    {}

    void MenuBar::addMenu(const value_type& menu)
    {
        m_holder.push_back(menu);
    }

    void MenuBar::removeMenu(const value_type& menu)
    {
        for (auto it = cbegin(); it != cend(); ++it)
            if (menu == (*it))
                m_holder.erase(it);
    }

    bool MenuBar::empty() const
    {
        return m_holder.empty();
    }

} // namespace pTK
