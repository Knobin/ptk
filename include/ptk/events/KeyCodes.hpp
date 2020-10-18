//
//  events/KeyCodes.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-19.
//

#ifndef PTK_KEYCODES_HPP
#define PTK_KEYCODES_HPP

// Local Headers
#include "ptk/Core.hpp"

// TODO: Add more keycodes.

namespace pTK
{
    typedef enum class KeyCode : byte
    {
        Unknown = 0,
        Escape  = 27,
        Space   = 32,

        // Numbers
        D0      = 48,
        D1      = 49,
        D2      = 50,
        D3      = 51,
        D4      = 52,
        D5      = 53,
        D6      = 54,
        D7      = 55,
        D8      = 56,
        D9      = 57,

        // Letters
        A       = 65,
        B       = 66,
        C       = 67,
        D       = 68,
        E       = 69,
        F       = 70,
        G       = 71,
        H       = 72,
        I       = 73,
        J       = 74,
        K       = 75,
        L       = 76,
        M       = 77,
        N       = 78,
        O       = 79,
        P       = 80,
        Q       = 81,
        R       = 82,
        S       = 83,
        T       = 84,
        U       = 85,
        V       = 86,
        W       = 87,
        X       = 88,
        Y       = 89,
        Z       = 90

    } Key;
}



#endif //PTK_KEYCODES_HPP
