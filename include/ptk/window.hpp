//
//  window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// GLEW Headers
#include <GL/glew.h>

// GLFW Headers
#include <GLFW/glfw3.h>

// C++ Headers
#include <string>

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

    private:
        GLFWwindow* m_window;
        WindowData m_data;

        void process_input();
    };

    // GLFW Callbacks.
    void framebuffer_size_callback(GLFWwindow*, int t_width, int t_height);
};