//
//  platform/unix/KeyMap_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// Local Headers
#include "x11.hpp"

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK::Platform::KeyMapFactoryImpl
{
    //
    // Will probably rework keyboard input at some point, this works for now.
    //

    std::map<int32_t, KeyCode> Make()
    {
        // clang-format off

        std::map<int32_t, KeyCode> map{};
        map[XK_space] = Key::Space;
        map[XK_0] = Key::D0; map[XK_1] = Key::D1; map[XK_2] = Key::D2; map[XK_3] = Key::D3;
        map[XK_4] = Key::D4; map[XK_5] = Key::D5; map[XK_6] = Key::D6; map[XK_7] = Key::D7;
        map[XK_8] = Key::D8; map[XK_9] = Key::D9;
        map[XK_KP_0] = Key::D0; map[XK_KP_1] = Key::D1; map[XK_KP_2] = Key::D2;
        map[XK_KP_3] = Key::D3; map[XK_KP_4] = Key::D4; map[XK_KP_5] = Key::D5;
        map[XK_KP_6] = Key::D6; map[XK_KP_7] = Key::D7; map[XK_KP_8] = Key::D8;
        map[XK_KP_9] = Key::D9;
        map[XK_a] = Key::A; map[XK_b] = Key::B; map[XK_c] = Key::C; map[XK_d] = Key::D;
        map[XK_e] = Key::E; map[XK_f] = Key::F; map[XK_g] = Key::G; map[XK_h] = Key::H;
        map[XK_i] = Key::I; map[XK_j] = Key::J; map[XK_k] = Key::K; map[XK_l] = Key::L;
        map[XK_m] = Key::M; map[XK_n] = Key::N; map[XK_o] = Key::O; map[XK_p] = Key::P;
        map[XK_q] = Key::Q; map[XK_r] = Key::R; map[XK_s] = Key::S; map[XK_t] = Key::T;
        map[XK_u] = Key::U; map[XK_v] = Key::V; map[XK_w] = Key::W; map[XK_x] = Key::X;
        map[XK_y] = Key::Y; map[XK_z] = Key::Z;

        map[XK_Escape] = Key::Escape; map[XK_Return] = Key::Enter; map[XK_Tab] = Key::Tab;
        map[XK_BackSpace] = Key::Backspace; map[XK_Delete] = Key::Delete;

        map[XK_Right] = Key::Right; map[XK_Left] = Key::Left; map[XK_Down] = Key::Down; map[XK_Up] = Key::Up;

        map[XK_Home] = Key::Home; map[XK_End] = Key::End;

        map[XK_Shift_L] = Key::LeftShift; map[XK_Control_L] = Key::LeftControl;
        map[XK_Alt_L] = Key::LeftAlt; map[XK_Super_L] = Key::LeftSuper;

        map[XK_Shift_R] = Key::RightShift; map[XK_Control_R] = Key::RightControl;
        map[XK_Alt_R] = Key::RightAlt; map[XK_Super_R] = Key::RightSuper;

        // clang-format on

        return map;
    }

} // namespace pTK::Platform::KeyMapFactoryImpl
