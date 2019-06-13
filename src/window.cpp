//
//  window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// Local Headers
#include "ptk/window.hpp"
#include "ptk/events/keyevent.hpp"
#include "ptk/events/mouseevent.hpp"
#include "ptk/events/windowevent.hpp"

// C++ Headers
#include <exception>
#include <sstream>
#include <iostream>

namespace pTK
{
    Window::Window(const std::string& t_name, unsigned int t_width, unsigned int t_height)
        : EventHandler(), m_window{nullptr}, m_data{t_name, t_width, t_height}
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

        // Initialize GLEW.
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            std::stringstream ss{"GLEW Error: "};
            ss << glewGetErrorString(err);
            glfwTerminate();
            throw std::logic_error(ss.str());
        }
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);

        // Set Callbacks
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* t_window, int t_width, int t_height){
            auto window = static_cast<Window*>( glfwGetWindowUserPointer( t_window ) );
            window->push_event(new ResizeEvent((unsigned int)t_width, (unsigned int)t_height));
        });
        // key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        glfwSetKeyCallback(m_window, [](GLFWwindow* t_window, int t_key, int, int t_action, int){
            auto window = static_cast<Window*>( glfwGetWindowUserPointer( t_window ) );
            if (t_action == GLFW_PRESS)
                window->push_event(new KeyEvent(EventType::KeyPressed, t_key));
            else if (t_action == GLFW_RELEASE)
                window->push_event(new KeyEvent(EventType::KeyReleased, t_key));
        });
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* t_window, double t_xpos, double t_ypos){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->push_event(new MotionEvent((int)t_xpos, (int)t_ypos));
        });
        // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* t_window, int t_button, int t_action, int){
            auto window = static_cast<Window*>( glfwGetWindowUserPointer( t_window ) );
            MouseButton button;
            if (t_button == GLFW_MOUSE_BUTTON_LEFT)
                button = MouseButton::Left;
            else if (t_button == GLFW_MOUSE_BUTTON_MIDDLE)
                button = MouseButton::Middle;
            else if (t_button == GLFW_MOUSE_BUTTON_RIGHT)
                button = MouseButton::Right;
            else
                button = MouseButton::NONE;

            if (t_action == GLFW_PRESS)
                window->push_event(new ButtonEvent(EventType::MouseButtonPressed, button));
            else if (t_action == GLFW_RELEASE)
                window->push_event(new ButtonEvent(EventType::MouseButtonReleased, button));
        });
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::update()
    {
        glfwPollEvents();
        process_events();
    }

    void Window::swap_buffers()
    {
        glfwSwapBuffers(m_window);
    }

    bool Window::should_close()
    {
        return (bool)glfwWindowShouldClose(m_window);
    }

    void Window::resize(unsigned int t_width, unsigned int t_height)
    {
        glViewport(0, 0, t_width, t_height);
        std::cout << "Window Resized to: " << t_width << "x" << t_height << "\n";
    }

    void Window::process_events()
    {
        while (!m_event_queue.empty())
        {
            Event* event = pop_event();

            if (event->get_category() == EventCategory::Mouse)
                mouse_event(event);
            else if (event->get_category() == EventCategory::Key)
                key_event(event);
            else if (event->get_category() == EventCategory::Window)
                window_event(event);

            delete event;
        }
    }

    // Event processing
    void Window::key_event(Event* t_event)
    {
        KeyEvent* event = (KeyEvent*)t_event;
        if (event->get_type() == EventType::KeyPressed)
            std::cout << "Key pressed: " << event->get_keycode() << "\n";
        else if (event->get_type() == EventType::KeyReleased)
            std::cout << "Key released: " << event->get_keycode() << "\n";
    }

    void Window::mouse_event(Event* t_event)
    {
        EventType type = t_event->get_type();
        if (type == EventType::MouseMoved)
        {
            MotionEvent* motion_event = (MotionEvent*)t_event;
            std::cout << "Mouse moved to: " << motion_event->get_posx() << "x" << motion_event->get_posy() << "\n";
        } else if (type == EventType::MouseButtonPressed || type == EventType::MouseButtonReleased)
        {
            ButtonEvent* button_event = (ButtonEvent*)t_event;
            if (button_event->get_type() == EventType::MouseButtonPressed)
                std::cout << "Mouse pressed: " << (int)button_event->get_button() << "\n";
            else if (button_event->get_type() == EventType::MouseButtonReleased)
                std::cout << "Mouse released: " << (int)button_event->get_button() << "\n";
        }
    }

    void Window::window_event(Event* t_event)
    {
        EventType type = t_event->get_type();
        if (type == EventType::WindowResize)
        {
            ResizeEvent* resize_event = (ResizeEvent*)t_event;
            resize(resize_event->get_width(), resize_event->get_height());
        }
    }
}