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
    /** Shortcut class implementation.

        Keyboard shortcut.
    */
    class PTK_API Shortcut
    {
    public:
        /** Constructs Shortcut with default values.

            @return     default initialized Shortcut
        */
        Shortcut() = default;

        /** Constructs Shortcut with modifiers and key.

            @param modifiers    sequence of modifier keys
            @param key          final key, must be of alpha type
            @return             initialized Shortcut
        */
        Shortcut(const std::initializer_list<KeyCode>& modifiers, const KeyCode key)
            : m_modifiers{modifiers},
              m_key{key}
        {}

        /** Function for retrieving the modifiers.

            @return     modifiers
        */
        [[nodiscard]] const std::vector<KeyCode>& modifiers() const { return m_modifiers; }

        /** Function for retrieving the key.

            @return     key
        */
        [[nodiscard]] KeyCode key() const { return m_key; }

    private:
        std::vector<KeyCode> m_modifiers{};
        KeyCode m_key{pTK::KeyCode::Unknown};
    };

} // namespace pTK

#endif // PTK_MENU_SHORTCUT_HPP
