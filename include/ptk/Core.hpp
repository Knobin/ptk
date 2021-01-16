//
//  Core.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-18.
//

#ifndef PTK_CORE_HPP
#define PTK_CORE_HPP

// pTK Headers
#include "ptk/Log.hpp"

// C++ Headers
#include <memory>

#ifdef PTK_DEBUG
    #define PTK_ENABLE_ASSERT
#endif

// Unused parameter.
#ifdef PTK_COMPILER_GCC
    #define UNUSED(x) x __attribute__((unused))
#elif PTK_COMPILER_CLANG
    #define UNUSED(x) x __attribute__((unused))
#elif PTK_COMPILER_MSVC
    #define UNUSED(x) __pragma(warning(suppress:4100)) x
#else
    #define UNUSED(x)
#endif

// Enable Asserts
#ifdef PTK_ENABLE_ASSERT
    #define PTK_ASSERT(x, ...)   if(!(x)) { PTK_ERROR("Assertion Failed: {0}", __VA_ARGS__); abort(); }
#else
    #define PTK_ASSERT(...)
#endif

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

