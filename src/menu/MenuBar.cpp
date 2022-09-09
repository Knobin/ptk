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
    MenuBar::MenuBar(const std::initializer_list<Ref<Menu>>& menus)
        : m_holder{menus}
    {}

    void MenuBar::addMenu(const Ref<Menu>& menu)
    {
        m_holder.push_back(menu);
    }

    void MenuBar::removeMenu(const Ref<Menu>& menu)
    {
        for (auto it = cbegin(); it != cend(); ++it)
            if (menu == (*it))
                m_holder.erase(it);
    }

    bool MenuBar::empty() const
    {
        return m_holder.empty();
    }

} // pTK
