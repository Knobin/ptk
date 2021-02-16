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
#include "ptk/menu/MenuItemEvent.hpp"

// C++ Headers
#include <functional>

namespace pTK
{
    class MenuItem : public MenuItemBase<MenuItemEvent::Status, MenuItemEvent::Update>
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

        template<typename Event, typename... FuncArgs>
        void updateWithEvent(FuncArgs&&... args)
        {
            static_assert(!std::is_same_v<MenuItemEvent::Update, Event>, "Event cannot be Update");
            handleEvent<Event>(std::forward<FuncArgs>(args)...);
            handleEvent<MenuItemEvent::Update>();
        }

    private:
        MenuItemType m_type{};
        MenuItemStatus m_status{};
    };
}

#endif // PTK_MENU_MENUITEM_HPP
