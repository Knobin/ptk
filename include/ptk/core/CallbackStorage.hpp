//
//  events/CallbackStorage.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-27.
//

#ifndef PTK_CORE_CALLBACKSTORAGE_HPP
#define PTK_CORE_CALLBACKSTORAGE_HPP

// pTK Headers
#include "ptk/core/Types.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <algorithm>
#include <functional>
#include <vector>

// Defines for logging in this file.
#if defined(PTK_CB_STORAGE_DEBUG) && defined(PTK_DEBUG)
    #define PTK_CB_STORAGE_LOG(...) PTK_INFO(__VA_ARGS__)
#else
    #define PTK_CB_STORAGE_LOG(...)
#endif

namespace pTK
{
    /** CallbackContainer class implementation.

        This class stores function of type Callback for the type T.

        Different Callback types will therefore be stored separately for
        the same type T.
    */
    template <typename T, typename Callback>
    class CallbackContainer
    {
    public:
        // Type of callback that will be stored.
        using callback_type = std::function<Callback>;

        // Each callback will be stored in a node together with
        // a unique identifer. Note: the uniqueness of the id will
        // not be checked, it is assumed that the id is unique when 
        // passed from adding the callback. 
        struct Node
        {
            Node(uint64 identifier, callback_type func)
                : id{identifier}, callback{func} 
            {}
            uint64 id;
            callback_type callback;
        };

        // Storage container.
        using container_type = std::vector<Node>;

    public:
        /** Constructs CallbackContainer with default values.

        */
        CallbackContainer() = default;

        /** Destructor for CallbackContainer.

        */
        ~CallbackContainer()
        {
#ifdef PTK_CB_STORAGE_DEBUG
            for (const Node& node : m_storage)
            {
                PTK_CB_STORAGE_LOG("Callback {} removed (destruction)", node.id);
            }
#endif
        }

        /** Function for adding a callback.

            @param id           unique identifier
            @param callback     function callback
        */
        void addCallback(uint64 id, const callback_type& callback)
        {
            m_storage.emplace_back(id, callback);
            PTK_CB_STORAGE_LOG("CallbackContainer: added callback with id: {}", id);
        }

        /** Function for removing a callback.

            @param id           unique identifier
            @return             true if removed, otherwise false
        */
        bool removeCallback(uint64 id)
        {
            auto it = std::find_if(m_storage.cbegin(), m_storage.cend(), [id](const Node& node) {
                return node.id == id;
            });

            if (it != m_storage.cend())
            {
                PTK_CB_STORAGE_LOG("CallbackContainer: removed callback with id: {}", id);
                m_storage.erase(it);
                return true;
            }

            return false;
        }

        /** Function for triggering all the callbacks.

        */
        template<typename... Args>
        void triggerCallbacks(Args&& ...args)
        {
            for (auto it = m_storage.begin(); it != m_storage.end();)
            {
                if (it->callback(std::forward<Args>(args)...))
                {
                    PTK_CB_STORAGE_LOG("CallbackContainer: auto-removed callback with id: {}", it->id);
                    it = m_storage.erase(it);
                }
                else
                    ++it;
            }
        }

    private:
        container_type m_storage;
    };

    /** CallbackIndexGen class implementation.

        This class generates unique identifiers for every type T and
        type Callback.

        The identifier starts at 0 and counts up for every new specialization
        generated of the GetIndex function.
    */
    struct CallbackIndexGen
    {
        /** Function for generating a unique identifier.

            @return     unique identifier
        */
        template <typename T, typename Callback>
        static std::size_t GetIndex()
        {
            static std::size_t index{s_counter++};
            return index;
        }

        // Counter variable used for counting.
        static std::size_t s_counter;
    };

    /** CallbackStorage class implementation.

        This class stores and handles all callbacks for specific types.

        It can store callbacks for any combination of T and Callback.
    */
    class CallbackStorage
    {
    public:
        using node_type = std::pair<void*, std::function<void(void*)>>;
        using container_type = std::vector<node_type>;

    public:
        /** Constructs CallbackStorage with default values.

        */
        CallbackStorage() = default;

        /** Destructor for CallbackStorage.

            Calls the destructor paired with every CallbackContainer in
            the storage.
        */
        ~CallbackStorage()
        {
            for (auto it{m_storage.begin()}; it != m_storage.end(); ++it)
                if (it->first != nullptr)
                    it->second(it->first);
        }

        /** Function for adding a callback of type Callback for type T.

            @param callback     function callback
            @return             unique identifier
        */
        template <typename T, typename Callback>
        uint64 addCallback(const std::function<Callback>& callback)
        {
            // Get index based on type T.
            std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Create node (and / or) get iterator to node.
            auto it = createNode<T, Callback>(index);

            // Cast data pointer.
            auto callbacks = static_cast<CallbackContainer<T, Callback>*>(it->first);

            // Finally insert the callback.
            // The id generated here must be unique (container assumes it is).
            uint64 id = s_idCounter++;
            callbacks->addCallback(id, callback);

            return id;
        }

        /** Function for removing a callback of type Callback with type T.

            @param id   unique identifier
            @return     true if removed, otherwise false
        */
        template <typename T, typename Callback>
        bool removeCallback(uint64 id)
        {
            // Get callbacks based on type of T.
            CallbackContainer<T, Callback>* cont = getCallbacks<T, Callback>();

            // Is container valid?
            if (cont != nullptr)
                return cont->removeCallback(id);

            // Not removed.
            return false;
        }

        /** Function for retrieving callbacks of type Callback with type T.

            @return     CallbackContainer if found, otherwise nullptr
        */
        template <typename T, typename Callback>
        CallbackContainer<T, Callback>* getCallbacks()
        {
            // Get index based on type T.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Does node exist?
            if (validIndex(index))
            {
                // Iterator to data at index.
                container_type::iterator it{ iteratorFromIndex(index) };

                // is node initialized?
                if (it->first != nullptr)
                    return static_cast<CallbackContainer<T, Callback>*>(it->first);
            }

            // Node not found or not initialized.
            return nullptr;
        }

        /** Function for triggering / handle callbacks.

        */
        template <typename T, typename Callback, typename... Args>
        void triggerCallbacks(Args&& ...args)
        {
            // Get callbacks based on type of T.
            CallbackContainer<T, Callback>* cont = getCallbacks<T, Callback>();

            // Is container valid?
            if (cont != nullptr)
                cont->triggerCallbacks(std::forward<Args>(args)...);
        }

    private:
        /** Check if the index is valid.

            @param index    index to check
            @return         true if valid, otherwise false
        */
        bool validIndex(std::size_t index)
        {
            return (index < m_storage.size());
        }

        /** Get iterator from index.

            @param index    index to use
            @return         iterator
        */
        container_type::iterator iteratorFromIndex(std::size_t index)
        {
            auto offset = static_cast<container_type::difference_type>(index);
            return std::next(m_storage.begin(), offset);
        }

        /** Create a new node (CallbackContainer).

            @param index    index to use
            @return         iterator
        */
        template <typename T, typename Callback>
        container_type::iterator createNode(std::size_t index)
        {
            // Resize if needed.
            if (!validIndex(index))
                m_storage.resize(index + 1, {nullptr, nullptr});

            // Iterator to data at index.
            container_type::iterator it{iteratorFromIndex(index)};

            // Check if container is created, if not, create it and set delete function.
            if (it->first == nullptr)
            {
                CallbackContainer<T, Callback> *container = new CallbackContainer<T, Callback>();
                it->first = static_cast<void*>(container);
                it->second = [](void *ptr) {
                    // Pointer is assumed to be valid here.
                    auto *cptr = static_cast<CallbackContainer<T, Callback>*>(ptr);
                    delete cptr;
                };
            }

            // Node is created and initialized.
            return it;
        }

    private:
        container_type m_storage{};
        static uint64 s_idCounter;
    };
}

#endif // PTK_CORE_CALLBACKSTORAGE_HPP
