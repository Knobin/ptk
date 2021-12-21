//
//  core/EventManager.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-21.
//

#ifndef PTK_CORE_EVENTMANAGER_HPP
#define PTK_CORE_EVENTMANAGER_HPP

// Local Headers
#include "Types.hpp"

// pTK Headers
#include "ptk/Log.hpp"

// C++ Headers
#include <functional>
#include <map>

namespace pTK
{
    /** EventManager class implementation.

        This class is for handling callbacks / listeners
        for events in the application.

        Every event in the application should be Triggered
        here so if the widget / user wants to act in the event.
    */
    class EventManager
    {
    public:
        /** Function to add a listener.

            @param callback     function to call on event
            @return             callback id
        */
        template <typename T>
        static uint64 AddListener(std::function<bool(T&)> callback)
        {
            uint64 id = s_counter++;
            getListeners<T>().insert({id, std::move(callback)});
            PTK_INFO("EventManager: Added callback with id: {}", id);
            return id;
        }

        /** Function to remove a listener with id.

            @param id     callback id to remove
        */
        template <typename T>
        static void RemoveListener(uint64 id)
        {
            if (id != 0)
            {
                auto& cont = getListeners<T>();
                for (auto it = cont.begin(); it != cont.end(); ++it)
                {
                    if (cont.first == id)
                    {
                        cont.erase(it);
                        PTK_INFO("EventManager: removed callback with id: {}", id);
                        break;
                    }
                }
            }
        }

        /** Function to trigger an event.

            @param event    triggered event
        */
        template <typename T>
        static void TriggerEvent(T& event)
        {
            auto& cont = getListeners<T>();

            for (auto it = cont.begin(); it != cont.end();)
                if (it->second(event))
                    cont.erase(it++);
                else
                    ++it;
        }

    private:
        template <typename T>
        static std::map<uint64, std::function<bool(T&)>>& getListeners()
        {
            static std::map<uint64, std::function<bool(T&)>> listeners{};
            return listeners;
        }

    private:
        static uint64 s_counter;
    };
}

#endif // PTK_CORE_EVENTMANAGER_HPP
