//
//  util/Time.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-26.
//

// Local Headers
#include "ptk/util/Time.hpp"

namespace pTK
{
    Time::Time()
    {
        reset();
    }
    
    void Time::reset()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }
    
    uint32_t Time::nanoseconds() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_start).count();
    }
    
    uint32_t Time::microseconds() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(now - m_start).count();
    }
    
    uint32_t Time::milliseconds() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();
    }
    
    uint32_t Time::seconds() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(now - m_start).count();
    }
    
    uint32_t Time::minutes() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::minutes>(now - m_start).count();
    }
    
    uint32_t Time::hours() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::hours>(now - m_start).count();
    }
}
