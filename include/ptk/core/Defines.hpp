//
//  core/Defines.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-21.
//

#ifndef PTK_CORE_DEFINES_HPP
#define PTK_CORE_DEFINES_HPP

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

// Ignore all warnings (Begin)
#ifndef PTK_DISABLE_WARN_BEGIN
    #ifdef PTK_COMPILER_GCC
        // TODO
        #define PTK_DISABLE_WARN_BEGIN()
    #elif PTK_COMPILER_CLANG
        #define PTK_DISABLE_WARN_BEGIN() \
            _Pragma("clang diagnostic push") \
            _Pragma("clang diagnostic ignored \"-Weverything\"")
    #elif PTK_COMPILER_MSVC
        #define PTK_DISABLE_WARN_BEGIN() __pragma(warning(push, 0))
    #else
        #define PTK_DISABLE_WARN_BEGIN()
    #endif
#endif

// Ignore all warnings (End)
#ifndef PTK_DISABLE_WARN_END
    #ifdef PTK_COMPILER_GCC
        // TODO
        #define PTK_DISABLE_WARN_END()
    #elif PTK_COMPILER_CLANG
        #define PTK_DISABLE_WARN_END() _Pragma("clang diagnostic pop");
    #elif PTK_COMPILER_MSVC
        #define PTK_DISABLE_WARN_END() __pragma(warning(pop))
    #else
        #define PTK_DISABLE_WARN_END()
    #endif
#endif

#endif // PTK_CORE_DEFINES_HPP

