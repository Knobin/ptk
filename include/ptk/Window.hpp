//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/util/NonMovable.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/widgets/VBox.hpp"
#include "ptk/core/Canvas.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/SafeQueue.hpp"

// C++ Headers
#include <string>
#include <thread>
#include <atomic>

// GLFW Headers
#include <GLFW/glfw3.h>

#include "ptk/Log.hpp"

namespace pTK
{
    class Window : public VBox, public NonMovable, public NonCopyable
    {
    public:
        Window() = delete;
        Window(const std::string& name, uint width, uint height);
        virtual ~Window();

        // Size
        Size getContentSize() const;
        const Vec2f& getDPIScale() const;
        void setSize(const Size& size) override;
        void setMinSize(const Size& size) override;
        void setMaxSize(const Size& size) override;
        
        // Close
        bool shouldClose();
        void close();
        
        // Visible
        void show();
        void hide();
        
        // Event
        void pollEvents();
        
        template<typename T, typename... Args>
        void sendEvent(Args&& ...args)
        {
            std::shared_ptr<T> event = std::make_shared<T>(std::forward<Args>(args)...);
            if (event->category() == EventCategory::Window)
            {
                if (std::this_thread::get_id() == m_mainThreadID)
                {
                    handleMainThreadEvents(event.get());
                }else
                {
                    m_mainThreadEvents.push(event);
                    glfwPostEmptyEvent();
                }
            }
            
            // Push event to other thread.
            m_handleThreadEvents.push(event);
        }
        
    private:
        // Window
        GLFWwindow* m_window;
        Vec2f m_scale;
        std::unique_ptr<Canvas> m_drawCanvas;
        SafeQueue<std::shared_ptr<Event>> m_handleThreadEvents;
        SafeQueue<std::shared_ptr<Event>> m_mainThreadEvents;
        std::thread m_handleThread;
        std::atomic<bool> m_runThreads;
        std::thread::id m_mainThreadID;

        // Init Functions
        void initGLFW();
        
        // Set Event Callbacks
        void setWindowCallbacks();
        void setMouseCallbacks();
        void setKeyCallbacks();
        
        // Event processing
        void handleMainThreadEvents(Event* event);
        void handleThreadEvents();
        void handleKeyboardEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);
        
        void onDraw(SkCanvas* canvas) override;
        bool drawChild(Widget* widget) override;
        
        void swapBuffers();
        void setLimits(const Size& minSize, const Size& maxSize);
        
        // Hide functions
        void setPosHint(const Position&) override {}
        using Widget::setParent;
        using Widget::getParent;
    };
}

#endif // PTK_WINDOW_HPP
