//
//  Log.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-16.
//

#ifndef PTK_LOG_HPP
#define PTK_LOG_HPP

// Enable Logging
#ifdef PTK_DEBUG

    // spdlog Headers
    #include "spdlog/spdlog.h"

    // C++ Headers
    #include <memory>

    namespace pTK
    {
        class Log
        {
        public:
            static void init();

            static std::shared_ptr<spdlog::logger>& getLogger();

        private:
            static std::shared_ptr<spdlog::logger> s_logger;
        };
    }

    #define PTK_INIT_LOGGING(...) pTK::Log::init(__VA_ARGS__)
    #define PTK_WARN(...)   pTK::Log::getLogger()->warn(__VA_ARGS__)
    #define PTK_ERROR(...)  pTK::Log::getLogger()->error(__VA_ARGS__)
    #define PTK_TRACE(...)  pTK::Log::getLogger()->trace(__VA_ARGS__)
    #define PTK_INFO(...)   pTK::Log::getLogger()->info(__VA_ARGS__)
    #define PTK_FATAL(...)  pTK::Log::getLogger()->critical(__VA_ARGS__)
#else
    #define PTK_INIT_LOGGING(...)
    #define PTK_WARN(...)
    #define PTK_ERROR(...)
    #define PTK_TRACE(...)
    #define PTK_INFO(...)
    #define PTK_FATAL(...)
#endif

#endif // PTK_LOG_HPP
