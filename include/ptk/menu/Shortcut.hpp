//
//  menu/Shortcut.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-02-15.
//

#ifndef PTK_MENU_SHORTCUT_HPP
#define PTK_MENU_SHORTCUT_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <vector>

namespace pTK
{
    class Shortcut
    {
    public:
        Shortcut() = default;
        Shortcut(const std::initializer_list<KeyCode>& modifiers, const KeyCode key)
            : m_modifiers{modifiers}, m_key{key}
        {}

        [[nodiscard]] const std::vector<KeyCode>& modifiers() const { return m_modifiers; }
        [[nodiscard]] KeyCode key() const { return m_key; }

    private:
        std::vector<KeyCode> m_modifiers{};
        KeyCode m_key{pTK::KeyCode::Unknown};
    };

} // pTK

#endif // PTK_MENU_SHORTCUT_HPP
