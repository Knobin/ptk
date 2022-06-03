//
//  core/Defines.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-21.
//

#ifndef PTK_CORE_DEFINES_HPP
#define PTK_CORE_DEFINES_HPP

// Build type.
#if !defined(PTK_DEBUG) && !defined(PTK_RELEASE)
    #ifdef NDEBUG
        #define PTK_RELEASE
    #else
        #define PTK_DEBUG
    #endif
#endif

// Enable debug options.
#if defined(PTK_DEBUG)
    #if !defined(PTK_ENABLE_ASSERT)
        #define PTK_ENABLE_ASSERT
    #endif
#endif

// Check platform.
#if !defined(PTK_PLATFORM_WINDOWS) && defined(_WIN32)
    #define PTK_PLATFORM_WINDOWS
#elif !defined(PTK_PLATFORM_APPLE) && defined(__APPLE__)
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        #define PTK_PLATFORM_APPLE
    #endif
#elif !defined(PTK_PLATFORM_UNIX) && defined(__unix__)
    #define PTK_PLATFORM_UNIX
#endif

// Check compiler.
#if !defined(PTK_COMPILER_CLANG) && defined(__clang__)
    #define PTK_COMPILER_CLANG
#elif !defined(PTK_COMPILER_GCC) && defined(__GNUC__)
    #define PTK_COMPILER_GCC
#elif !defined(PTK_COMPILER_MSVC) && defined(_MSC_VER)
    #define PTK_COMPILER_MSVC
#endif

// Shared library defines.
#if !defined(PTK_API)
    #if defined(PTK_SHARED)
        #if defined(PTK_COMPILER_MSVC)
            #if defined(PTK_EXPORTS)
                #define PTK_API __declspec(dllexport)
            #else
                #define PTK_API __declspec(dllimport)
            #endif
        #else
            #define PTK_API __attribute__((visibility("default")))
        #endif
    #else
        #define PTK_API
    #endif
#endif

// Unused parameter.
#if defined(PTK_COMPILER_GCC)
    #define UNUSED(x) x __attribute__((unused))
#elif defined(PTK_COMPILER_CLANG)
    #define UNUSED(x) x __attribute__((unused))
#elif defined(PTK_COMPILER_MSVC)
    #define UNUSED(x) __pragma(warning(suppress:4100)) x
#else
    #define UNUSED(x)
#endif

// Enable Asserts
#if defined(PTK_ENABLE_ASSERT)
    #define PTK_ASSERT(x, ...)   if(!(x)) { PTK_ERROR("Assertion Failed: {0}", __VA_ARGS__); abort(); }
#else
    #define PTK_ASSERT(...)
#endif

// Ignore all warnings (Begin)
#if !defined(PTK_DISABLE_WARN_BEGIN)
    #if defined(PTK_COMPILER_GCC)
        // TODO
        #define PTK_DISABLE_WARN_BEGIN()
    #elif defined(PTK_COMPILER_CLANG)
        #define PTK_DISABLE_WARN_BEGIN() \
            _Pragma("clang diagnostic push") \
            _Pragma("clang diagnostic ignored \"-Weverything\"")
    #elif defined(PTK_COMPILER_MSVC)
        #define PTK_DISABLE_WARN_BEGIN() __pragma(warning(push, 0))
    #else
        #define PTK_DISABLE_WARN_BEGIN()
    #endif
#endif

// Ignore all warnings (End)
#if !defined(PTK_DISABLE_WARN_END)
    #if defined(PTK_COMPILER_GCC)
        // TODO
        #define PTK_DISABLE_WARN_END()
    #elif defined(PTK_COMPILER_CLANG)
        #define PTK_DISABLE_WARN_END() _Pragma("clang diagnostic pop");
    #elif defined(PTK_COMPILER_MSVC)
        #define PTK_DISABLE_WARN_END() __pragma(warning(pop))
    #else
        #define PTK_DISABLE_WARN_END()
    #endif
#endif

#endif // PTK_CORE_DEFINES_HPP

