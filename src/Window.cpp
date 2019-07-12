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
#include "ptk/util/Semaphore.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <exception>
#include <sstream>
#include <iostream>

namespace pTK
{
    Window::Window(const std::string& name, uint width, uint height)
        : VBox(), NonMovable(), NonCopyable(),
            m_window{nullptr}, m_minSize{0.0f, 0.0f}, m_maxSize{0.0f, 0.0f}, m_scale{1.0f, 1.0f},
            m_drawCanvas{nullptr}, m_handleThreadEvents{}, m_mainThreadEvents{}, m_handleThread{},
            m_runThreads{false}, m_mainThreadID{std::this_thread::get_id()}
    {
        // Set Widget properties.
        Widget::setSize({(float)width, (float)height});
        setName(name);
        
        initGLFW();

        // Create Window.
        m_window = glfwCreateWindow((int)width, (int)height, name.c_str(), nullptr, nullptr);
        PTK_ASSERT(m_window, "[Window] Failed to create GLFW Window");
        PTK_INFO("[Window] GLFW Window Created with {0:d}x{1:d}", (int)width, (int)height);
        
        // Get Monitor Scale
        glfwGetWindowContentScale(m_window, &m_scale.x, &m_scale.y);
        PTK_INFO("[Window] Monitor scale is {0:0.2f}x{1:0.2f}", m_scale.x, m_scale.y);
        
        // Bind context.
        glfwMakeContextCurrent(m_window);
        
        // Init Canvas
        Size wSize = getContentSize();
        m_drawCanvas = new Canvas(Size(wSize.width, wSize.height));
        PTK_ASSERT(m_drawCanvas, "[Window] Failed to create Canvas");
        
        // Set Callbacks
        glfwSetWindowUserPointer(m_window, this);
        setWindowCallbacks();
        setMouseCallbacks();
        setKeyCallbacks();
        
        // Start Event handler thread
        Semaphore sema(0);
        m_runThreads = true;
        m_handleThread = std::thread([&](){
            PTK_INFO("[Window] Event Thread Started");
            
            // TODO: Init handleThread if necessary.
            
            // Init finished
            PTK_INFO("[Window] Event Thread Initialized");
            sema.up();
            
            handleThreadEvents();
            
            PTK_INFO("[Window] Event Thread Ended");
        });
        
        // Wait for m_handleThread to init.
        sema.down();
        PTK_INFO("[Window] Initialization finished");
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
            Vec2u windowSize{(uint)t_width, (uint)t_height};
            Size contentSize = window->getContentSize();
            window->sendEvent<ResizeEvent>(windowSize, Vec2u(contentSize.width, contentSize.height));
        });
        
        // void window_close_callback(GLFWwindow* window)
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* t_window){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            window->sendEvent<Event>(EventCategory::Window, EventType::WindowClose);
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
                    window->sendEvent<MotionEvent>((int)t_xpos, (int)t_ypos);
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
                window->sendEvent<ButtonEvent>(EventType::MouseButtonPressed, button, xpos, ypos);
            else if (t_action == GLFW_RELEASE)
                window->sendEvent<ButtonEvent>(EventType::MouseButtonReleased, button, xpos, ypos);
        });
    }
    
    void Window::setKeyCallbacks()
    {
        // void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        glfwSetKeyCallback(m_window, [](GLFWwindow* t_window, int t_key, int, int t_action, int){
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));
            if (t_action == GLFW_PRESS)
                window->sendEvent<KeyEvent>(EventType::KeyPressed, t_key);
            else if (t_action == GLFW_RELEASE)
                window->sendEvent<KeyEvent>(EventType::KeyReleased, t_key);
        });
    }

    Window::~Window()
    {
        m_handleThread.join();
        PTK_INFO("[Window] Event Thread joined");
        delete m_drawCanvas;
        glfwTerminate();
        PTK_INFO("[Window] Destructed");
    }
    
    void Window::onDraw(SkCanvas* canvas)
    {
        Color color = getBackground();
        m_drawCanvas->clear(color);
        SkMatrix matrix;
        matrix.setScale(m_scale.x, m_scale.y);
        canvas->setMatrix(matrix);
        drawWidgets(canvas);
        canvas->flush();
        swapBuffers();
    }
    
    bool Window::drawChild(Widget* widget)
    {
        if (find(widget) != cend())
        {
            sendEvent<Event>(EventCategory::Window, EventType::WindowDraw);
            return true;
        }
        
        return false;
    }
    
    void Window::pollEvents()
    {
        glfwWaitEvents();
        
        uint eventCount = m_mainThreadEvents.size();
        bool drawEventHandled = false;
        for (uint i = 0; i < eventCount; i++)
        {
            std::shared_ptr<Event> wEvent = m_mainThreadEvents.front();
            m_mainThreadEvents.pop();
            
            if (!drawEventHandled || wEvent->type() != EventType::WindowDraw)
                handleMainThreadEvents(wEvent.get());
            
            if (wEvent->type() == EventType::WindowDraw)
                drawEventHandled = true;
        }
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }
    
    // Size
    Size Window::getContentSize() const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        
        return Size(width, height);
    }
    
    const Vec2f& Window::getDPIScale() const
    {
        return m_scale;
    }
    
    void Window::setSize(const Size& size)
    {
        // TODO: Some checking in layout to confirm if size change
        // is doable.
        glfwSetWindowSize(m_window, size.width, size.height);
        Widget::setSize(size);
    }
    
    void Window::setMinSize(const Size& size)
    {
        // TODO: Some checking in layout to confirm if min size
        // is doable. If not, set the lowest size doable instead.
        m_minSize = size;
    }
    
    void Window::setMaxSize(const Size& size)
    {
        // TODO: Some checking in layout to confirm if max size
        // is doable. If not, set the highest size doable instead.
        m_maxSize = size;
    }

    // Close
    bool Window::shouldClose()
    {
        return (bool)glfwWindowShouldClose(m_window);
    }
    
    void Window::close()
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        sendEvent<Event>(EventCategory::Window, EventType::WindowClose);
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
    void Window::handleMainThreadEvents(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        if (event->category() == EventCategory::Window)
        {
            if (event->type() == EventType::WindowDraw)
            {
                onDraw(m_drawCanvas->skCanvas());
            }else if (event->type() == EventType::WindowResize)
            {
                ResizeEvent* rEvent = (ResizeEvent*)event;
                Vec2u cSize = rEvent->getContentSize();
                
                // Set Framebuffer Size.
                Size fbSize{static_cast<float>(cSize.x), static_cast<float>(cSize.y)};
                if (fbSize != m_drawCanvas->getSize())
                    m_drawCanvas->resize(fbSize);
                
                // Send draw event.
                sendEvent<Event>(EventCategory::Window, EventType::WindowDraw);
            }
        }
    }
    
    void Window::handleThreadEvents()
    {
        while (m_runThreads)
        {
            std::shared_ptr<Event> event = m_handleThreadEvents.front();
            m_handleThreadEvents.pop();
            if (event->category() == EventCategory::Window)
                handleWindowEvent(event.get());
            else if (event->category() == EventCategory::Key)
                handleKeyboardEvent(event.get());
            else if (event->category() == EventCategory::Mouse)
                handleMouseEvent(event.get());
        }
    }

    // Event processing
    void Window::handleKeyboardEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        KeyEvent* kEvent = (KeyEvent*)event;
        handleKeyEvent(kEvent->type(), kEvent->get_keycode());
    }

    void Window::handleMouseEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        EventType type = event->type();
        if (type == EventType::MouseMoved)
        {
            MotionEvent* mEvent = (MotionEvent*)event;
            handleHoverEvent(mEvent->getPos());
        } else if (type == EventType::MouseButtonPressed || type == EventType::MouseButtonReleased)
        {
            ButtonEvent* bEvent = (ButtonEvent*)event;
            Position pos{bEvent->getPos()};
            MouseButton btn = bEvent->getButton();
            if (type == EventType::MouseButtonPressed)
                handleClickEvent(btn, pos);
            else if (type == EventType::MouseButtonReleased)
                handleReleaseEvent(btn, pos);
        }
    }

    void Window::handleWindowEvent(Event* event)
    {
        PTK_ASSERT(event, "Undefined Event");
        EventType type = event->type();
        if (type == EventType::WindowDraw)
        {
            ; // TODO: ?
        }
        else if (type == EventType::WindowResize)
        {
            ResizeEvent* rEvent = (ResizeEvent*)event;
            Vec2u size = rEvent->getSize();
            VBox::setSize(Size(size.x, size.y));
        }else if (type == EventType::WindowClose)
        {
            m_runThreads = false;
        }
    }
}
