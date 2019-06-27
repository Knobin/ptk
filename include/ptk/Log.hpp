//
//  Log.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-16.
//

#ifndef PTK_LOG_HPP
#define PTK_LOG_HPP

// spdlog Headers
#include "spdlog/spdlog.h"

// PTK_DEBUG will enable all debug defines.
#ifdef PTK_DEBUG
    #define PTK_ENABLE_ASSERT
    #define PTK_ENABLE_WARNING
    #define PTK_ENABLE_ERROR
    #define PTK_ENABLE_TRACE
    #define PTK_ENABLE_INFO
    #define PTK_ENABLE_FATAL
#endif

#ifdef PTK_ENABLE_ASSERT
    #define PTK_ASSERT(x, ...)   if(!(x)) { PTK_ERROR("Assertion Failed: {0}", __VA_ARGS__); }
#else
    #define PTK_ASSERT(...)
#endif

#ifdef PTK_ENABLE_WARNING
    #define PTK_WARN(...)   spdlog::warn(__VA_ARGS__)
#else
    #define PTK_WARN(...)
#endif

#ifdef PTK_ENABLE_ERROR
    #define PTK_ERROR(...)  spdlog::error(__VA_ARGS__)
#else
    #define PTK_ERROR(...)
#endif

#ifdef PTK_ENABLE_TRACE
    #define PTK_TRACE(...)  spdlog::trace(__VA_ARGS__)
#else
    #define PTK_TRACE(...)
#endif

#ifdef PTK_ENABLE_INFO
    #define PTK_INFO(...)   spdlog::info(__VA_ARGS__)
#else
    #define PTK_INFO(...)
#endif

#ifdef PTK_ENABLE_FATAL
    #define PTK_FATAL(...)  spdlog::fatal(__VA_ARGS__)
#else
    #define PTK_FATAL(...)
#endif

#endif // PTK_LOG_HPP
