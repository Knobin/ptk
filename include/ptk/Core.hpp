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
}

#endif // PTK_CORE_HPP

