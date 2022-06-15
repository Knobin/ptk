//
//  menu/MenuItem.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUITEM_HPP
#define PTK_MENU_MENUITEM_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"
#include "ptk/core/Types.hpp"
#include "ptk/menu/MenuItemType.hpp"

// C++ Headers
#include <functional>
#include <map>
#include <string>

namespace pTK
{
    /** MenuItem class implementation.

        Basic menu item class that has common stuff.
    */
    class PTK_API MenuItem
    {
    public:
        /** Deleted default constructor.

        */
        MenuItem() = delete;

        /** Constructs Menu with type and status.

            @param type     the type of the menu item
            @param status   the status of the menu item
            @return         initialized Menu
        */
        MenuItem(MenuItemType type, MenuItemStatus status);

        /** Destructor.

        */
        virtual ~MenuItem() = default;

        /** Function for retrieving the type of the menu item.

            @return     type of the menu item
        */
        [[nodiscard]] MenuItemType type() const;

        /** Function for retrieving the type of the menu item.

            @return     type of the menu item in string format
        */
        [[nodiscard]] std::string_view typeName() const;

        /** Function for retrieving the status of the menu item.

            @return     status of the menu item
        */
        [[nodiscard]] MenuItemStatus status() const;

        /** Function for setting the status of the menu item.

            @param status   new status to set
        */
        void setStatus(MenuItemStatus status);

        /** Function for signaling that something changed in the menu item.

        */
        void update();

        /** Template function for handling events.

        */
        template<MenuItemEvent Event, typename... FuncArgs>
        void handleEvent(FuncArgs&&... args);

        /** Template function for updating and handling events.

        */
        template<MenuItemEvent Event, typename... FuncArgs>
        void updateWithEvent(FuncArgs&&... args);

        /** Function for adding a update callback.

            @param name     name of the callback
            @param func     callback function
        */
        void onUpdate(const std::string& name, const std::function<void()>& func);

        /** Function for adding a status callback.

            @param name     name of the callback
            @param func     callback function
        */
        void onStatus(const std::string& name, const std::function<void(MenuItemStatus, MenuItemStatus)>& func);

        /** Function for removing a callback by name.

            @param name     name of the callback
        */
        virtual void removeCallback(const std::string& name);

    private:
        std::map<std::string, std::function<void()>> m_updateCallbacks{};
        std::map<std::string, std::function<void(MenuItemStatus, MenuItemStatus)>> m_statusCallbacks{};

        MenuItemType m_type{};
        MenuItemStatus m_status{};
    };

    template<MenuItemEvent Event, typename... FuncArgs>
    void MenuItem::handleEvent(FuncArgs&&... args) {
        if constexpr (Event == MenuItemEvent::Update)
        {
            for (const auto& it : m_updateCallbacks)
                it.second();
        }
        else if constexpr (Event == MenuItemEvent::Status)
        {
            for (const auto& it : m_statusCallbacks)
                it.second(std::forward<FuncArgs>(args)...);
        }
    }

    template<MenuItemEvent Event, typename... FuncArgs>
    void MenuItem::updateWithEvent(FuncArgs&&... args)
    {
        if constexpr (Event != MenuItemEvent::Update)
            handleEvent<Event>(std::forward<FuncArgs>(args)...);
        update();
    }
}

#endif // PTK_MENU_MENUITEM_HPP
