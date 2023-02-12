//
//  core/CallbackStorage.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-27.
//

#ifndef PTK_CORE_CALLBACKSTORAGE_HPP
#define PTK_CORE_CALLBACKSTORAGE_HPP

// pTK Headers
#include "ptk/Log.hpp"
#include "ptk/core/Defines.hpp"

// C++ Headers
#include <algorithm>
#include <cstdint>
#include <functional>
#include <unordered_map>
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

        This class stores function pointers of type Callback
        and provides an API to access and modify them.

        Callbacks are stored together with a unique identifier.
        This id will not be checked by this class, it is assumed
        that the user will provide a unique identifier.
    */
    template <typename Callback>
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
                : id{identifier},
                  callback{func}
            {}
            uint64_t id;
            callback_type callback;
        };

        // Storage container.
        using container_type = std::vector<Node>;

    public:
        /** Constructs CallbackContainer with default values.

            @return    initialized CallbackContainer
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
        void clear() noexcept { m_storage.clear(); }

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

            @param id       unique identifier
            @return         true if removed, otherwise false
        */
        bool removeCallback(uint64_t id)
        {
            auto it =
                std::find_if(m_storage.cbegin(), m_storage.cend(), [id](const Node& node) { return node.id == id; });

            if (it != m_storage.cend())
            {
                PTK_CB_STORAGE_LOG("CallbackContainer: removed callback with id: {}", id);
                m_storage.erase(it);
                return true;
            }

            return false;
        }

        /** Function for triggering all the callbacks.

            @param args     callback parameters
        */
        template <typename... Args>
        void triggerCallbacks(Args&&... args) const
        {
            for (auto it = m_storage.cbegin(); it != m_storage.cend(); ++it)
                it->callback(std::forward<Args>(args)...);
        }

        /** Function for triggering and conditionally removing callbacks.

            @param p     predicate
        */
        template <typename UnaryPredicate>
        void removeCallbackIf(UnaryPredicate p)
        {
            for (auto it = m_storage.begin(); it != m_storage.end();)
            {
                if (p(*it))
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
        [[nodiscard]] std::size_t size() const noexcept { return m_storage.size(); }

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

    /** CallbackStorageNodeInterface struct implementation.

        Pure virtual base class for CallbackStorageNode that provides
        the API necessary for CallbackStorage.
    */
    struct PTK_API CallbackStorageNodeInterface
    {
        virtual ~CallbackStorageNodeInterface() = default;
        [[nodiscard]] virtual std::size_t count() const noexcept = 0;
        [[nodiscard]] virtual std::unique_ptr<CallbackStorageNodeInterface> clone() const = 0;
        [[nodiscard]] virtual const void* data() const noexcept = 0;
        [[nodiscard]] virtual void* data() noexcept = 0;
    };

    /** CallbackStorageNode struct implementation.

        Templated implementation for type Callback.
    */
    template <typename Callback>
    struct CallbackStorageNode : public CallbackStorageNodeInterface
    {
        virtual ~CallbackStorageNode() = default;

        [[nodiscard]] std::size_t count() const noexcept override { return container.size(); }

        [[nodiscard]] std::unique_ptr<CallbackStorageNodeInterface> clone() const override
        {
            auto copy = std::make_unique<CallbackStorageNode<Callback>>();
            copy->container = container.clone();
            return copy;
        }

        [[nodiscard]] const void* data() const noexcept override { return static_cast<const void*>(&container); }

        [[nodiscard]] void* data() noexcept override { return static_cast<void*>(&container); }

        CallbackContainer<Callback> container{};
    };

    /** CallbackStorage class implementation.

        This class stores and handles all callbacks for specific types.
        It can store callbacks for any combination of T and Callback.
    */
    class PTK_API CallbackStorage
    {
    public:
        using node_type = std::unique_ptr<CallbackStorageNodeInterface>;
        using container_type = std::unordered_map<std::size_t, node_type>;

    public:
        /** Constructs CallbackStorage with default values.

            @return    default initialized CallbackStorage
        */
        CallbackStorage() = default;

        /** Destructor for CallbackStorage.

        */
        virtual ~CallbackStorage() = default;

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

            for (const auto& it : m_storage)
                copy.m_storage.emplace(it.first, it.second->clone());

            return copy;
        }

        /** Function for clearing the CallbackStorage.

        */
        void clear() noexcept { m_storage.clear(); }

        /** Function for adding a callback of type Callback for type T.

            @param callback     function callback
            @return             unique identifier
        */
        template <typename T, typename Callback>
        uint64_t addCallback(const std::function<Callback>& callback)
        {
            // Get callbacks based on T & Callback types.
            CallbackContainer<Callback>* cont{getCallbackContainer<T, Callback>()};

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
            CallbackContainer<Callback>* cont{getCallbackContainer<T, Callback>()};

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
            CallbackContainer<Callback>* cont{getCallbackContainer<T, Callback>()};

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

            @return     CallbackContainer<Callback> pointer if found, otherwise nullptr
        */
        template <typename T, typename Callback>
        [[nodiscard]] const CallbackContainer<Callback>* getCallbacks() const
        {
            return getCallbackContainer<T, Callback>();
        }

        /** Function for retrieving callbacks of type Callback with type T.

            @return     CallbackContainer<Callback> pointer if found, otherwise nullptr
        */
        template <typename T, typename Callback>
        [[nodiscard]] CallbackContainer<Callback>* getCallbacks()
        {
            return getCallbackContainer<T, Callback>();
        }

        /** Function for triggering / handle callbacks.

            @param args     callback parameters
        */
        template <typename T, typename Callback, typename... Args>
        void triggerCallbacks(Args&&... args) const
        {
            // Get index based on T & Callback types.
            const CallbackContainer<Callback>* cont{getCallbackContainer<T, Callback>()};

            // Is container valid?
            if (cont != nullptr)
                cont->triggerCallbacks(std::forward<Args>(args)...);
        }

        /** Function for triggering and conditionally removing callbacks.

            @param p     predicate
        */
        template <typename T, typename Callback, typename UnaryPredicate>
        void removeCallbackIf(UnaryPredicate p)
        {
            // Get index based on T & Callback types.
            CallbackContainer<Callback>* cont{getCallbackContainer<T, Callback>()};

            // Is container valid?
            if (cont != nullptr)
            {
                // Trigger and remove callbacks if necessary.
                cont->removeCallbackIf(p);

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
        [[nodiscard]] std::size_t size() const noexcept { return m_storage.size(); }

        /** Function for retrieving the amount of callbacks in the storage.

            @return     number of callbacks in storage
        */
        [[nodiscard]] std::size_t count() const
        {
            std::size_t count{0};

            // Retrieve size of all containers.
            for (auto it{m_storage.cbegin()}; it != m_storage.cend(); ++it)
                count += it->second->count();

            return count;
        }

    private:
        /** Function for retrieving the CallbackContainer based on T and Callback types.

            @return     CallbackContainer<Callback> pointer if found, otherwise nullptr
        */
        template <typename T, typename Callback>
        [[nodiscard]] const CallbackContainer<Callback>* getCallbackContainer() const
        {
            // Get index based on T and Callback types.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Get key/value pair count.
            const auto count = m_storage.count(index);

            if (count == 1)
            {
                const CallbackStorageNodeInterface* node{m_storage.at(index).get()};
                return static_cast<const CallbackContainer<Callback>*>(node->data());
            }

            return nullptr;
        }

        /** Function for retrieving the CallbackContainer based on T and Callback types.

            @return     CallbackContainer<Callback> pointer if found, otherwise nullptr
        */
        template <typename T, typename Callback>
        [[nodiscard]] CallbackContainer<Callback>* getCallbackContainer()
        {
            // Get index based on T and Callback types.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Get key/value pair count.
            const auto count = m_storage.count(index);

            if (count == 1)
            {
                CallbackStorageNodeInterface* node{m_storage.at(index).get()};
                return static_cast<CallbackContainer<Callback>*>(node->data());
            }

            return nullptr;
        }

        /** Function for creating a node based on T & Callback types.

            @return     CallbackContainer<Callback> pointer if node is created, otherwise nullptr
        */
        template <typename T, typename Callback>
        [[nodiscard]] CallbackContainer<Callback>* createNode()
        {
            // Get index based on T and Callback types.
            const std::size_t index = CallbackIndexGen::GetIndex<T, Callback>();

            // Get key/value pair count.
            auto count = m_storage.count(index);

            // Check how many pairs exists for that hash.
            if (count == 0)
            {
                auto status = m_storage.emplace(index, std::make_unique<CallbackStorageNode<Callback>>());

                if (status.second)
                    return static_cast<CallbackContainer<Callback>*>(status.first->second.get()->data());
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
                m_storage.erase(index);
        }

    private:
        container_type m_storage{};
        static uint64_t s_idCounter;
    };
} // namespace pTK

#endif // PTK_CORE_CALLBACKSTORAGE_HPP
