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
    Window::Window(const std::string& name, uint width, uint height)
        : Container(), NonMovable(), NonCopyable(), m_window{nullptr}, m_name{name}, m_size{(float)width, (float)height},
            m_minSize{0.0f, 0.0f}, m_maxSize{0.0f, 0.0f}, m_scale{1.0f, 1.0f}, m_drawCanvas{nullptr}, m_events{}
    {
        initGLFW();

        // Create Window.
        m_window = glfwCreateWindow((int)width, (int)height, name.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create GLFW Window.");
        }
        
        PTK_INFO("[Window] Created with {0:d}x{1:d}", (int)width, (int)height);
        
        // Get Monitor Scale
        glfwGetWindowContentScale(m_window, &m_scale.x, &m_scale.y);
        PTK_INFO("[Window] Monitor scale is {0:f}x{1:f}", m_scale.x, m_scale.y);
        
        // Bind context.
        glfwMakeContextCurrent(m_window);

        // Init Canvas
        Vec2u fbSize = getContentSize();
        m_drawCanvas = new Canvas(fbSize);
        if (m_drawCanvas == nullptr)
        {
            glfwTerminate();
            throw std::logic_error("Failed to create Canvas.");
        }
        m_drawCanvas->setDPIScale(m_scale);
        
        // Set pointer for use in callbacks;
        glfwSetWindowUserPointer(m_window, this);
        
        setWindowCallbacks();
        setMouseCallbacks();
        setKeyCallbacks();
    }
    
    // Init Functions
    void Window::initGLFW()
    {
        // Initialize and configure of GLFW.
        glfwInit();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hide window on creation.
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
        
        // void window_close_callback(GLFWwindow* window)
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* t_window){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->sendEvent(new Event(EventCategory::Window, EventType::WindowClose));
        });
    }
    
    void Window::setMouseCallbacks()
    {
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* t_window, double t_xpos, double t_ypos){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            Size wSize = window->getSize();
            
            if ((t_xpos >= 0) && (t_xpos <= (wSize.width)))
                if ((t_ypos >= 0) && (t_ypos <= (wSize.height)))
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
                window->sendEvent(new ButtonEvent(EventType::MouseButtonPressed, button, xpos, ypos));
            else if (t_action == GLFW_RELEASE)
                window->sendEvent(new ButtonEvent(EventType::MouseButtonReleased, button, xpos, ypos));
            
            PTK_INFO("[Window] Cursor {0} at {1:d}x{2:d}", (t_action == GLFW_PRESS) ? "click" : "release", (int)xpos, (int)ypos);
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
        glfwTerminate();
    }

    void Window::draw()
    {
        m_drawCanvas->clear();
        
        for_each([&](const std::shared_ptr<Widget>& widget){
            widget->onDraw(*m_drawCanvas);
        });
        m_drawCanvas->skCanvas()->flush();
        
        swapBuffers();
    }
    
    void Window::pollEvents()
    {
        if (m_events.empty())
        {
            // GLFW will wait until event is sent.
            glfwWaitEvents();
        }else
        {
            // We already have events in queue,
            // poll GLFW events and continue.
            glfwPollEvents();
        }
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }
    
    // Size
    Vec2u Window::getContentSize() const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        
        return Vec2u(width, height);
    }
    
    const Vec2f& Window::getDPIScale() const
    {
        return m_scale;
    }
    
    const Size& Window::getSize() const
    {
        return m_size;
    }
    
    void Window::setSizeHint(const Size& size) const
    {
        // TODO: Some checking in layout to confirm if size change
        // is doable.
        glfwSetWindowSize(m_window, size.width, size.height);
    }
    
    void Window::setMinSizeHint(const Size& size)
    {
        // TODO: Some checking in layout to confirm if min size
        // is doable. If not, set the lowest size doable instead.
        m_minSize = size;
    }
    
    void Window::setMaxSizeHint(const Size& size)
    {
        // TODO: Some checking in layout to confirm if max size
        // is doable. If not, set the highest size doable instead.
        m_maxSize = size;
    }
    
    const Size& Window::getMinSize() const
    {
        return m_minSize;
    }
    
    const Size& Window::getMaxSize() const
    {
        return m_maxSize;
    }

    // Close
    bool Window::shouldClose()
    {
        return (bool)glfwWindowShouldClose(m_window);
    }
    
    /** This function will let GLFW know that we want to close the window.
        GLFW will then call the user definable callback window_close_callback().
        This function will send a WindowClose Event that can be handled in
        handleWindowEvent(). The event is not needed to catch or handle. just
        a way to potentially cleanup before the destructor is called.
     */
    void Window::close()
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    void Window::resize(unsigned int width, unsigned int height)
    {
        // Set Window Size.
        m_size.width = width;
        m_size.height = height;
        
        // Set Framebuffer Size.
        Vec2u fbSize = getContentSize();
        if (fbSize != m_drawCanvas->getSize())
            m_drawCanvas->resize(fbSize);
        
        // TODO: Resize the widgets
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
    
    // Widget handling
    void Window::add(const std::shared_ptr<Widget>& widget)
    {
        // This will be replaced in the future.
        // Currently just for testing.
        if (insert_widget(widget))
        {
            uint width = 0;
            uint height = 0;
            for_each([&width, &height](const std::shared_ptr<Widget>& widget){
                height += widget->getSize().height;
                width = (width < widget->getSize().width) ? widget->getSize().width : width;
            });
            widget->setPosHint({0, height - widget->getSize().height});
            PTK_INFO("[Window] resized to {0:d}x{1:d}", width, height);
            setSizeHint(Size(width, height));
        }
    }
    
    // Event
    void Window::sendEvent(Event* event)
    {
        if (event != nullptr)
            m_events.push(event);
    }
    
    /** User events can push more events to the queue when
     handling current events and make the queue larger.
     Therefore, we only handle the current events in the
     queue and handle events pushed in the loop in the
     next time we handle events.
     By doing this, we will avoid processing events forever,
     if the user accidentally pushes events in a non ending
     loop or creates a non ending loop.
     */
    void Window::handleEvents()
    {
        uint eventCount = m_events.size();
        
        // Until drawing is fully set up, we send a draw event.
        if (eventCount != 0)
        {
            eventCount++;
            sendEvent(new Event(EventCategory::Window, EventType::WindowDraw));
        }
        
        for (uint i{0}; i < eventCount; i++)
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
    }

    // Event processing
    void Window::handleKeyEvent(Event* t_event)
    {
        KeyEvent* event = (KeyEvent*)t_event;
        PTK_INFO("Key {0}: {1:d}", (event->type() == EventType::KeyPressed) ? "pressed" : "released",event->get_keycode());
    }

    void Window::handleMouseEvent(Event* event)
    {
        EventType type = event->type();
        if (type == EventType::MouseMoved)
        {
            ; // MotionEvent* m_event = (MotionEvent*)event;
            // TODO: Send MotionEvent to Layout
        } else if (type == EventType::MouseButtonPressed || type == EventType::MouseButtonReleased)
        {
            ButtonEvent* bEvent = (ButtonEvent*)event;
            Position clickPos{bEvent->getPos()};
            auto bWidget = rfind_if(clickPos);
            if (bWidget != nullptr)
            {
                EventType type = bEvent->type();
                if (type == EventType::MouseButtonPressed)
                    bWidget->handleClickEvent(bEvent->getButton(), clickPos);
                else if (type == EventType::MouseButtonReleased)
                    bWidget->handleReleaseEvent(bEvent->getButton(), clickPos);
            }
        }
    }

    void Window::handleWindowEvent(Event* event)
    {
        EventType type = event->type();
        if (type == EventType::WindowDraw)
        {
            //PTK_INFO("[Window] Draw");
            draw();
        }
        else if (type == EventType::WindowResize)
        {
            ResizeEvent* rEvent = (ResizeEvent*)event;
            resize(rEvent->getWidth(), rEvent->getHeight());
        }
    }
}
