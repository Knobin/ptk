//
//  events/KeyMap.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

#ifndef PTK_EVENTS_KEYMAP_HPP
#define PTK_EVENTS_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    /** KeyMap class implementation.

        Contains the key mapping and provides an abstracted interface for the user.

        The key map is created using the KeyMapInit function in PlatformInit.hpp
        and will be different depending on the platform.
    */
    class PTK_API KeyMap : public SingleObject
    {
    public:
        /** Function for retrieving the KeyMap instance.

            @return     KeyMap instance
        */
        [[nodiscard]] static KeyMap& Get();

        /** Function for checking if a Key exists in the KeyMap.

            @param  key     Key to check
            @return         status
        */
        [[nodiscard]] static bool KeyExists(Key key);

        /** Function for checking if a Key exists in the KeyMap.

            @param  keycode     Keycode to check
            @return             status
        */
        [[nodiscard]] static bool KeyCodeExists(int32_t keycode);

        /** Function for converting a keycode to Key.

            @param  keycode     Keycode to convert
            @return             Key if found, otherwise Key::Unknown
        */
        [[nodiscard]] static Key KeyCodeToKey(int32_t keycode);

        /** Function for converting a Key to keycode.

            @param  key     Key to convert
            @return         Keycode if found, otherwise value of Key::Unknown
        */
        [[nodiscard]] static int32_t KeyToKeyCode(Key key);

    private:
        /** Constructs KeyMap with default values.

            @return    default initialized KeyMap
        */
        KeyMap();

        /** Destructor for KeyMap.

        */
        virtual ~KeyMap() = default;

    private:
        static KeyMap s_Instance;
        const std::map<int32_t, Key> m_keyMap; // TODO(knobin): Change this to std::unordered_map.
    };

} // namespace pTK

#endif // PTK_EVENTS_KEYMAP_HPP