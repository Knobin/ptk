//
//  window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// GLEW Headers
#include <GL/glew.h>

// GLFW Headers
#include <GLFW/glfw3.h>

// C++ Headers
#include <string>
#include <queue>

#include "ptk/events/event.hpp"

namespace pTK
{
    struct WindowData
    {
        std::string name;
        unsigned int width;
        unsigned int height;
    };

    class Window
    {
    public:
        Window(const std::string& t_name, unsigned int t_width, unsigned int t_height);
        ~Window();

        void update();
        void swap_buffers();
        bool should_close();
        void push_event(Event* t_event) { m_events.push(t_event); }
        void resize(unsigned int t_width, unsigned int t_height);

    private:
        GLFWwindow* m_window;
        WindowData m_data;

        std::queue<Event*> m_events;
        void process_events();

        // Event processing
        void key_event(Event* t_event);
        void mouse_event(Event* t_event);
        void window_event(Event* t_event);
    };
}

#endif // PTK_WINDOW_HPP