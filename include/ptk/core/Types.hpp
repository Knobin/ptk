//
//  core/Types.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-27.
//

#ifndef PTK_CORE_TYPES_HPP
#define PTK_CORE_TYPES_HPP

// C++ Headers
#include <cstdint>
#include <memory>

typedef uint8_t     uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef uint64_t    uint64;

typedef int8_t      int8;
typedef int16_t     int16;
typedef int32_t     int32;
typedef int64_t     int64;

typedef uint8       byte;
typedef uint32      uint;

namespace pTK
{
    static int Auto     = -2;
    static int Infinite = -1;
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename... Args>
    inline auto create(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif // PTK_CORE_TYPES_HPP
