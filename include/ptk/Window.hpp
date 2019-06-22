//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/core/Canvas.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/core/Container.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <string>

// GLFW Headers
#include <GLFW/glfw3.h>

namespace pTK
{
    struct WindowData
    {
        std::string name;
        Vec2<uint32_t> size;
        Vec2<float> scale;
    };

    class Window : public Container
    {
    public:
        Window(const std::string& t_name, unsigned int t_width, unsigned int t_height);
        virtual ~Window();

        void draw();
        void update();
        void swap_buffers();
        bool should_close();
        void resize(unsigned int t_width, unsigned int t_height);
        
        // Event processing
        void key_event(Event* t_event);
        void mouse_event(Event* t_event);
        void window_event(Event* t_event);

    private:
        // Window
        GLFWwindow* m_window;
        WindowData m_data;
        Canvas* m_canvas;

        // Init Functions
        void init_glfw();
        
        // Set Event Callbacks
        void set_window_callbacks();
        void set_mouse_callbacks();
        void set_key_callbacks();
    };
}

#endif // PTK_WINDOW_HPP
