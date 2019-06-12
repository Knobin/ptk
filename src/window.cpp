//
//  window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// Local Headers
#include "ptk/window.hpp"

// C++ Headers
#include <exception>
#include <sstream>

namespace pTK
{
    Window::Window(const std::string& t_name, unsigned int t_width, unsigned int t_height)
        : m_window{nullptr}, m_data{t_name, t_width, t_height}
    {
        // Initialize and configure of glfw.
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create Window.
        m_window = glfwCreateWindow(t_width, t_height, t_name.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create GLFW Window.");
        }

        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

        // Initialize GLEW.
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            std::stringstream ss{"GLEW Error: "};
            ss << glewGetErrorString(err);
            glfwTerminate();
            throw std::logic_error(ss.str());
        }

        // OpenGL Clear Color.
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::update()
    {
        glfwPollEvents();
        process_input();
    }

    void Window::swap_buffers()
    {
        glfwSwapBuffers(m_window);
    }

    bool Window::should_close()
    {
        return (bool)glfwWindowShouldClose(m_window);
    }

    void Window::process_input()
    {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
    }

    void framebuffer_size_callback(GLFWwindow*, int t_width, int t_height)
    {
        glViewport(0, 0, t_width, t_height);
    }
}