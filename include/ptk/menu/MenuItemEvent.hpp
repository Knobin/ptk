//
//  menu/MenuItemEvent.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-14.
//

#ifndef PTK_MENU_MENUITEMEVENT_HPP
#define PTK_MENU_MENUITEMEVENT_HPP

// pTK Headers
#include "ptk/core/Types.hpp"
#include "ptk/menu/MenuItemType.hpp"

// C++ Headers
#include <functional>
#include <map>
#include <string>

namespace pTK
{
    namespace MenuItemEvent
    {
        template<typename Func>
        class Base
        {
        public:
            Base() = default;
            virtual ~Base() = default;

            std::map<std::string, Func> callbacks;
        };

        class Click : public Base<std::function<void()>>
        {
        public:
            Click() = default;
            virtual ~Click() = default;

            void onEvent() {
                for (auto& it : Base::callbacks)
                    it.second();
            }

            void onClick(const std::string& name, const std::function<void()>& func) {
                Base::callbacks[name] = func;
            }
        };

        class Status : public Base<std::function<void(MenuItemStatus, MenuItemStatus)>>
        {
        public:
            Status() = default;
            virtual ~Status() = default;

            void onEvent(MenuItemStatus from, MenuItemStatus to) {
                for (auto& it : Base::callbacks)
                    it.second(from, to);
            }

            void onStatus(const std::string& name, const std::function<void(MenuItemStatus, MenuItemStatus)>& func) {
                Base::callbacks[name] = func;
            }
        };

        struct Update : public Base<std::function<void()>>
        {
            Update() = default;
            virtual ~Update() = default;

            void onUpdate(const std::string& name, const std::function<void()>& func) {
                Base::callbacks[name] = func;
            }

            void onEvent() {
                for (auto& it : Base::callbacks)
                    it.second();
            }
        };
    }
}

#endif // PTK_MENU_MENUITEMEVENT_HPP
