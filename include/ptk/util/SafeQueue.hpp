//
//  util/SafeQueue.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-27.
//

#ifndef PTK_UTIL_SAFEQUEUE_HPP
#define PTK_UTIL_SAFEQUEUE_HPP

// C++ Headers
#include <queue>
#include <mutex>
#include <condition_variable>

namespace pTK
{
    /** SafeQueue class implementation.
     
        A thread safe queue.
        Implemented with Condition Variable, meaning that if you
        call front() or back() the thread will be blocked until
        push() has been called.
    */
    template<typename T>
    class SafeQueue
    {
    public:
        /** Constructs SafeQueue with default values.
         
            @return  default initialized SafeQueue
        */
        SafeQueue()
            : m_mutex{}, m_conditionVariable{}, m_queue{}
        {
        }
        
        ~SafeQueue() = default;
        
        /** Funtion for pushing an item to the queue.
         
            @param item    item to push
        */
        void push(const T& item)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(item);
            m_conditionVariable.notify_one();
        }
        
        /** Funtion for pushing an item to the queue.
         
            @param item    item to push
        */
        void push(T&& item)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(std::move(item));
            m_conditionVariable.notify_one();
        }
        
        /** Funtion for removing the first item in the queue.
         
        */
        void pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            if (!m_queue.empty())
                m_queue.pop();
        }
        
        /** Funtion for retrieving the item at the front in the queue.
         
            This function will block the thread until push() has been called.
            If multiple threads is waiting in this function, multiple push()
            may need to be called before the thread will wake up. Due to
            notify_one() in push().
         
            @return    item at the front
        */
        T front()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_queue.empty())
                m_conditionVariable.wait(lock);
            
            return m_queue.front();
        }
        
        /** Funtion for retrieving the item at the back in the queue.
         
            This function will block the thread until push() has been called.
            If multiple threads is waiting in this function, multiple push()
            may need to be called before the thread will wake up. Due to
            notify_one() in push().
            
            @return    item at the back
        */
        T back()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_queue.empty())
                m_conditionVariable.wait(lock);
            
            return m_queue.back();
        }
        
        /** Funtion for checking if the queue is empty or not.
         
            @return    empty status
        */
        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            return m_queue.empty();
        }
        
        /** Funtion for checking the size of the queue.
         
            @return    size
        */
        size_t size() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            return m_queue.size();
        }
        
    private:
        mutable std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
        std::queue<T> m_queue;
    };
}

#endif // PTK_UTIL_SAFEQUEUE_HPP
