//
//  menu/CheckboxMenuItem.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-14.
//

#ifndef PTK_MENU_CHECKBOXMENUITEM_HPP
#define PTK_MENU_CHECKBOXMENUITEM_HPP

// pTK Headers
#include "ptk/menu/NamedMenuItem.hpp"

namespace pTK
{
    class CheckboxMenuItem : public NamedMenuItem
    {
    public:
        CheckboxMenuItem() = delete;
        CheckboxMenuItem(const std::string& name, MenuItemStatus status = MenuItemStatus::Unchecked)
            : NamedMenuItem(name, MenuItemType::Checkbox, status)
        {
            init();
        }
        CheckboxMenuItem(const std::string& name, MenuItemStatus status, const Shortcut& shortcut)
            : NamedMenuItem(name, MenuItemType::Checkbox, status, shortcut)
        {
            init();
        }
        virtual ~CheckboxMenuItem() = default;

        [[nodiscard]] const std::map<std::string, std::function<void(MenuItemStatus)>>& toggleCallbacks() const
        {
            return m_toggleCallbacks;
        }

        void onToggle(const std::string& name, const std::function<void(MenuItemStatus)>& func)
        {
            m_toggleCallbacks[name] = func;
        }

        [[nodiscard]] bool checked() const
        {
            return static_cast<bool>(status() == MenuItemStatus::Checked);
        }

        void setChecked(bool checked)
        {
            if (checked && (status() == MenuItemStatus::Unchecked))
                setStatus(MenuItemStatus::Checked);
            else if (!checked && (status() == MenuItemStatus::Checked))
                setStatus(MenuItemStatus::Unchecked);
        }

        void check()
        {
            setChecked(true);
        }

        void uncheck()
        {
            setChecked(false);
        }

    private:
        void init()
        {
            onStatus("CMI::toggle::func", [this](MenuItemStatus from, MenuItemStatus to){
                if ((IsMenuItemStatusTypeCheckbox(from)) && IsMenuItemStatusTypeCheckbox(to))
                {
                    for (const auto& it : this->toggleCallbacks())
                        it.second(to);
                }
            });

            onClick("CMI::toggle::func", [this](){
                this->setChecked(!this->checked());
            });
        }

    private:
        std::map<std::string, std::function<void(MenuItemStatus)>> m_toggleCallbacks;
    };

} // pTK

#endif // PTK_MENU_CHECKBOXMENUITEM_HPP
