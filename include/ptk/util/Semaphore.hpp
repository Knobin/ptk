//
//  util/Semaphore.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_UTIL_SEMAPHORE_HPP
#define PTK_UTIL_SEMAPHORE_HPP

// C++ Headers
#include <mutex>
#include <condition_variable>

namespace pTK
{
    /** Semaphore class implementation.
     
     A basic class for synchronizing threads.
     */
    class Semaphore
    {
    public:
        Semaphore() = delete;
        
        /** Constructs Semaphore with default values.
         
         @return  default initialized Semaphore
         */
        Semaphore(unsigned int count);
        ~Semaphore() = default;
        
        /** Funtion for incrementing count and notify a waiting
        thread (if any).
         
         */
        void up();
        
        /** Funtion for decrementing count, if count is zero, the
         thread will wait until up is called by another thread, then
         decrement count.
         
         */
        void down();
        
    private:
        std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
        unsigned int m_count;
    };
}

#endif // PTK_UTIL_SEMAPHORE_HPP
