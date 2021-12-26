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
#include <algorithm>
#include <functional>
#include <map>
#include <utility>
#include <vector>

namespace pTK
{
    // Widget forward declaration.
    class Widget;

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
        static uint64 AddListener(std::function<bool(const T&)> callback)
        {
            uint64 id = s_counter++;
            getListeners<T>().insert({id, std::move(callback)});
            PTK_INFO("EventManager: Added [GLOBAL] callback with id: {}", id);
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
                auto& listeners = getListeners<T>();

                auto found = listeners.find(id);
                if (found != listeners.end())
                {
                    listeners.erase(found);
                    PTK_INFO("EventManager: removed [GLOBAL] callback with id: {}", id);
                }
            }
        }

        /** Function to trigger an event.

            This will only trigger GLOBAL callbacks and NOT functions
            bound to widgets.

            @param event    triggered event
        */
        template <typename T>
        static void TriggerEvent(const T& event)
        {
            auto& cont = getListeners<T>();

            for (auto it = cont.begin(); it != cont.end();)
                if (it->second(event))
                {
                    PTK_INFO("EventManager: auto-removed [GLOBAL] callback with id: {}", it->first);
                    cont.erase(it++);
                }
                else
                    ++it;
        }

    private:

        template <typename T>
        static std::map<uint64, std::function<bool(const T&)>>& getListeners()
        {
            static std::map<uint64, std::function<bool(const T&)>> listeners{};
            return listeners;
        }

    private:
        static uint64 s_counter;
    };
}

#endif // PTK_CORE_EVENTMANAGER_HPP
