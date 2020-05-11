//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

// Local Headers
#include "GLFWBackend.hpp"
#include "ptk/Window.hpp"
#include "ptk/events/KeyEvent.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WindowEvent.hpp"
#include "ptk/util/Semaphore.hpp"
#include "ptk/Core.hpp"

namespace pTK
{

    struct GLFWBackendData
    {
        Window *window;
        Size size;
        Point pos;
        bool hidden;
    };

    GLFWBackend::GLFWBackend(Window *window, const std::string& name, const Size& size, BackendType backend)
        : WindowBackend(backend),
          m_window{nullptr}, m_drawCanvas{nullptr}, m_parentWindow{window}, m_scale{1.0f, 1.0f}
    {
        initGLFW();

        // Create Window.
        m_window = glfwCreateWindow(static_cast<int>(size.width), static_cast<int>(size.height), name.c_str(), nullptr, nullptr);
        PTK_ASSERT(m_window, "Failed to create GLFW Window");
        PTK_INFO("GLFW Window Created, {0:d}x{1:d}", static_cast<int>(size.width), static_cast<int>(size.height));

        // Get position
        Point pos{};
        glfwGetWindowPos(m_window, &pos.x, &pos.y);

        // Get Monitor Scale
        glfwGetWindowContentScale(m_window, &m_scale.x, &m_scale.y);
        PTK_INFO("Monitor scale, {0:0.2f}x{1:0.2f}", m_scale.x, m_scale.y);

        // Bind context.
        glfwMakeContextCurrent(m_window);

        // Init Canvas
        Size wSize{};
        wSize.width = static_cast<Size::value_type>(size.width * m_scale.x);
        wSize.height = static_cast<Size::value_type>(size.height * m_scale.y);
        m_drawCanvas = std::make_unique<GLContext>(Size(wSize.width, wSize.height));
        PTK_ASSERT(m_drawCanvas, "Failed to create Canvas");

        // Set Callbacks
        m_data = std::make_unique<GLFWBackendData>(GLFWBackendData{window, size, pos, false});
        glfwSetWindowUserPointer(m_window, m_data.get());
        setWindowCallbacks();
        setMouseCallbacks();
        setKeyCallbacks();

        PTK_INFO("Window Initialization finished");
    }

    // Init Functions
    void GLFWBackend::initGLFW()
    {
        // Initialize and configure of GLFW.
        glfwInit();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hide window on creation.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 0);
		glfwWindowHint(GLFW_SAMPLES, 4);
    }

    // Set Event Callbacks
    void GLFWBackend::setWindowCallbacks()
    {
        // void window_size_callback(GLFWwindow* window, int width, int height)
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            ResizeEvent evt{{static_cast<Size::value_type>(width), static_cast<Size::value_type>(height)}};
            data->window->sendEvent(&evt);
            data->window->forceDrawAll();
        });

        // void window_iconify_callback(GLFWwindow* window, int iconify)
        glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int iconify){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            Event::Type type{(iconify) ? Event::Type::WindowMinimize: Event::Type::WindowRestore};
            Event evt{Event::Category::Window, type};
            data->window->sendEvent(&evt);
        });

        // void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
        glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xpos, int ypos){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            Point pos{xpos, ypos};
            if (pos != data->pos)
            {
                MoveEvent evt{pos};
                data->window->sendEvent(&evt);
            }
        });

        // void window_close_callback(GLFWwindow* window)
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            data->window->postEvent<Event>(Event::Category::Window, Event::Type::WindowClose);
        });

        // void window_focus_callback(GLFWwindow* window, int focused)
        glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            Event::Type type{(focused) ? Event::Type::WindowFocus : Event::Type::WindowLostFocus};
            Event evt{Event::Category::Window, type};
            data->window->sendEvent(&evt);
        });

        // void window_maximize_callback(GLFWwindow* window, int maximized)
        glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            int width{0}, height{0};
            glfwGetWindowSize(window, &width, &height);
            data->window->postEvent<ResizeEvent>(Size{static_cast<Size::value_type>(width), static_cast<Size::value_type>(height)});
        });
    }

    void GLFWBackend::setMouseCallbacks()
    {
        // void cursor_enter_callback(GLFWwindow* window, int entered)
        glfwSetCursorEnterCallback(m_window,[](GLFWwindow* window, int entered){
            if (!entered)
            {
                auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
                MotionEvent event{{-1, -1}};
                data->window->sendEvent(&event);
            }
        });

        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            Size wSize{data->size};

            if ((xpos >= 0) && (xpos <= (wSize.width)))
            {
                if ((ypos >= 0) && (ypos <= (wSize.height)))
                {
                    MotionEvent event{{static_cast<Point::value_type>(xpos),
                                       static_cast<Point::value_type>(ypos)}};
                    data->window->sendEvent(&event);
                }
            }


        });
        // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            Mouse::Button btn;
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                btn = Mouse::Button::Left;
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
                btn = Mouse::Button::Middle;
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
                btn = Mouse::Button::Right;
            else
                btn = Mouse::Button::NONE;

            if (action == GLFW_PRESS)
            {
                ButtonEvent event{Event::Type::MouseButtonPressed,
                                  btn,
                                  {static_cast<Point::value_type>(xpos),
                                   static_cast<Point::value_type>(ypos)}};
                data->window->sendEvent(&event);
            }
            else if (action == GLFW_RELEASE)
            {
                ButtonEvent event{Event::Type::MouseButtonReleased,
                                  btn,
                                  {static_cast<Point::value_type>(xpos),
                                   static_cast<Point::value_type>(ypos)}};
                data->window->sendEvent(&event);
            }
        });

        // void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            Vec2f v{static_cast<float>(xoffset), static_cast<float>(yoffset)};
            ScrollEvent event{v};
            data->window->sendEvent(&event);
        });
    }

    void GLFWBackend::setKeyCallbacks()
    {
        // void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int, int action, int){
            auto data{static_cast<GLFWBackendData*>(glfwGetWindowUserPointer(window))};
            if (action == GLFW_PRESS)
            {
                KeyEvent event{KeyEvent::Pressed, key};
                data->window->sendEvent(&event);
            }
            else if (action == GLFW_RELEASE)
            {
                KeyEvent event{KeyEvent::Released, key};
                data->window->sendEvent(&event);
            }
        });
    }

    GLFWBackend::~GLFWBackend()
    {
		// Free canvas before glfw to avoid OpenGL errors.
		m_drawCanvas.reset(nullptr);
        glfwDestroyWindow(m_window);
		glfwTerminate();
        PTK_INFO("Window Destroyed");
    }

    bool GLFWBackend::setPosHint(const Point& pos)
    {
        if (pos != m_data->pos)
        {
            m_data->pos = pos;
            glfwSetWindowPos(m_window, pos.x, pos.y);
            return true;
        }

        return false;
    }

    void GLFWBackend::pollEvents()
    {
        glfwWaitEvents();
    }

    void GLFWBackend::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    bool GLFWBackend::resize(const Size& size)
    {
        if (size != m_data->size)
        {
            m_data->size = size;
            const Size scaledSize{static_cast<Size::value_type>(std::ceil(size.width * m_scale.x)),
                                  static_cast<Size::value_type>(std::ceil(size.height * m_scale.y))};
            if (scaledSize != m_drawCanvas->getSize())
                m_drawCanvas->resize(scaledSize);
            return true;
        }

        return false;
    }

    bool GLFWBackend::setLimits(const Size& min, const Size& max)
    {
        if (m_window)
        {
            int width{(max.width == Size::Limits::Max) ? GLFW_DONT_CARE : max.width};
            int height{(max.height == Size::Limits::Max) ? GLFW_DONT_CARE : max.height};
            glfwSetWindowSizeLimits(m_window, static_cast<int>(min.width), static_cast<int>(min.height), width, height);
            return true;
        }

        return false;
    }

    bool GLFWBackend::close()
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        m_parentWindow->postEvent<Event>(Event::Category::Window, Event::Type::WindowClose);
        return true;
    }

    // Visible
    bool GLFWBackend::show()
    {
        glfwShowWindow(m_window);
        m_parentWindow->forceDrawAll();
        return true;
    }

    bool GLFWBackend::hide()
    {
        glfwHideWindow(m_window);
        return true;
    }

    bool GLFWBackend::isHidden() const
    {
        return static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_VISIBLE));
    }

    ContextBase *GLFWBackend::getContext() const
    {
        return m_drawCanvas.get();
    }

    Vec2f GLFWBackend::getDPIScale() const
    {
        return m_scale;
    }

    bool GLFWBackend::setTitle(const std::string& name)
    {
        glfwSetWindowTitle(m_window, name.c_str());
        return true;
    }

    bool GLFWBackend::setIcon(int32 width, int32 height, byte* pixels)
    {
        GLFWimage images[1];
        images[0] = {width, height, pixels};
        glfwSetWindowIcon(m_window, 1, images);
        return true;
    }

    void GLFWBackend::notifyEvent()
    {
        glfwPostEmptyEvent();
    }

    Point GLFWBackend::getWinPos() const
    {
        return m_data->pos;
    }

    Size GLFWBackend::getWinSize() const
    {
        Size size{};
        glfwGetWindowSize(m_window, &size.width, &size.height);
        return size;
    }

    bool GLFWBackend::minimize()
    {
        glfwIconifyWindow(m_window);
        return true;
    }

    bool GLFWBackend::isMinimized() const
    {
        return static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_ICONIFIED));
    }

    bool GLFWBackend::restore()
    {
        glfwRestoreWindow(m_window);
        return true;
    }

    bool GLFWBackend::isFocused() const
    {
        return static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_FOCUSED));
    }
}
