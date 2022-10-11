//
//  menu/CheckboxMenuItem.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// pTK Headers
#include "ptk/menu/CheckboxMenuItem.hpp"

namespace pTK
{
    CheckboxMenuItem::CheckboxMenuItem(const std::string& name, MenuItemStatus status)
        : NamedMenuItem(name, MenuItemType::Checkbox, status)
    {
        init();
    }
    CheckboxMenuItem::CheckboxMenuItem(const std::string& name, MenuItemStatus status, const Shortcut& shortcut)
        : NamedMenuItem(name, MenuItemType::Checkbox, status, shortcut)
    {
        init();
    }

    const std::map<std::string, std::function<void(MenuItemStatus)>>& CheckboxMenuItem::toggleCallbacks() const
    {
        return m_toggleCallbacks;
    }

    void CheckboxMenuItem::onToggle(const std::string& name, const std::function<void(MenuItemStatus)>& func)
    {
        m_toggleCallbacks[name] = func;
    }

    bool CheckboxMenuItem::checked() const
    {
        return status() == MenuItemStatus::Checked;
    }

    void CheckboxMenuItem::setChecked(bool checked)
    {
        if (checked && (status() == MenuItemStatus::Unchecked))
            setStatus(MenuItemStatus::Checked);
        else if (!checked && (status() == MenuItemStatus::Checked))
            setStatus(MenuItemStatus::Unchecked);
    }

    void CheckboxMenuItem::check()
    {
        setChecked(true);
    }

    void CheckboxMenuItem::uncheck()
    {
        setChecked(false);
    }

    void CheckboxMenuItem::init()
    {
        onStatus("CMI_s_func", [this](MenuItemStatus from, MenuItemStatus to) {
            if ((IsMenuItemStatusTypeCheckbox(from)) && IsMenuItemStatusTypeCheckbox(to))
            {
                for (const auto& it : this->toggleCallbacks())
                    it.second(to);
            }
        });

        onClick("CMI_c_func", [this]() {
            this->setChecked(!this->checked());
        });
    }

} // namespace pTK
