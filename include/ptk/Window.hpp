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

// C++ Headers
#include <string>
#include <queue>

// GLFW Headers
#include <GLFW/glfw3.h>

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
        void setSizeHint(const Size& size) override;
        void setMinSizeHint(const Size& size);
        const Size& getMinSize() const;
        void setMaxSizeHint(const Size& size);
        const Size& getMaxSize() const;
        
        // Close
        bool shouldClose();
        void close();
        
        // Visible
        void show();
        void hide();
        
        // Event
        void pollEvents();
        void sendEvent(Event* event);
        void handleEvents();
        
    private:
        // Window
        GLFWwindow* m_window;
        Size m_minSize;
        Size m_maxSize;
        Vec2f m_scale;
        Canvas* m_drawCanvas;
        std::queue<Event*> m_events;

        // Init Functions
        void initGLFW();
        
        // Set Event Callbacks
        void setWindowCallbacks();
        void setMouseCallbacks();
        void setKeyCallbacks();
        
        // Event processing
        void handleKeyboardEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);
        
        void onDraw(SkCanvas* canvas) override;
        bool drawChild(Widget* widget) override;
        
        void resize(unsigned int width, unsigned int height);
        void swapBuffers();
        
        // Hide functions
        void setPosHint(const Position&) override {}
        using Widget::setParent;
        using Widget::getParent;
    };
}

#endif // PTK_WINDOW_HPP
