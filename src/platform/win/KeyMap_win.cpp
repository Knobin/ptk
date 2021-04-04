//
//  platform/win/KeyMap_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

// Local Headers
#include "KeyMap_win.hpp"

// Windows Headers
#define NOMINMAX
#include <windows.h>

namespace pTK
{
    std::map<int32, KeyCode> KeyCodeMapInit_win()
    {
        std::map<int32, KeyCode> map{};
        map[VK_SPACE] = Key::Space; map[VK_ESCAPE] = Key::Escape;
        map[0x30] = Key::D0; map[0x31] = Key::D1; map[0x32] = Key::D2; map[0x33] = Key::D3;
        map[0x34] = Key::D4; map[0x35] = Key::D5; map[0x36] = Key::D6; map[0x37] = Key::D7;
        map[0x38] = Key::D8; map[0x39] = Key::D9;
        map[VK_NUMPAD0] = Key::D0; map[VK_NUMPAD1] = Key::D1; map[VK_NUMPAD2] = Key::D2;
        map[VK_NUMPAD3] = Key::D3; map[VK_NUMPAD4] = Key::D4; map[VK_NUMPAD5] = Key::D5;
        map[VK_NUMPAD6] = Key::D6; map[VK_NUMPAD7] = Key::D7; map[VK_NUMPAD8] = Key::D8;
        map[VK_NUMPAD9] = Key::D9;
        map[0x41] = Key::A; map[0x42] = Key::B; map[0x43] = Key::C; map[0x44] = Key::D;
        map[0x45] = Key::E; map[0x46] = Key::F; map[0x47] = Key::G; map[0x48] = Key::H;
        map[0x49] = Key::I; map[0x4A] = Key::J; map[0x4B] = Key::K; map[0x4C] = Key::L;
        map[0x4D] = Key::M; map[0x4E] = Key::N; map[0x4F] = Key::O; map[0x50] = Key::P;
        map[0x51] = Key::Q; map[0x52] = Key::R; map[0x53] = Key::S; map[0x54] = Key::T;
        map[0x55] = Key::U; map[0x56] = Key::V; map[0x57] = Key::W; map[0x58] = Key::X;
        map[0x59] = Key::Y; map[0x5A] = Key::Z;

        map[VK_LSHIFT] = Key::LeftShift; map[VK_LCONTROL] = Key::LeftControl; map[VK_LMENU] = Key::LeftAlt;
        map[VK_RSHIFT] = Key::RightShift; map[VK_RCONTROL] = Key::RightControl; map[VK_RMENU] = Key::RightAlt;

        return map;
    }
} // namespace pTK