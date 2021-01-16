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
    class Window;
    class MenuItem;

    class MenuItemHandler
    {
    public:
        MenuItemHandler() = default;
        virtual ~MenuItemHandler() = default;

        virtual void update(MenuItem* UNUSED(item)) {};
        virtual void handleEvent(Window* UNUSED(window), MenuItem* UNUSED(item)) {};

        void lock() { m_lockStatus = true; }
        void unlock() { m_lockStatus = false; }

        [[nodiscard]] bool isLocked() const { return m_lockStatus; }

    private:
        bool m_lockStatus{false};
    };

    class MenuItemHandlerGuard
    {
    public:
        MenuItemHandlerGuard() = delete;
        MenuItemHandlerGuard(const MenuItemHandlerGuard&) = delete;
        MenuItemHandlerGuard& operator = (const MenuItemHandlerGuard&) = delete;

        explicit MenuItemHandlerGuard(MenuItemHandler *handler)
            : m_handler{handler}
        {
            m_handler->lock();
        }

        ~MenuItemHandlerGuard()
        {
            m_handler->unlock();
        }

    private:
        MenuItemHandler *m_handler{nullptr};
    };

    class MenuItem : public MenuItemBase
    {
    public:
        enum class Type : byte
        {
            Text = 0,   // Text item, can be pressed (button).
            Checkbox,   // Text with a checkbox, can be toggled.
        };

        enum class Status : byte
        {
            Enabled = 0,    // Can be selected.
            Disabled,       // Can not be selected and is "greyed" out.
            Checked,        // Should only be combined with Type::Checkbox.
            Unchecked,      // Should only be combined with Type::Checkbox.
        };

        static constexpr std::string_view TypeStr(const Type& type)
        {
            switch (type)
            {
                case Type::Text:
                    return "Text";
                case Type::Checkbox:
                    return "Checkbox";
            }
        }

        static constexpr std::string_view StatusStr(const Status& status)
        {
            switch (status)
            {
                case Status::Enabled:
                    return "Enabled";
                case Status::Disabled:
                    return "Disabled";
                case Status::Checked:
                    return "Checked";
                case Status::Unchecked:
                    return "Unchecked";
            }

            PTK_ASSERT(false, "Missing MenuItem::Status");
            return "";
        }

    public:
        MenuItem() = default;

        explicit MenuItem(const std::string& name, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_callback{func}
        {

        }

        MenuItem(const std::string& name, const Type& type, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_callback{func}, m_type{type}
        {
            if (m_type == Type::Checkbox)
                m_status = Status::Unchecked;
        }

         MenuItem(const std::string& name, const std::initializer_list<KeyCode>& shortcut)
            : MenuItemBase(name), m_shortcutKeys{shortcut}
        {

        }

        MenuItem(const std::string& name, const std::initializer_list<KeyCode>& shortcut, const std::function<void(Window*, MenuItem*)>& func = nullptr)
            : MenuItemBase(name), m_shortcutKeys{shortcut}, m_callback{func}
        {

        }

        virtual ~MenuItem() = default;

        void onEvent(const std::function<void(Window*, MenuItem*)>& func) { m_callback = func; }

        void handleEvent(Window *window)
        {
            if (m_handler && !m_handler->isLocked())
                m_handler->handleEvent(window, this);

            if (m_callback)
                m_callback(window, this);
        }

        [[nodiscard]] std::string_view typeName() const override { return "MenuItem"; }

        [[nodiscard]] const std::vector<KeyCode>& shortcutKeys() const { return m_shortcutKeys; }

        void setHandler(const Ref<MenuItemHandler>& handler) { m_handler = handler; }
        [[nodiscard]] Ref<MenuItemHandler> handler() const { return m_handler; }

        void setStatus(const Status& status) { m_status = status; update(); }
        [[nodiscard]] const Status& status() const { return m_status; }

        [[nodiscard]] const Type& type() const { return m_type; }

    private:
        void update() { if (m_handler && !m_handler->isLocked()) m_handler->update(this); }

    private:
        std::vector<KeyCode> m_shortcutKeys;
        std::function<void(Window*, MenuItem*)> m_callback{nullptr};

        Type m_type{Type::Text};
        Status m_status{Status::Enabled};
        Ref<MenuItemHandler> m_handler;
    };
}

#endif // PTK_MENU_MENUITEM_HPP
