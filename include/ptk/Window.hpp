//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/core/Container.hpp"
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
    class Window : public Container
    {
    public:
        Window(const std::string& name, uint width, uint height);
        virtual ~Window();

        // Size
        Vec2u getContentSize() const;
        const Vec2f& getDPIScale() const;
        const Size& getSize() const;
        void setSizeHint(const Size& size) const;
        void setMinSizeHint(const Size& size);
        void setMaxSizeHint(const Size& size);
        const Size& getMinSize() const;
        const Size& getMaxSize() const;
        
        // Close
        bool shouldClose();
        void close();
        
        // Visible
        void show();
        void hide();
        
        // Widget handling
        void add(const std::shared_ptr<Widget>& widget) override;
        
        // Event
        void pollEvents();
        void sendEvent(Event* event);
        void handleEvents();
        
    private:
        // Window
        GLFWwindow* m_window;
        std::string m_name;
        Size m_size;
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
        void handleKeyEvent(Event* event);
        void handleMouseEvent(Event* event);
        void handleWindowEvent(Event* event);
        
        void draw();
        void resize(unsigned int width, unsigned int height);
        void swapBuffers();
    };
}

#endif // PTK_WINDOW_HPP
