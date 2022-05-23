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

// C++ Headers
#include <type_traits>

namespace pTK
{
    /** SizePolicy struct implementation.

        The SizePolicy is used to define how a size is to be interpreted.

        For example a relation between a size, minimal size, maximum size. With
        the policy the size can be expanded (grow or shrink between the min and max),
        or be fixed.

        The idea is that the PolicyFlag defines if it is possible to
        grow, shrink or be fixed.

        Policy then uses the Policy flag to define if it is fixed or can be
        expanded.
    */
    struct SizePolicy
    {
        // Defines some common types of SizePolicy.
        // For example, defining a fixed SizePolicy is done this way:
        // SizePolicy p = {SizePolicy::Policy::Fixed, SizePolicy::Policy::Fixed};
        // With these type this can instead be written as:
        // SizePolicy p = SizePolicy::Type::Fixed;
        struct Type
        {
            static const SizePolicy Fixed;
            static const SizePolicy Expanding;
        };

        enum PolicyFlag : byte
        {
            Fixed = 1,  // Size is fixed.
            Grow = 2,   // Size can grow.
            Shrink = 4  // Size can shrink.
        };

        enum class Policy : std::underlying_type<PolicyFlag>::type
        {
            Fixed = PolicyFlag::Fixed, // Fixed size.
            Expanding = PolicyFlag::Grow | PolicyFlag::Shrink // Size can be altered (either shrink or grow).
        };

        Policy horizontal{Policy::Fixed};
        Policy vertical{Policy::Fixed};
    };

    inline const SizePolicy SizePolicy::Type::Fixed = { SizePolicy::Policy::Fixed , SizePolicy::Policy::Fixed };
    inline const SizePolicy SizePolicy::Type::Expanding = { SizePolicy::Policy::Expanding , SizePolicy::Policy::Expanding };

    // Comparison operators.
    inline bool operator==(const SizePolicy& lhs, const SizePolicy& rhs) noexcept
    {
        return ((lhs.horizontal == rhs.horizontal) && (lhs.vertical == rhs.vertical));
    }

    inline bool operator!=(const SizePolicy& lhs, const SizePolicy& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    /** Function for checking if a specific PolicyFlag is set.

    */
    template<typename... Flags>
    constexpr bool IsSizePolicyFlagSet(std::underlying_type<SizePolicy::Policy>::type policy, Flags&&... flags) noexcept
    {
        using policy_utype = std::underlying_type<SizePolicy::Policy>::type;
        policy_utype sp = 0;

        for (const auto p : { flags... })
            sp |= static_cast<policy_utype>(p);

        return ((policy & sp) == sp);
    }

    /** Function for checking if a specific PolicyFlag is set in the Policy.

    */
    template<typename... Flags>
    constexpr bool IsSizePolicyFlagSet(SizePolicy::Policy policy, Flags&&... flags) noexcept
    {
        using policy_utype = std::underlying_type<SizePolicy::Policy>::type;
        policy_utype p = static_cast<policy_utype>(policy);

        return IsSizePolicyFlagSet(p, std::forward<Flags>(flags)...);
    }
}

#endif // PTK_UTIL_SIZEPOLICY_HPP
