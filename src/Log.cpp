//
//  Log.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-17.
//

// Local Headers
#include "include/Log.hpp"

// spdlog Headers
PTK_DISABLE_WARN_BEGIN()
#include "spdlog/sinks/stdout_color_sinks.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    std::shared_ptr<spdlog::logger> Log::s_logger;

    void Log::init()
    {
        static bool initialized = false;

        if (initialized)
        {
            PTK_WARN("Logger have already been initialized!");
            return;
        }

        spdlog::set_pattern("%^[%Y-%m-%d %T] %n: %v%$");
        s_logger = spdlog::stdout_color_mt("pTK");
        s_logger->set_level(spdlog::level::debug);
        initialized = true;
        PTK_INFO("Initialized Logger.");
    }

    std::shared_ptr<spdlog::logger>& Log::getLogger()
    {
        return s_logger;
    }
} // namespace pTK
