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

namespace pTK
{
    template<typename... Args>
    class MenuItemBase : public Args...
    {
    public:
        MenuItemBase()
            : Args()...
        {}
        virtual ~MenuItemBase() = default;

        template<typename Event, typename... FuncArgs>
        void handleEvent(FuncArgs&&... args) {
            static_assert(std::disjunction_v<std::is_same<Event, Args>...>, "Must be an Event that is inherited.");
            (static_cast<void>(CallEventFunc<Event, Args>(std::forward<FuncArgs>(args)...)), ...);
        }

    private:
        template<typename T, typename U, typename... FuncArgs>
            void CallEventFunc(FuncArgs&&... args) {
            if constexpr (std::is_same_v<T, U> && std::disjunction_v<std::is_same<T, Args>...>) {
                T::onEvent(std::forward<FuncArgs>(args)...);
            }
        }
    };
}

#endif // PTK_MENU_MENUITEMBASE_HPP
