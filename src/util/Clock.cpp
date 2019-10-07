//
//  util/Clock.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-26.
//

// Local Headers
#include "ptk/util/Clock.hpp"

namespace pTK
{
    Clock::Clock()
        : m_running{true}
    {
        reset();
    }
    
    void Clock::reset()
    {
        m_start = std::chrono::high_resolution_clock::now();
        m_running = true;
    }
    
    void Clock::stop()
    {
        m_running = false;
        m_stop = std::chrono::high_resolution_clock::now();
    }
    
    uint64 Clock::nanoseconds() const
    {
        using namespace std::chrono;
        high_resolution_clock::time_point now = (m_running) ? high_resolution_clock::now() : m_stop;
        return static_cast<uint64>(duration_cast<std::chrono::nanoseconds>(now - m_start).count());
    }
    
    uint64 Clock::microseconds() const
    {
        using namespace std::chrono;
        high_resolution_clock::time_point now = (m_running) ? high_resolution_clock::now() : m_stop;
        return static_cast<uint64>(duration_cast<std::chrono::microseconds>(now - m_start).count());
    }
    
    uint64 Clock::milliseconds() const
    {
        using namespace std::chrono;
        high_resolution_clock::time_point now = (m_running) ? high_resolution_clock::now() : m_stop;
        return static_cast<uint64>(duration_cast<std::chrono::milliseconds>(now - m_start).count());
    }
    
    uint64 Clock::seconds() const
    {
        using namespace std::chrono;
        high_resolution_clock::time_point now = (m_running) ? high_resolution_clock::now() : m_stop;
        return static_cast<uint64>(duration_cast<std::chrono::seconds>(now - m_start).count());
    }
    
    uint32 Clock::minutes() const
    {
        using namespace std::chrono;
        high_resolution_clock::time_point now = (m_running) ? high_resolution_clock::now() : m_stop;
        return static_cast<uint32>(duration_cast<std::chrono::minutes>(now - m_start).count());
    }
    
    uint32 Clock::hours() const
    {
        using namespace std::chrono;
        high_resolution_clock::time_point now = (m_running) ? high_resolution_clock::now() : m_stop;
        return static_cast<uint32>(duration_cast<std::chrono::hours>(now - m_start).count());
    }
}
