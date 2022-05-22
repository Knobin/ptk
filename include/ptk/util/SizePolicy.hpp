//
//  util/SizePolicy.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2022-05-22.
//

#ifndef PTK_UTIL_SIZEPOLICY_HPP
#define PTK_UTIL_SIZEPOLICY_HPP

// pTK Headers
#include "ptk/core/Types.hpp"

namespace pTK
{
    enum SizePolicyFlag : byte
    {
        NONE = 0,   // Size is fixed.
        Grow = 1,   // Size can grow.
        Shrink = 2  // Size can shrink.
    };

    enum class SizePolicy : byte
    {
        Fixed = SizePolicyFlag::NONE, // Fixed size.
        Expanding = SizePolicyFlag::Grow | SizePolicyFlag::Shrink // Size can be altered (either shrink or grow).
    };

    template<typename... Flags>
    constexpr bool IsSizePolicyFlagSet(std::underlying_type<SizePolicy>::type policy, Flags&&... flags) noexcept
    {
        using policy_utype = std::underlying_type<SizePolicy>::type;
        policy_utype sp = 0;

        for (const auto p : { flags... })
            sp |= static_cast<policy_utype>(p);

        return ((policy & sp) == sp);
    }
}

#endif // PTK_UTIL_SIZEPOLICY_HPP
