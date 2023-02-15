//
//  core/CommandBuffer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-15.
//

#ifndef PTK_CORE_COMMMANDBUFFER_HPP
#define PTK_CORE_COMMMANDBUFFER_HPP

// pTK Headers
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <deque>
#include <functional>
#include <mutex>

namespace pTK
{
    /** CommandBuffer class implementation.

        Queue that holds function pointer of type void().

        Most functions are non blocking and therefore not thread safe.
        It's up to the user to lock & unlock when needed.
    */
    template <template <typename, typename> class Container = std::deque,
              template <typename> class alloc = std::allocator>
    class CommandBuffer : public SingleObject
    {
    public:
        using container_type = Container<std::function<void()>, alloc<std::function<void()>>>;
        using value_type = typename container_type::value_type;
        using reference = typename container_type::reference;
        using const_reference = typename container_type::const_reference;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs CommandBuffer with default values.

            @return     default initialized CommandBuffer
        */
        CommandBuffer() = default;

        /** Destructor for CommandBuffer.

        */
        virtual ~CommandBuffer() = default;

        /** Function for adding a command (function) to the queue (non block).

            @param func     callback
        */
        template <typename Func>
        void push(Func func)
        {
            m_queue.push_back(std::move(func));
        }

        /** Function for removing a command from the front of the queue (non block).

            @param func     callback
        */
        void pop() { m_queue.pop_front(); }

        /** Function for retrieving the size of the queue (non block).

            @return    elements in queue
        */
        [[nodiscard]] std::size_t size() const { return m_queue.size(); }

        /** Function for retrieving the item at the front in the queue (non block).

            @return    reference to item at the front
        */
        [[nodiscard]] reference front() { return m_queue.front(); }

        /** Function for retrieving the item at the front in the queue (non block).

            @return    const reference to item at the front
        */
        [[nodiscard]] const_reference front() const { return m_queue.front(); }

        /** Function for checking if the queue is empty (non block).

            @return    status
        */
        [[nodiscard]] bool empty() const { return m_queue.empty(); }

        /** Function for locking the queue.

            Will block if the mutex is not available.
        */
        void lock() { m_mutex.lock(); }

        /** Function for trying to lock the queue.

            Will return instantly.

            @return    lock status
        */
        [[nodiscard]] bool try_lock() { return m_mutex.try_lock(); }

        /** Function for unlocking the queue.

        */
        void unlock() { m_mutex.unlock(); }

    private:
        container_type m_queue{};
        mutable std::mutex m_mutex;
    };
} // namespace pTK

#endif // PTK_CORE_COMMMANDBUFFER_HPP
