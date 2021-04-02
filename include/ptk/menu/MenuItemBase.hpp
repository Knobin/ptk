//
//  menu/MenuItemBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUITEMBASE_HPP
#define PTK_MENU_MENUITEMBASE_HPP

// pTK Headers
#include "ptk/menu/MenuItemType.hpp"

// C++ Headers
#include <type_traits>
#include <map>

namespace pTK
{
    class MenuItemBase
    {
    public:
        MenuItemBase() = default;
        virtual ~MenuItemBase() = default;

        template<MenuItemEvent Event, typename... FuncArgs>
        void handleEvent(FuncArgs&&... args) {
            if constexpr (Event == MenuItemEvent::Update)
            {
                for (auto& it : m_updateCallbacks)
                    it.second();
            }
            else if constexpr (Event == MenuItemEvent::Status)
            {
                for (auto& it : m_statusCallbacks)
                    it.second(std::forward<FuncArgs>(args)...);
            }
        }

        void onUpdate(const std::string& name, const std::function<void()>& func) {
            m_updateCallbacks[name] = func;
        }

        void onStatus(const std::string& name, const std::function<void(MenuItemStatus, MenuItemStatus)>& func) {
            m_statusCallbacks[name] = func;
        }

        virtual void removeCallback(const std::string& name)
        {
            auto updateIt = m_updateCallbacks.find(name);
            if (updateIt != m_updateCallbacks.end())
                m_updateCallbacks.erase(updateIt);

            auto statusIt = m_statusCallbacks.find(name);
            if (statusIt != m_statusCallbacks.end())
                m_statusCallbacks.erase(statusIt);
        }

    private:
        std::map<std::string, std::function<void()>> m_updateCallbacks{};
        std::map<std::string, std::function<void(MenuItemStatus, MenuItemStatus)>> m_statusCallbacks{};
    };
}

#endif // PTK_MENU_MENUITEMBASE_HPP
