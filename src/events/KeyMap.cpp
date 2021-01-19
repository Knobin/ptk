//
//  events/KeyMap.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

// Local Headers
#include "../platform/KeyMapInit.hpp"

// C++ Headers
#include <map>

namespace pTK::KeyMap
{
    static const std::map<int32, Key> s_keyMap{MapInit()};

    const std::map<int32, Key>& Map()
    {
        return s_keyMap;
    }

    bool KeyExists(Key key)
    {
        auto it = std::find_if(s_keyMap.cbegin(), s_keyMap.cend(), [&](const auto& pair){
            return pair.second == key;
        });

        return it != s_keyMap.cend();
    }

    Key KeyCodeToKey(int32 code)
    {
        std::map<int32, KeyCode>::const_iterator it{s_keyMap.find(code)};
        if (it != s_keyMap.cend())
            return it->second;

        return Key::Unknown;
    }

    int32 KeyToKeyCode(Key key)
    {
        auto it = std::find_if(s_keyMap.cbegin(), s_keyMap.cend(), [&](const auto& pair){
            return pair.second == key;
        });

        if (it != s_keyMap.cend())
            return it->first;

        return static_cast<int32>(Key::Unknown);
    }

} // namespace pTK::KeyMap