//
//  events/CallbackStorage.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-27.
//

#ifndef PTK_CORE_CALLBACKSTORAGE_HPP
#define PTK_CORE_CALLBACKSTORAGE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>
#include <unordered_map>

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
        // a unique identifier. Note: the uniqueness of the id will
        // not be checked, it is assumed that the id is unique when 
        // passed from adding the callback. 
        struct Node
        {
            Node(uint64_t identifier, callback_type func)
                : id{identifier}, callback{func} 
            {}
            uint64_t id;
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
#ifdef PTK_CB_STORAGE_DEBUG
        ~CallbackContainer()
        {
            for (const Node& node : m_storage)
            {
                PTK_CB_STORAGE_LOG("Callback {} removed (destruction)", node.id);
            }
        }
#else
        ~CallbackContainer() = default;
#endif

        /** Move Constructor for CallbackContainer.

            @return    initialized CallbackContainer from value
        */
        CallbackContainer(CallbackContainer&& other) noexcept
            : m_storage(std::move(other.m_storage))
        {}

        /** Deleted Copy Constructor.

            Copying of callbacks can return undesired results, especially implicit copying.
            To get a deep copy of CallbackStorage, use the clone() function.

        */
        CallbackContainer(const CallbackContainer&) = delete;

        /** Move Assignment operator for CallbackContainer.

            @return    CallbackStorage with value
        */
        CallbackContainer& operator=(CallbackContainer&& other) noexcept
        {
            m_storage = std::move(other.m_storage);
            return *this;
        }

        /** Deleted Copy Assignment operator.

            Copying of callbacks can return undesired results, especially implicit copying.
            To get a deep copy of CallbackStorage, use the clone() function.

        */
        CallbackContainer& operator=(const CallbackContainer&) = delete;

        /** Copy function for CallbackContainer.

            Copying of callbacks can have undesired effects.
            Use this function with caution.

            @return    Copy of CallbackStorage
        */
        [[nodiscard]] CallbackContainer clone() const
        {
            CallbackContainer container{};
            container.m_storage = m_storage; // Copy contents of storage.
            return container;
        }

        /** Function for clearing the CallbackContainer.

        */
        void clear() noexcept
        {
            m_storage.clear();
        }

        /** Function for adding a callback.

            @param id           unique identifier
            @param callback     function callback
        */
        void addCallback(uint64_t id, const callback_type& callback)
        {
            m_storage.emplace_back(id, callback);
            PTK_CB_STORAGE_LOG("CallbackContainer: added callback with id: {}", id);
        }

        /** Function for removing a callback.

            @param id           unique identifier
            @return             true if removed, otherwise false
        */
        bool removeCallback(uint64_t id)
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

        /** Function for retrieving the callback count in the container.

            @return     number of callbacks
        */
        [[nodiscard]] std::size_t size() const noexcept
        {
            return m_storage.size();
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
    struct PTK_API CallbackIndexGen
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

    /** CallbackStorageNode Node struct implementation.

        "Member" functions for CallbackContainer<T, Callback> to use.
        All functions should take a void pointer and cast it for use.

        Pointer is always assumed to be valid.
    */
    template <typename T, typename Callback>
    struct PTK_API CallbackStorageNodeFunctions
    {
        static CallbackContainer<T, Callback>* Cast(void *ptr)
        {
            return static_cast<CallbackContainer<T, Callback>*>(ptr);
        }

        static void Deleter(void *ptr)
        {
            delete Cast(ptr);
        }

        static void *Clone(void *ptr)
        {
            CallbackContainer<T, Callback>* contPtr{Cast(ptr)};
            auto *copy = new CallbackContainer<T, Callback>();
            *copy = contPtr->clone();
            return static_cast<void*>(copy);
        }

        static std::size_t Count(void *ptr)
        {
            return Cast(ptr)->size();
        }
    };

    /** CallbackStorage class implementation.

        This class stores and handles all callbacks for specific types.

        It can store callbacks for any combination of T and Callback.
    */
    class PTK_API CallbackStorage
    {
    public:
        /** CallbackStorage Node struct implementation.

            Contains the data pointer and member function pointers to use.
        */
        struct PTK_API Node
        {
            std::function<void(void*)> deleter{nullptr};
            std::function<void*(void*)> clone{nullptr};
            std::function<std::size_t(void*)> count{nullptr};
            void *data{nullptr};
        };

        using node_type = Node;
        using container_type = std::unordered_map<std::size_t, node_type>;

    public:
        /** Constructs CallbackStorage with default values.

            @return    default initialized CallbackStorage
        */
        CallbackStorage() = default;

        /** Destructor for CallbackStorage.

            Calls the destructor paired with every CallbackContainer in
            the storage.
        */
        virtual ~CallbackStorage()
        {
            // Free used memory (if any).
            clear();
        }

        /** Move Constructor for CallbackStorage.

            @return    initialized CallbackStorage from value
        */
        CallbackStorage(CallbackStorage&& other) noexcept
            : m_storage(std::move(other.m_storage))
        {}

        /** Deleted Copy Constructor.

            Copying of callbacks can return undesired results, especially implicit copying.
            To get a deep copy of CallbackStorage, use the clone() function.

        */
        CallbackStorage(const CallbackStorage&) = delete;

        /** Move Assignment operator for CallbackStorage.

            @return    CallbackStorage with value
        */
        CallbackStorage& operator=(CallbackStorage&& other) noexcept
        {
            m_storage = std::move(other.m_storage);
            return *this;
        }

        /** Deleted Copy Assignment operator.

            Copying of callbacks can return undesired results, especially implicit copying.
            To get a deep copy of CallbackStorage, use the clone() function.

        */
        CallbackStorage& operator=(const CallbackStorage&) = delete;

        /** Copy function for CallbackStorage.

            Copying of callbacks can have undesired effects.
            Use this function with caution.

            The id for each callback is unique and should either be discarded
            since there exists 2 version of this callback now or the id can
            be used for indexing the copied callback.

            @return    Copy of CallbackStorage
        */
        [[nodiscard]] CallbackStorage clone() const
        {
            CallbackStorage copy{};
            copy.m_storage.reserve(m_storage.size());

            for (auto it = m_storage.cbegin(); it != m_storage.cend(); ++it)
            {
                const Node& from = it->second;

                // Copy node.

                Node to{};
                to.deleter = from.deleter;
                to.clone = from.clone;
                to.count = from.count;
                to.data = from.clone(from.data);

                copy.m_storage.emplace(it->first, to);
            }

            return copy;
        }

        /** Function for clearing the CallbackStorage.

        */
        void clear() noexcept
        {
            if (!m_storage.empty())
            {
                // Call destructor for all nodes.
                for (auto it{m_storage.begin()}; it != m_storage.end(); ++it)
                    if (it->second.data != nullptr)
                        it->second.deleter(it->second.data);

                m_storage.clear();
            }
        }

        /** Function for adding a callback of type Callback for type T.

            @param callback     function callback
            @return             unique identifier
        */
        template <typename T, typename Callback>
        uint64_t addCallback(const std::function<Callback>& callback)
        {
            // Get callbacks based on T & Callback types.
            CallbackContainer<T, Callback>* cont{getCallbackContainer<T, Callback>()};

            if (cont == nullptr)
                cont = createNode<T, Callback>();

            if (cont != nullptr)
            {
                // Insert the callback.
                // The id generated here must be unique (container assumes it is).
                uint64_t id = s_idCounter++;
                cont->addCallback(id, callback);
                return id;
            }

            return 0; // 0 is not used for id.
        }

        /** Function for removing a callback of type Callback with type T.

            @param id   unique identifier
            @return     true if removed, otherwise false
        */
        template <typename T, typename Callback>
        bool removeCallback(uint64_t id)
        {
            // Get callbacks based on T & Callback types.
            CallbackContainer<T, Callback>* cont{getCallbackContainer<T, Callback>()};

            // Is container valid?
            if (cont != nullptr)
            {
                auto status = cont->removeCallback(id);

                // Remove node is no callbacks exists.
                if (cont->size() == 0)
                    removeNode<T, Callback>();

                return status;
            }

            // Not removed.
            return false;
        }

        /** Function for removing all callbacks based on T & Callback types.

            @return     true if removed, otherwise false
        */
        template <typename T, typename Callback>
        bool removeCallbacks()
        {
            // Get callbacks based on T & Callback types.
            CallbackContainer<T, Callback>* cont{getCallbackContainer<T, Callback>()};

            // Is container valid?
            if (cont != nullptr)
            {
                cont->clear();
                removeNode<T, Callback>();
                return true;
            }

            // Not removed.
            return false;
        }

        /** Function for retrieving callbacks of type Callback with type T.

            @return     CallbackContainer if found, otherwise nullptr
        */
        template <typename T, typename Callback>
        [[nodiscard]] CallbackContainer<T, Callback>* getCallbacks() const
        {
            // Get index based on T & Callback types.
            CallbackContainer<T, Callback> *cont{getCallbackContainer<T, Callback>()};

            // is node initialized?
            if (cont != nullptr)
                return cont;

            // Node not found or not initialized.
            return nullptr;
        }

        /** Function for triggering / handle callbacks.

        */
        template <typename T, typename Callback, typename... Args>
        void triggerCallbacks(Args&& ...args)
        {
            // Get index based on T & Callback types.
            CallbackContainer<T, Callback> *cont{getCallbackContainer<T, Callback>()};

            // Is container valid?
            if (cont != nullptr)
            {
                cont->triggerCallbacks(std::forward<Args>(args)...);

                // Remove node is no callbacks exists.
                if (cont->size() == 0)
                    removeNode<T, Callback>();
            }
        }

        /** Function for retrieving the container (bucket) count in the storage.

            Note: Does not return the number of callbacks (use count() for that),
            only the amount of containers (buckets) currently in the storage.

            @return     number of containers in storage
        */
        [[nodiscard]] std::size_t size() const noexcept
        {
            return m_storage.size();
        }

        /** Function for retrieving the amount of callbacks in the storage.

            @return     number of callbacks in storage
        */
        [[nodiscard]] std::size_t count() const
        {
            std::size_t count{0};

            // Retrieve size of all containers.
            for (auto it{m_storage.begin()}; it != m_storage.end(); ++it)
                if (it->second.data != nullptr)
                    count += it->second.count(it->second.data);

            return count;
        }

    private:
        /** Function to create index mapping based on T & Callback types.

            @return     optional index
        */
        template <typename T, typename Callback>
        [[nodiscard]] CallbackContainer<T, Callback>* getCallbackContainer() const
        {
            // Get index based on T and Callback types.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Get key/value pair count.
            const auto count = m_storage.count(index);

            if (count == 1)
                return static_cast<CallbackContainer<T, Callback>*>(m_storage.at(index).data);

            return nullptr;
        }

        /** Function to create index mapping based on T & Callback types.

            Can be used for creating index mapping, if mapping already exists the
            current index will be returned.

            @return     optional index
        */
        template <typename T, typename Callback>
        [[nodiscard]] CallbackContainer<T, Callback> *createNode()
        {
            // Get index based on T and Callback types.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Get key/value pair count.
            auto count = m_storage.count(index);

            // Check how many pairs exists for that hash.
            if (count == 0)
            {
                // New Container for T and Callback types.
                auto *container = new CallbackContainer<T, Callback>();

                // Create Node.
                Node node{};
                node.data = static_cast<void*>(container);
                node.deleter = CallbackStorageNodeFunctions<T, Callback>::Deleter;
                node.clone = CallbackStorageNodeFunctions<T, Callback>::Clone;
                node.count = CallbackStorageNodeFunctions<T, Callback>::Count;

                // add the key/value pair.
                m_storage.emplace(index, node);
                return container;
            }

            return nullptr;
        }

        /** Function to remove node based on T & Callback types.

            Removes both the node and the index.
        */
        template <typename T, typename Callback>
        void removeNode()
        {
            // Get index based on T and Callback types.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Get key/value pair count.
            const auto count = m_storage.count(index);

            if (count > 0)
            {
                auto& node = m_storage.at(index);

                // is node initialized?
                if (node.data != nullptr)
                    node.deleter(node.data); // Will call delete on the pointer.

                m_storage.erase(index);
            }
        }

    private:
        container_type m_storage{};
        static uint64_t s_idCounter;
    };
}

#endif // PTK_CORE_CALLBACKSTORAGE_HPP
