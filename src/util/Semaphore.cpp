//
//  util/Semaphore.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

// Local Headers
#include "ptk/util/Semaphore.hpp"

namespace pTK
{
    Semaphore::Semaphore(unsigned int count)
        : m_mutex{}, m_conditionVariable{}, m_count{count}
    {
    }
    
    void Semaphore::up()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_count++;
        m_conditionVariable.notify_one();
    }
    
    void Semaphore::down()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_count == 0)
            m_conditionVariable.wait(lock);
        
        m_count--;
    }
}
