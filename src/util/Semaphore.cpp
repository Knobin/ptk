//
//  util/Semaphore.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

// pTK Headers
#include "ptk/util/Semaphore.hpp"

namespace pTK
{
    Semaphore::Semaphore(unsigned int count)
        : SingleObject(),
          m_mutex{},
          m_conditionVariable{},
          m_count{count}
    {}

    int Semaphore::post()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_count++;
        m_conditionVariable.notify_one();
        return 0;
    }

    int Semaphore::wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_count == 0)
            m_conditionVariable.wait(lock);

        m_count--;
        return 0;
    }

    int Semaphore::trywait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_count == 0)
            return -1;

        m_count--;
        return 0;
    }

    int Semaphore::timedwait(std::chrono::duration<double> duration)
    {
        using namespace std::chrono;
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_count == 0)
            if (!m_conditionVariable.wait_for(lock, duration_cast<nanoseconds>(duration), [&] {
                    return m_count != 0;
                }))
                return -1;

        m_count--;
        return 0;
    }

    int Semaphore::getvalue()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return static_cast<int>(m_count);
    }
} // namespace pTK
