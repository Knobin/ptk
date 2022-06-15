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
    class PTK_API KeyMap : public SingleObject
    {
    public:
        static KeyMap& Get();
        static const std::map<int32, Key>& Map();
        static bool KeyExists(Key key);
        static Key KeyCodeToKey(int32 keycode);
        static int32 KeyToKeyCode(Key key);

    private:
        KeyMap();
        virtual ~KeyMap() = default;

        static KeyMap s_Instance;
        const std::map<int32, Key> m_keyMap;
    };

} // namespace pTK

#endif // PTK_EVENTS_KEYMAP_HPP