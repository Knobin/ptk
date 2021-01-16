//
//  util/Semaphore.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_UTIL_SEMAPHORE_HPP
#define PTK_UTIL_SEMAPHORE_HPP

// pTK Headers
#include "ptk/util/Singleton.hpp"

// C++ Headers
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace pTK
{
    /** Semaphore class implementation.
     
        A basic class for synchronizing threads.
    */
    class Semaphore : public Singleton
    {
    public:
        /** Deleted Constructor for Semaphore.

            Object should always be constructed with a count.
        */
        Semaphore() = delete;
        
        /** Constructs Semaphore with default values.
         
            @return  default initialized Semaphore
        */
        Semaphore(unsigned int count);

        /** Deconstructor for Semaphore.

        */
        virtual ~Semaphore() = default;
        
        /** Function for incrementing count and notify a waiting
            thread (if any).
        */
        int post();
        
        /** Function for decrementing count, if count is zero, the
            thread will wait until post() is called by another thread, then
            decrement count.
        */
        int wait();
        
        /** Same as wait() except if count is zero, it does not block but
            returns an error.
        */
        int trywait();
        
        /** Same as wait() except if count is zero, function will block and if
            specified time is reached, it will return an error. If count is greater
            than zero, the specified time will be skipped.
         
        */
        int timedwait(std::chrono::duration<double> duration);
        
        /** Function for returning the current value of count.
         
        */
        int getvalue();
        
    private:
        std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
        unsigned int m_count;
    };
}

#endif // PTK_UTIL_SEMAPHORE_HPP
