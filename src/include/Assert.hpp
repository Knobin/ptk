//
//  core/Defines.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-19.
//

// clang-format off

#ifndef PTK_SRC_CORE_ASSERT_HPP
#define PTK_SRC_CORE_ASSERT_HPP

// Local Headers
#include "../include/Log.hpp"

// Enable Asserts
#if defined(PTK_ENABLE_ASSERT)
    #define PTK_ASSERT(x, ...) do { if(!(x)) { PTK_ERROR("Assertion Failed: {0}", __VA_ARGS__); abort(); } } while(false)
#else
    #define PTK_ASSERT(...)
#endif

#endif // PTK_SRC_CORE_ASSERT_HPP

// clang-format on
