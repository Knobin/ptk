//
//  menu/MenuItemBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUITEM_HPP
#define PTK_MENU_MENUITEM_HPP

// pTK Headers
#include "ptk/menu/MenuItemBase.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/core/Types.hpp"

// C++ Headers
#include <functional>

namespace pTK
{
    class MenuItem : public MenuItemBase
    {
    public:
        MenuItem() = delete;
        MenuItem(MenuItemType type, MenuItemStatus status)
            : MenuItemBase(), m_type{type}, m_status{status}
        {}
        virtual ~MenuItem() = default;

        [[nodiscard]] MenuItemType type() const { return m_type; }
        [[nodiscard]] std::string_view typeName() const { return MenuItemTypeToStr(m_type); }
        [[nodiscard]] MenuItemStatus status() const { return m_status; }

        void setStatus(MenuItemStatus status)
        {
            MenuItemStatus old{m_status};
            m_status = status;
            updateWithEvent<MenuItemEvent::Status>(old, status);
        }

        void update()
        {
            handleEvent<MenuItemEvent::Update>();
        }

        template<MenuItemEvent Event, typename... FuncArgs>
        void updateWithEvent(FuncArgs&&... args)
        {
            if constexpr (Event != MenuItemEvent::Update)
                handleEvent<Event>(std::forward<FuncArgs>(args)...);
            update();
        }

    private:
        MenuItemType m_type{};
        MenuItemStatus m_status{};
    };
}

#endif // PTK_MENU_MENUITEM_HPP
