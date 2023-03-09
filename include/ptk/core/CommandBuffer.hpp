//
//  core/CommandBuffer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-15.
//

#ifndef PTK_CORE_COMMMANDBUFFER_HPP
#define PTK_CORE_COMMMANDBUFFER_HPP

// C++ Headers
#include <functional>
#include <vector>

namespace pTK
{
    /** CommandBuffer class implementation.

    */
    template <typename Signature, template <typename, typename> class Container = std::vector,
              template <typename> class alloc = std::allocator>
    class CommandBuffer
    {
    public:
        using container_type = Container<std::function<Signature>, alloc<std::function<Signature>>>;
        using value_type = typename container_type::value_type;
        using reference = typename container_type::reference;
        using const_reference = typename container_type::const_reference;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs CommandBuffer with default values.

            @return     default initialized CommandBuffer
        */
        CommandBuffer() = default;

        /** Function for adding a command to the end of the buffer.

            @param func     callback
        */
        template <typename Func>
        void add(Func func)
        {
            m_container.push_back(std::move(func));
        }

        /** Function for invoking the first callback in the buffer.

            Will invoke and remove the first callback.

            @param args     function arguments
        */
        template <typename... Args>
        void invoke(Args&&... args)
        {
            std::invoke(m_container.front(), std::forward<Args>(args)...);
            m_container.erase(m_container.begin());
        }

        /** Function for invoking all the added commands.

            Will use the same arguments for all commands.

            @param args     function arguments
        */
        template <typename... Args>
        void batchInvoke(Args&&... args)
        {
            for (auto it{m_container.cbegin()}; it != m_container.cend(); ++it)
                std::invoke((*it), std::forward<Args>(args)...);
            m_container.clear();
        }

        /** Function for retrieving the size of the queue.

            @return    elements in queue
        */
        [[nodiscard]] std::size_t size() const { return m_container.size(); }

        /** Function for checking if the queue is empty.

            @return    status
        */
        [[nodiscard]] bool empty() const { return m_container.empty(); }

    private:
        container_type m_container{};
    };
} // namespace pTK

#endif // PTK_CORE_COMMMANDBUFFER_HPP
