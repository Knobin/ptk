//
//  core/CallbackStorage.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-27.
//

// pTK Headers
#include "ptk/core/CallbackStorage.hpp"

namespace pTK
{
    // CallbackIndexGen class static definitions.
    std::size_t CallbackIndexGen::s_counter{0};

    // CallbackStorage class static definitions.
    uint64_t CallbackStorage::s_idCounter{1};
} // namespace pTK
