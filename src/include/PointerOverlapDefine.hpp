//
//  include/PointerOverlapDefine.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

#ifndef PTK_INCLUDE_POINTER_OVERLAP_DEFINE_BUFFER_HPP
#define PTK_INCLUDE_POINTER_OVERLAP_DEFINE_BUFFER_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

#ifdef PTK_DEBUG
    #include "Assert.hpp"

namespace pTK
{
    /** Function for checking if two pointers overlap.

        @param source       pointer to copy data from
        @param length       total amount of bytes at source pointer
        @param destination  pointer to destination
        @return             status
    */
    constexpr bool CheckPointerOverlap(const uint8_t* source, std::size_t length, const uint8_t* destination) noexcept
    {
        const uint8_t* start = source;
        const uint8_t* end = start + static_cast<std::ptrdiff_t>(length);

        return ((destination >= start) && (end >= destination));
    }
} // namespace pTK

    #define PTK_DEBUG_POINTER_OVERLAP_CHECK(...)                                                                       \
        PTK_ASSERT(!CheckPointerOverlap(__VA_ARGS__), "Source and Destination pointer overlap")
#else
    #define PTK_DEBUG_POINTER_OVERLAP_CHECK(...)
#endif

#endif // PTK_INCLUDE_POINTER_OVERLAP_DEFINE_BUFFER_HPP
