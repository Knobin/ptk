//
//  window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/core/eventhandling.hpp"

// C++ Headers
#include <string>

// OpenGL Headers
#include <OpenGL/gl.h>

// Skia
#include "include/gpu/GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"

// GLFW Headers
#include <GLFW/glfw3.h>

namespace pTK
{
    struct WindowData
    {
        std::string name;
        unsigned int width;
        unsigned int height;
    };

    class Window : public EventHandling
    {
    public:
        Window(const std::string& t_name, unsigned int t_width, unsigned int t_height);
        ~Window() final;

        void update();
        void swap_buffers();
        bool should_close();
        void resize(unsigned int t_width, unsigned int t_height);
        
        // Event processing
        void key_event(Event* t_event) override;
        void mouse_event(Event* t_event) override;
        void window_event(Event* t_event);

    private:
        // Window
        GLFWwindow* m_window;
        WindowData m_data;

        // Skia
        GrContext* m_context;
        SkSurface* m_surface;
        SkCanvas* m_canvas;
        
        // Init Functions
        void init_glfw();
        void init_skia();
        
        // Set Event Callbacks
        void set_window_callbacks();
        void set_mouse_callbacks();
        void set_key_callbacks();
    };
}

#endif // PTK_WINDOW_HPP
