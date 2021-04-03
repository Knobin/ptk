//
//  core/EventQueue.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_CORE_EVENTQUEUE_HPP
#define PTK_CORE_EVENTQUEUE_HPP

// pTK Headers
#include "ptk/util/SingleObject.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/events/WindowEvent.hpp"

// C++ Headers
#include <deque>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <memory>
#include <iostream>

namespace pTK
{
    /** EventQueue class implementation.

        TODO: Add documentation.
    */
    template<template <typename, typename> class Container = std::deque,
             template <typename> class alloc = std::allocator>
    class EventQueue : public SingleObject
    {
    public:
        using container_type = Container<std::unique_ptr<Event>, alloc<std::unique_ptr<Event>>>;
        using value_type = typename container_type::value_type;
        using reference = typename container_type::reference;
        using const_reference = typename container_type::const_reference;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs EventQueue with default values.

            @return            default initialized EventQueue
        */
        EventQueue() = default;

        /** Destructor for EventQueue.

        */
        virtual ~EventQueue() = default;

        template<typename T, typename... Args>
        bool push(Args&& ...args)
        {
            static_assert(std::is_base_of<Event, T>::value, "T should inherit from Event");

            std::unique_ptr<Event> event = std::make_unique<T>(std::forward<Args>(args)...);
            if constexpr (std::is_same_v<PaintEvent, T>)
            {
                PaintEvent *pEvent{static_cast<PaintEvent*>(event.get())};
                for (auto it{m_queue.rbegin()}; it != m_queue.rend(); ++it)
                {
                    if ((*it)->type == Event::Type::WindowPaint)
                    {
                        PaintEvent *evt{dynamic_cast<PaintEvent*>((*it).get())};
                        if (evt)
                            if ((pEvent->pos == evt->pos) && (pEvent->size == evt->size))
                                return false;
                    }
                }

            }
            m_queue.push_back(std::move(event));
            return true;
        }

        void pop()
        {
            m_queue.pop_front();
        }

        [[nodiscard]] std::size_t size() const
        {
            return m_queue.size();
        }

        /** Function for retrieving the item at the front in the queue.

            @return    reference to item at the front
        */
        [[nodiscard]] reference front()
        {
            return m_queue.front();
        }

        /** Function for retrieving the item at the front in the queue.

            @return    const reference to item at the front
        */
        [[nodiscard]] const_reference front() const
        {
            return m_queue.front();
        }

        [[nodiscard]] bool empty() const
        {
            return m_queue.empty();
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
        [[nodiscard]] bool try_lock()
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
        container_type m_queue{};
        mutable std::mutex m_mutex;
    };
}

#endif // PTK_CORE_EVENTQUEUE_HPP
