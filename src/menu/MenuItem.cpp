//
//  menu/MenuItem.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// pTK Headers
#include "ptk/menu/MenuItem.hpp"

namespace pTK
{
    MenuItem::MenuItem(MenuItemType type, MenuItemStatus status)
        : m_type{type}, m_status{status}
    {}

    [[nodiscard]] MenuItemType MenuItem::type() const
    {
        return m_type;
    }

    [[nodiscard]] std::string_view MenuItem::typeName() const
    {
        return MenuItemTypeToStr(m_type);
    }

    [[nodiscard]] MenuItemStatus MenuItem::status() const
    {
        return m_status;
    }

    void MenuItem::setStatus(MenuItemStatus status)
    {
        MenuItemStatus old{m_status};
        m_status = status;
        updateWithEvent<MenuItemEvent::Status>(old, status);
    }

    void MenuItem::update()
    {
        handleEvent<MenuItemEvent::Update>();
    }

    void MenuItem::onUpdate(const std::string& name, const std::function<void()>& func)
    {
        m_updateCallbacks[name] = func;
    }

    void MenuItem::onStatus(const std::string& name, const std::function<void(MenuItemStatus, MenuItemStatus)>& func)
    {
        m_statusCallbacks[name] = func;
    }

    void MenuItem::removeCallback(const std::string& name)
    {
        auto updateIt = m_updateCallbacks.find(name);
        if (updateIt != m_updateCallbacks.end())
            m_updateCallbacks.erase(updateIt);

        auto statusIt = m_statusCallbacks.find(name);
        if (statusIt != m_statusCallbacks.end())
            m_statusCallbacks.erase(statusIt);
    }

} // pTK
