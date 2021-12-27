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
#include <functional>
#include <map>
#include <vector>

// Defines for logging in this file.
#if defined(PTK_CB_STORAGE_DEBUG) && defined(PTK_DEBUG)
    #define PTK_CB_STORAGE_LOG(...) PTK_INFO(__VA_ARGS__)
#else
    #define PTK_CB_STORAGE_LOG(...)
#endif

namespace pTK
{
    // TODO: Add documentation for CallbackContainer.
    template <typename T>
    class CallbackContainer
    {
    public:
        using container_type = std::map<uint64, std::function<bool(const T&)>>;

    public:
        CallbackContainer() = default;
        ~CallbackContainer()
        {
#ifdef PTK_CB_STORAGE_DEBUG
            for (auto it{callbacks.cbegin()}; it != callbacks.cend(); ++it)
            {
                PTK_CB_STORAGE_LOG("Callback {} removed (destruction)", it->first);
            }
#endif
        }

        void addCallback(uint64 id, const std::function<bool(const T&)>& callback)
        {
            callbacks.insert({id, callback});
            PTK_CB_STORAGE_LOG("CallbackContainer: added callback with id: {}", id);
        }

        void removeCallback(uint64 id)
        {
            auto found = callbacks.find(id);
            if (found != callbacks.end())
            {
                PTK_CB_STORAGE_LOG("CallbackContainer: removed callback with id: {}", id);
                callbacks.erase(found);
            }
        }

        void triggerCallbacks(const T& evt)
        {
            for (auto it = callbacks.begin(); it != callbacks.end();)
            {
                if (it->second(evt))
                {
                    PTK_CB_STORAGE_LOG("CallbackContainer: auto-removed callback with id: {}", it->first);
                    callbacks.erase(it++);
                }
                else
                    ++it;
            }
        }

    private:
        container_type callbacks;
    };

    // TODO: Add documentation for CallbackIndexGen.
    struct CallbackIndexGen
    {
        template <typename T>
        static std::size_t GetIndex()
        {
            static std::size_t index{s_counter++};
            return index;
        }

        static std::size_t s_counter;
    };

    // TODO: Add documentation for CallbackStorage.
    class CallbackStorage
    {
    public:
        using node_type = std::pair<void*, std::function<void(void*)>>;
        using container_type = std::vector<node_type>;

    public:
        CallbackStorage() = default;
        ~CallbackStorage()
        {
            for (auto it{m_storage.begin()}; it != m_storage.end(); ++it)
                if (it->first != nullptr)
                    it->second(it->first);
        }

        template <typename T>
        uint64 addCallback(const std::function<bool(const T&)>& callback)
        {
            // Get index based on type T.
            std::size_t index = CallbackIndexGen::GetIndex<T>();

            // Create node (and / or) get iterator to node.
            auto it = createNode<T>(index);

            // Cast data pointer.
            auto callbacks = static_cast<CallbackContainer<T>*>(it->first);

            // Finally insert the callback.
            uint64 id = s_idCounter++;
            callbacks->addCallback(id, callback);

            return id;
        }

        template <typename T>
        void removeCallback(uint64 id)
        {
            // Get callbacks based on type of T.
            CallbackContainer<T> *cont = getCallbacks<T>();

            // Is container valid?
            if (cont != nullptr)
                cont->removeCallback(id);
        }

        template <typename T>
        CallbackContainer<T> *getCallbacks()
        {
            // Get index based on type T.
            std::size_t index = CallbackIndexGen::GetIndex<T>();

            // Does node exist?
            if (validIndex(index))
            {
                // Iterator to data at index.
                container_type::iterator it{iteratorFromIndex(index)};

                // is node initialized?
                if (it->first != nullptr)
                    return static_cast<CallbackContainer<T>*>(it->first);
            }

            // Node not found or not initialized.
            return nullptr;
        }

        template <typename T>
        void triggerCallbacks(const T& data)
        {
            // Get callbacks based on type of T.
            CallbackContainer<T> *cont = getCallbacks<T>();

            // Is container valid?
            if (cont != nullptr)
                cont->triggerCallbacks(data);
        }

    private:

        bool validIndex(std::size_t index)
        {
            return (index < m_storage.size());
        }

        container_type::iterator iteratorFromIndex(std::size_t index)
        {
            auto offset = static_cast<container_type::difference_type>(index);
            return std::next(m_storage.begin(), offset);
        }

        template <typename T>
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
                CallbackContainer<T> *container = new CallbackContainer<T>();
                it->first = static_cast<void*>(container);
                it->second = [](void *ptr) {
                    // Pointer is assumed to be valid here.
                    auto *cptr = static_cast<CallbackContainer<T>*>(ptr);
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
