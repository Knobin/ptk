//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// Local Headers
#include "ptk/Window.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WindowEvent.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <exception>
#include <sstream>
#include <iostream>

namespace pTK
{
    Window::Window(const std::string& name, unsigned int width, unsigned int height)
        : Container(), m_window{nullptr}, m_data{name, {width, height}, {1.0f, 1.0f}}, m_canvas{nullptr}
    {
        initGLFW();

        // Create Window.
        m_window = glfwCreateWindow((int)width, (int)height, name.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create GLFW Window.");
        }
        
        PTK_INFO("[Window] Created with w: {0:d}px and h: {0:d}px", width, height);
        
        // Get Monitor Scale
        glfwGetWindowContentScale(m_window, &m_data.scale.x, &m_data.scale.y);
        PTK_INFO("[Window] Monitor scale is x: {0:f} and y: {0:f}", m_data.scale.x, m_data.scale.y);
        
        // Bind context.
        glfwMakeContextCurrent(m_window);

        // Init Canvas
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        m_canvas = new Canvas(Vec2<uint32_t>(fbWidth, fbHeight));
        if (m_canvas == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create Canvas.");
        }
        
        // Set pointer for use in callbacks;
        glfwSetWindowUserPointer(m_window, this);
        
        setWindowCallbacks();
        setMouseCallbacks();
        setKeyCallbacks();
    }
    
    // Init Functions
    void Window::initGLFW()
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
    void Window::setWindowCallbacks()
    {
        // void window_size_callback(GLFWwindow* window, int width, int height)
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* t_window, int t_width, int t_height){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->handleWindowEvent(new ResizeEvent((unsigned int)t_width, (unsigned int)t_height));
        });
    }
    
    void Window::setMouseCallbacks()
    {
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* t_window, double t_xpos, double t_ypos){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->handleWindowEvent(new MotionEvent((int)t_xpos, (int)t_ypos));
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
                window->handleMouseEvent(new ButtonEvent(EventType::MouseButtonPressed, button, (int)xpos, (int)ypos));
            else if (t_action == GLFW_RELEASE)
                window->handleMouseEvent(new ButtonEvent(EventType::MouseButtonReleased, button, (int)xpos, (int)ypos));
        });
    }
    
    void Window::setKeyCallbacks()
    {
        // void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        glfwSetKeyCallback(m_window, [](GLFWwindow* t_window, int t_key, int, int t_action, int){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            if (t_action == GLFW_PRESS)
                window->handleKeyEvent(new KeyEvent(EventType::KeyPressed, t_key));
            else if (t_action == GLFW_RELEASE)
                window->handleKeyEvent(new KeyEvent(EventType::KeyReleased, t_key));
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
            widget->onDraw(m_canvas->skCanvas());
        });
        
        m_canvas->skCanvas()->flush();
    }
    
    void Window::update()
    {
        glfwPollEvents();
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    bool Window::shouldClose()
    {
        return (bool)glfwWindowShouldClose(m_window);
    }

    void Window::resize(unsigned int width, unsigned int height)
    {
        // Set Window Size.
        m_data.size.x = width;
        m_data.size.y = height;
        
        // Set Framebuffer Size.
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
        m_canvas->resize(Vec2<uint32_t>(fb_width, fb_height));
        
        PTK_INFO("ResizeEvent: W: {0:d}x{1:d}, FB: {2:d}x{3:d}", width, height, fb_width, fb_height);
    }

    // Event processing
    void Window::handleKeyEvent(Event* t_event)
    {
        KeyEvent* event = (KeyEvent*)t_event;
        if (event->type() == EventType::KeyPressed)
            std::cout << "Key pressed: " << event->get_keycode() << "\n";
        else if (event->type() == EventType::KeyReleased)
            std::cout << "Key released: " << event->get_keycode() << "\n";
        
        delete t_event;
    }

    void Window::handleMouseEvent(Event* t_event)
    {
        EventType type = t_event->type();
        if (type == EventType::MouseMoved)
        {
            MotionEvent* m_event = (MotionEvent*)t_event;
            std::shared_ptr<Widget> m_widget = rfind_if(Vec2<float>(m_event->get_posx(), m_event->get_posy()));
        } else if (type == EventType::MouseButtonPressed || type == EventType::MouseButtonReleased)
        {
            ButtonEvent* b_event = (ButtonEvent*)t_event;
            Vec2<int> pos(b_event->get_posx()*m_data.scale.x, b_event->get_posy()*m_data.scale.y);
            std::shared_ptr<Widget> b_widget = rfind_if(Vec2<float>(pos));
            if (b_widget != nullptr)
            {
                EventType type = b_event->type();
                if (type == EventType::MouseButtonPressed)
                    b_widget->handle_click_event(b_event->get_button(), pos);
                else if (type == EventType::MouseButtonReleased)
                    b_widget->handle_release_event(b_event->get_button(), pos);
            }
        }
        
        delete t_event;
    }

    void Window::handleWindowEvent(Event* event)
    {
        EventType type = event->type();
        if (type == EventType::WindowResize)
        {
            ResizeEvent* resize_event = (ResizeEvent*)event;
            resize(resize_event->get_width(), resize_event->get_height());
        }
        
        delete event;
    }
}
