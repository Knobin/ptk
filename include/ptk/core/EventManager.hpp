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

        /** Function to add a listener for a widget.

            @param widget       pointer to widget
            @param callback     function to call on event
            @return             callback id
        */
        template <typename T>
        static uint64 AddListener(Widget *widget, std::function<bool(const T&)> callback)
        {
            if (widget != nullptr)
            {
                auto& cont = getWidgetListeners<T>();
                auto found = cont.find(widget);

                if (found == cont.end())
                {
                    // Not found, insert new node.

                    auto it = cont.insert({widget, {}});
                    PTK_ASSERT(it.second, "EventManager: Error inserting new node");
                    found = it.first;
                }

                // Insert new callback for widget with id.

                auto& callbacks = found->second;
                uint64 id = s_counter++;
                callbacks.push_back({id, std::move(callback)});
                PTK_INFO("EventManager: Added [WIDGET] callback with id: {}", id);
                return id;
            }

            // id = 0, does not exist.
            return 0;
        }

        /** Function to remove a listener with id.

            @param id     callback id to remove
        */
        template <typename T>
        static void RemoveListener(uint64 id)
        {
            if (id != 0)
            {
                bool removed{removeListener<T>(id)};

                if (!removed)
                    removeWidgetListener<T>(id);
            }
        }

        /** Function to remove a listener with id.

            @param widget   pointer to widget
            @param id       callback id to remove
        */
        template <typename T>
        static void RemoveListener(Widget *widget, uint64 id)
        {
            if ((id != 0) && (widget != nullptr))
            {
                bool removed{removeWidgetListener<T>(id)};

                if (!removed)
                    removeListener<T>(widget, id);
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

        /** Function to trigger an event.

            This will only trigger both GLOBAL and WIDGET callbacks in that order.

            @param event    triggered event
        */
        template <typename T>
        static void TriggerWidgetEvent(Widget *widget, const T& event)
        {
            // Trigger global callbacks.
            TriggerEvent<T>(event);

            // Trigger widget callbacks.
            auto& widgets = getWidgetListeners<T>();

            auto found = widgets.find(widget);
            if (found != widgets.end())
            {
                for (auto it = found->second.begin(); it != found->second.end();)
                    if (it->second(event))
                    {
                        PTK_INFO("EventManager: auto-removed [WIDGET] callback with id: {}", it->first);
                        found->second.erase(it++);
                    }
                    else
                        ++it;

                // Check if (top level) container needs to be removed.
                if (found->second.empty())
                {
                    widgets.erase(found);
                    PTK_INFO("EventManager: removed [WIDGET] callback container");
                }
            }
        }

    private:
        template <typename T>
        static std::map<uint64, std::function<bool(const T&)>>& getListeners()
        {
            static std::map<uint64, std::function<bool(const T&)>> listeners{};
            return listeners;
        }

        template <typename T>
        static std::map<Widget*, std::vector<std::pair<uint64, std::function<bool(const T&)>>>>& getWidgetListeners()
        {
            static std::map<Widget*, std::vector<std::pair<uint64, std::function<bool(const T&)>>>> listeners{};
            return listeners;
        }

        template <typename T>
        static bool removeListener(uint64 id)
        {
            auto& listeners = getListeners<T>();

            auto found = listeners.find(id);
            if (found != listeners.end())
            {
                listeners.erase(found);
                PTK_INFO("EventManager: removed [GLOBAL] callback with id: {}", id);
                return true;
            }

            return false;
        }

        template <typename T>
        static bool removeWidgetListener(Widget *widget, uint64 id)
        {
            auto& widgets = getWidgetListeners<T>();

            auto w_found = widgets.find(widget);
            if (w_found != widgets.end())
            {
                auto f_found = std::find(w_found->second.begin(), w_found->second.end(), id);
                if (f_found != w_found->second->end())
                {
                    w_found->second.erase(f_found);
                    PTK_INFO("EventManager: removed [GLOBAL] callback with id: {}", id);

                    // Check if (top level) container needs to be removed.
                    if (w_found->second.empty())
                    {
                        widgets.erase(w_found);
                        PTK_INFO("EventManager: removed [WIDGET] callback container");
                    }

                    return true;
                }
            }

            return false;
        }

        template <typename T>
        static bool removeWidgetListener(uint64 id)
        {
            auto& widgets = getWidgetListeners<T>();

            for (auto it = widgets.begin(); it != widgets.end(); ++it)
            {
                for (auto func_it = it->second.begin(); func_it != it->second->end(); ++func_it)
                {
                    if (func_it->first == id)
                    {
                        // Remove callback.
                        it->second.erase(func_it);
                        PTK_INFO("EventManager: removed [WIDGET] callback with id: {}", id);

                        // Check if (top level) container needs to be removed.
                        if (it->second.empty())
                        {
                            widgets.erase(it);
                            PTK_INFO("EventManager: removed [WIDGET] callback container");
                        }

                        return true;
                    }
                }
            }

            return false;
        }

    private:
        static uint64 s_counter;
    };
}

#endif // PTK_CORE_EVENTMANAGER_HPP
