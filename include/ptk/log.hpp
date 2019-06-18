//
//  log.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-16.
//

#ifndef PTK_LOG_HPP
#define PTK_LOG_HPP

// spdlog Headers
#include "spdlog/spdlog.h"

#ifdef NDEBUG
    #define PTK_TRACE(...)  spdlog::trace(__VA_ARGS__)
    #define PTK_INFO(...)   spdlog::info(__VA_ARGS__)
    #define PTK_WARN(...)   spdlog::warn(__VA_ARGS__)
    #define PTK_ERROR(...)  spdlog::error(__VA_ARGS__)
    #define PTK_FATAL(...)  spdlog::fatal(__VA_ARGS__)
#else
    #define PTK_TRACE(...)
    #define PTK_INFO(...)
    #define PTK_WARN(...)
    #define PTK_ERROR(...)
    #define PTK_FATAL(...)
#endif

#endif // PTK_LOG_HPP
