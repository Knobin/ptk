//
//  util/SafeQueue.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-27.
//

#ifndef PTK_UTIL_SAFEQUEUE_HPP
#define PTK_UTIL_SAFEQUEUE_HPP

// C++ Headers
#include <deque>
#include <mutex>
#include <condition_variable>
#include <utility>

namespace pTK
{
    /** SafeQueue class implementation.

        A thread safe wrapper for a container.
        The container as the second template parameter must satisfy the requirements
        of SequenceContainer and implement these following functions with the usual semantics:
            - push_back()
            - pop_front()
            - back()
            - front()

        The default container is std::deque.

        Implemented with Condition Variable, meaning that if you
        call front() or back() the thread will be blocked until
        push() has been called.

        Important to note is that all the comparison and assignment operators do not lock 
        the queues!
    */
    template<typename T, typename Container = std::deque<T>>
    class SafeQueue
    {
    public:
        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using size_type = typename Container::size_type;
        using container_type = Container;

    public:
        /** Constructs SafeQueue with default values.

            @return  default initialized SafeQueue
        */
        SafeQueue()
            : m_mutex{},
              m_conditionVariable{},
              m_cont{}
        {

        }

        /** Copy Constructs for SafeQueue.

            Note: Will only lock the SafeQueue it is copying from.
            There is no point in locking the current object whilst
            it is being created.

            @param other    to copy from
            @return         SafeQueue copy of other
        */
        SafeQueue(const SafeQueue& other)
            : m_mutex{},
              m_conditionVariable{},
              m_cont{}
        {
            std::unique_lock<std::mutex> lock(other.m_mutex);
            m_cont = other.m_cont;
        }

        /** Move Constructs for SafeQueue.

            Note: Will only lock the SafeQueue it is moving from.
            There is no point in locking the current object whilst
            it is being created.

            @param other    to move from
            @return         SafeQueue with values moved from other
        */
        SafeQueue(SafeQueue&& other)
        {
            std::unique_lock<std::mutex> lock(other.m_mutex);
            std::swap(m_cont, other.m_cont);
        }

        /** Destructor for SafeQueue.

        */
        ~SafeQueue() = default;

        /** Copy Assignment for SafeQueue.

            Note: Will not lock either queue!

            @param other    to copy from
            @return         SafeQueue copy of other
        */
        SafeQueue& operator=(const SafeQueue& other)
        {
            if (this == &other)
                return *this;

            m_cont = other.m_cont;
            return *this;
        }

        /** Move Assignment for SafeQueue.

            Note: Will not lock either queue!

            @param other    to move from
            @return         SafeQueue with values moved from other
        */
        SafeQueue& operator=(SafeQueue&& other)
        {
            if (this == &other)
                return *this;

            m_cont = std::move(other.m_cont);
            return *this;
        }

        /** Function for pushing an item to the queue.

            @param item    item to push
        */
        void push(const value_type& item)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cont.push_back(item);
            m_conditionVariable.notify_one();
        }

        /** Function for pushing an item to the queue.

            @param item    item to push
        */
        void push(value_type&& item)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cont.push_back(std::move(item));
            m_conditionVariable.notify_one();
        }

        /** Function for emplacing an item to the queue.

            @param item    item to emplace
        */
        template<typename... Args>
        void emplace(Args&&... args)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cont.emplace_back(std::forward<Args>(args)...);
            m_conditionVariable.notify_one();
        }

        /** Function for removing the first item in the queue.

        */
        void pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            if (!m_cont.empty())
                m_cont.pop_front();
        }

        /** Function for retrieving the item at the front in the queue.

            This function will block the thread until push() has been called.
            If multiple threads is waiting in this function, multiple push()
            may need to be called before the thread will wake up. Due to
            notify_one() in push().

            @return    reference to item at the front
        */
        reference front()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_cont.empty())
                m_conditionVariable.wait(lock);

            return m_cont.front();
        }

        /** Function for retrieving the item at the front in the queue.

            This function will block the thread until push() has been called.
            If multiple threads is waiting in this function, multiple push()
            may need to be called before the thread will wake up. Due to
            notify_one() in push().

            @return    const reference to item at the front
        */
        const_reference front() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_cont.empty())
                m_conditionVariable.wait(lock);

            return m_cont.front();
        }

        /** Function for retrieving the item at the back in the queue.

            This function will block the thread until push() has been called.
            If multiple threads is waiting in this function, multiple push()
            may need to be called before the thread will wake up. Due to
            notify_one() in push().

            @return    reference to item at the back
        */
        reference back()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_cont.empty())
                m_conditionVariable.wait(lock);

            return m_cont.back();
        }

        /** Function for retrieving the item at the back in the queue.

            This function will block the thread until push() has been called.
            If multiple threads is waiting in this function, multiple push()
            may need to be called before the thread will wake up. Due to
            notify_one() in push().

            @return    const reference to item at the back
        */
        const_reference back() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_cont.empty())
                m_conditionVariable.wait(lock);

            return m_cont.back();
        }

        /** Function for checking if the queue is empty or not.

            @return    empty status
        */
        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_cont.empty();
        }

        /** Function for checking the size of the queue.

            @return    size
        */
        size_type size() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            return m_cont.size();
        }

        /** Function for locking the queue.
            
            Will block if the mutex is not available.
        */
        void lock()
        {
            m_mutex.lock();
        }

        /** Function for trying to lock the queue.

            Will return instantly.

            @return    lock status
        */
        bool try_lock()
        {
            return m_mutex.try_lock();
        }

        /** Function for unlocking the queue.

            Will block if the mutex is not available.
        */
        void unlock()
        {
            m_mutex.unlock();
        }

    private:
        mutable std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
        Container m_cont;

        template<typename T1, typename Container1>
        friend bool operator==(const SafeQueue<T1, Container1>& lhs,
            const SafeQueue<T1, Container1>& rhs);

        template<typename T1, typename Container1>
        friend bool operator>(const SafeQueue<T1, Container1>& lhs,
            const SafeQueue<T1, Container1>& rhs);
    };

    template<typename T, typename Container>
    bool operator==(const SafeQueue<T, Container>& lhs, const SafeQueue<T, Container>& rhs)
    {
        return lhs.m_cont == rhs.m_cont;
    }

    template<typename T, typename Container>
    bool operator!=(const SafeQueue<T, Container>& lhs, const SafeQueue<T, Container>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Container>
    bool operator<(const SafeQueue<T, Container>& lhs, const SafeQueue<T, Container>& rhs)
    {
        return rhs > lhs;
    }

    template<typename T, typename Container>
    bool operator<=(const SafeQueue<T, Container>& lhs, const SafeQueue<T, Container>& rhs)
    {
        return !(lhs > rhs);
    }

    template<typename T, typename Container>
    bool operator>(const SafeQueue<T, Container>& lhs, const SafeQueue<T, Container>& rhs)
    {
        return lhs.m_cont > rhs.m_cont;
    }

    template<typename T, typename Container>
    bool operator>=(const SafeQueue<T, Container>& lhs, const SafeQueue<T, Container>& rhs)
    {
        return !(lhs < rhs);
    }
}

#endif // PTK_UTIL_SAFEQUEUE_HPP