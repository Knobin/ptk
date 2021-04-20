//
//  Core.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-18.
//

#ifndef PTK_CORE_HPP
#define PTK_CORE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <memory>
#include <limits>

namespace pTK
{
    // Custom Ref pointer, will maybe be a custom class.
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename... Args>
    inline auto Create(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    // returns x + y if no overflow and max of T if overflow.
    template<typename T>
    T AddWithoutOverflow(T x, T y)
    {
        constexpr auto max = std::numeric_limits<T>::max();

        if (x > (max - y))
            return max;

        return x + y;
    }
}

#endif // PTK_CORE_HPP

