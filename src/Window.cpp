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
        : Container(), m_window{nullptr}, m_data{name, {width, height}, {1.0f, 1.0f}}, m_mainCanvas{nullptr}, m_drawCanvas{nullptr}, m_events{}
    {
        initGLFW();

        // Create Window.
        m_window = glfwCreateWindow((int)width, (int)height, name.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create GLFW Window.");
        }
        
        PTK_INFO("[Window] Created with w: {0:d}px and h: {1:d}px", (int)width, (int)height);
        
        // Get Monitor Scale
        glfwGetWindowContentScale(m_window, &m_data.scale.x, &m_data.scale.y);
        PTK_INFO("[Window] Monitor scale is x: {0:f} and y: {1:f}", m_data.scale.x, m_data.scale.y);
        
        // Bind context.
        glfwMakeContextCurrent(m_window);

        // Init Canvas
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        m_mainCanvas = new Canvas(Vec2<uint32_t>(fbWidth, fbHeight));
        if (m_mainCanvas == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create Canvas.");
        }
        
        m_drawCanvas = new Canvas(*m_mainCanvas, Vec2<uint32_t>(fbWidth, fbHeight));
        
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
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
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
            window->sendEvent(new ResizeEvent((unsigned int)t_width, (unsigned int)t_height));
        });
    }
    
    void Window::setMouseCallbacks()
    {
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* t_window, double t_xpos, double t_ypos){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->sendEvent(new MotionEvent((int)t_xpos, (int)t_ypos));
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
                window->sendEvent(new ButtonEvent(EventType::MouseButtonPressed, button, (int)xpos, (int)ypos));
            else if (t_action == GLFW_RELEASE)
                window->sendEvent(new ButtonEvent(EventType::MouseButtonReleased, button, (int)xpos, (int)ypos));
        });
    }
    
    void Window::setKeyCallbacks()
    {
        // void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        glfwSetKeyCallback(m_window, [](GLFWwindow* t_window, int t_key, int, int t_action, int){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            if (t_action == GLFW_PRESS)
                window->sendEvent(new KeyEvent(EventType::KeyPressed, t_key));
            else if (t_action == GLFW_RELEASE)
                window->sendEvent(new KeyEvent(EventType::KeyReleased, t_key));
        });
    }

    Window::~Window()
    {
        delete m_drawCanvas;
        delete m_mainCanvas;
        glfwTerminate();
    }

    void Window::draw()
    {
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        SkCanvas* canvas = m_drawCanvas->skCanvas();
        for_each([canvas](const std::shared_ptr<Widget>& widget){
            widget->onDraw(canvas);
        });
        canvas->flush();
        
        m_drawCanvas->skSurface()->draw(m_mainCanvas->skCanvas(), 0, 0, nullptr);
        m_mainCanvas->skCanvas()->flush();
        
        swapBuffers();
    }
    
    void Window::pollEvents()
    {
        glfwPollEvents();
        handleEvents();
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
        m_mainCanvas->resize(Vec2<uint32_t>(fb_width, fb_height));
        m_drawCanvas->resize(Vec2<uint32_t>(fb_width, fb_height));
        
        PTK_INFO("ResizeEvent: W: {0:d}x{1:d}, FB: {2:d}x{3:d}", width, height, fb_width, fb_height);
    }
    
    // Visible
    void Window::show()
    {
        glfwShowWindow(m_window);
    }
    
    void Window::hide()
    {
        glfwHideWindow(m_window);
    }
    
    // Event
    void Window::sendEvent(Event* event)
    {
        if (event != nullptr)
            m_events.push(event);
    }
    
    void Window::handleEvents()
    {
        while (!m_events.empty())
        {
            Event* event = m_events.front();
            m_events.pop();
            
            if (event->category() == EventCategory::Window)
                handleWindowEvent(event);
            else if (event->category() == EventCategory::Key)
                handleKeyEvent(event);
            else if (event->category() == EventCategory::Mouse)
                handleMouseEvent(event);
            
            delete event;
        }
        
        draw();
    }

    // Event processing
    void Window::handleKeyEvent(Event* t_event)
    {
        KeyEvent* event = (KeyEvent*)t_event;
        if (event->type() == EventType::KeyPressed)
            std::cout << "Key pressed: " << event->get_keycode() << "\n";
        else if (event->type() == EventType::KeyReleased)
            std::cout << "Key released: " << event->get_keycode() << "\n";
    }

    void Window::handleMouseEvent(Event* event)
    {
        EventType type = event->type();
        if (type == EventType::MouseMoved)
        {
            MotionEvent* m_event = (MotionEvent*)event;
            std::shared_ptr<Widget> m_widget = rfind_if(Vec2<float>(m_event->get_posx(), m_event->get_posy()));
        } else if (type == EventType::MouseButtonPressed || type == EventType::MouseButtonReleased)
        {
            ButtonEvent* bEvent = (ButtonEvent*)event;
            Vec2<int> pos(bEvent->get_posx()*m_data.scale.x, bEvent->get_posy()*m_data.scale.y);
            std::shared_ptr<Widget> bWidget = rfind_if(Vec2<float>(pos));
            if (bWidget != nullptr)
            {
                if (bWidget->visible())
                {
                    EventType type = bEvent->type();
                    if (type == EventType::MouseButtonPressed)
                        bWidget->handleClickEvent(bEvent->get_button(), pos);
                    else if (type == EventType::MouseButtonReleased)
                        bWidget->handleReleaseEvent(bEvent->get_button(), pos);
                }
            }
        }
    }

    void Window::handleWindowEvent(Event* event)
    {
        EventType type = event->type();
        if (type == EventType::WindowResize)
        {
            ResizeEvent* rEvent = (ResizeEvent*)event;
            resize(rEvent->get_width(), rEvent->get_height());
        }
    }
}
