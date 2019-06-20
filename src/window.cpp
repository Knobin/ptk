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
#include "ptk/log.hpp"

// C++ Headers
#include <exception>
#include <sstream>
#include <iostream>

namespace pTK
{
    Window::Window(const std::string& t_name, unsigned int t_width, unsigned int t_height)
        : EventHandling(), Container(), m_window{nullptr}, m_data{t_name, t_width, t_height}, m_canvas{nullptr}
    {
        init_glfw();

        // Create Window.
        m_window = glfwCreateWindow((int)t_width, (int)t_height, t_name.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create GLFW Window.");
        }
        
        PTK_INFO("[Window] Created with w: {0:d}px and h: {0:d}px", t_width, t_height);
        
        // Bind context.
        glfwMakeContextCurrent(m_window);

        // Init Canvas
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_canvas = new Canvas(width, height);
        if (m_canvas == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create Canvas.");
        }
        
        
        // Set pointer for use in callbacks;
        glfwSetWindowUserPointer(m_window, this);
        
        set_window_callbacks();
        set_mouse_callbacks();
        set_key_callbacks();
    }
    
    // Init Functions
    void Window::init_glfw()
    {
        // Initialize and configure of glfw.
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_STENCIL_BITS, 0);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);
    }
    
    // Set Event Callbacks
    void Window::set_window_callbacks()
    {
        // void window_size_callback(GLFWwindow* window, int width, int height)
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* t_window, int t_width, int t_height){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->window_event(new ResizeEvent((unsigned int)t_width, (unsigned int)t_height));
        });
    }
    
    void Window::set_mouse_callbacks()
    {
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* t_window, double t_xpos, double t_ypos){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->mouse_event(new MotionEvent((int)t_xpos, (int)t_ypos));
        });
        // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* t_window, int t_button, int t_action, int){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            
            double xpos, ypos;
            glfwGetCursorPos(t_window, &xpos, &ypos);
            
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
                window->mouse_event(new ButtonEvent(EventType::MouseButtonPressed, button, (int)xpos, (int)ypos));
            else if (t_action == GLFW_RELEASE)
                window->mouse_event(new ButtonEvent(EventType::MouseButtonReleased, button, (int)xpos, (int)ypos));
        });
    }
    
    void Window::set_key_callbacks()
    {
        // void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        glfwSetKeyCallback(m_window, [](GLFWwindow* t_window, int t_key, int, int t_action, int){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            if (t_action == GLFW_PRESS)
                window->key_event(new KeyEvent(EventType::KeyPressed, t_key));
            else if (t_action == GLFW_RELEASE)
                window->key_event(new KeyEvent(EventType::KeyReleased, t_key));
        });
    }

    Window::~Window()
    {
        delete m_canvas;
        glfwTerminate();
    }

    void Window::draw()
    {
        for_each([&](const std::shared_ptr<Widget>& widget){
            widget->draw(m_canvas->skcanvas());
        });
        
        m_canvas->skcanvas()->flush();
    }
    
    void Window::update()
    {
        glfwPollEvents();
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
        // Set Window Size.
        m_data.width = t_width;
        m_data.height = t_height;
        
        // Set Framebuffer Size.
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
        m_canvas->resize(fb_width, fb_height);
        
        PTK_INFO("ResizeEvent: W: {0:d}x{1:d}, FB: {2:d}x{3:d}", t_width, t_height, fb_width, fb_height);
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
            //std::cout << "Mouse moved to: " << motion_event->get_posx() << "x" << motion_event->get_posy() << "\n";
        } else if (type == EventType::MouseButtonPressed || type == EventType::MouseButtonReleased)
        {
            ButtonEvent* button_event = (ButtonEvent*)t_event;
            if (button_event->get_type() == EventType::MouseButtonPressed)
            PTK_INFO("ButtonEvent: Pressed: {0:d}, At: {1:d}x{2:d}", (int)button_event->get_button(), button_event->get_posx(), button_event->get_posy());
            else if (button_event->get_type() == EventType::MouseButtonReleased)
                PTK_INFO("ButtonEvent: Released: {0:d}, At: {1:d}x{2:d}", (int)button_event->get_button(), button_event->get_posx(), button_event->get_posy());
            
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
