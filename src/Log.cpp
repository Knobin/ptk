//
//  Log.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-17.
//

// Local Headers
#include "ptk/Log.hpp"

namespace pTK
{
    std::shared_ptr<spdlog::logger> Log::s_logger;
    
    void Log::init()
    {
        spdlog::set_pattern("%^[%Y-%m-%d %T] %n: %v%$");
        s_logger = spdlog::stdout_color_mt("pTK");
        s_logger->set_level(spdlog::level::debug);
    }
        
    std::shared_ptr<spdlog::logger>& Log::getLogger()
    {
        return s_logger;
    }
}
