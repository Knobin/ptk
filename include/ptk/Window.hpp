//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/util/Singleton.hpp"
#include "ptk/widgets/VBox.hpp"
#include "ptk/core/Canvas.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/SafeQueue.hpp"

// C++ Headers
#include <string>

// GLFW Headers
#include <GLFW/glfw3.h>

namespace pTK
{
    /** Window class implementation.
     
     Implements the Window and is also responsible 
     for bridging the events to the Widgets.

     */
    class Window : public VBox, public Singleton
    {
    public:
        /** Default Window consturctor is disabled.
        
         */
        Window() = delete;

        /** Window Button with default values with name, width and height.
         
         Note: Window width or height cant be 0, if either one of them is 0, it will set
         that value to 1.

         @param name    name of the window
         @param width   width of the window
         @param height  height of the window
         @return        default initialized Window
         */
        Window(const std::string& name, uint width, uint height);
        virtual ~Window();

        /** Function for retrieving the content size.
         
         If the monitor supports high DPI, the content of
         the window will size*DPIScale.
         
         If high DPI is not supported this will return the
         window size.

         @return    content size
         */
        Size getContentSize() const;

        /** Function for retrieving the DPI Scale of the window.

         @return    DPI scale
         */
        const Vec2f& getDPIScale() const;

        /** Function for setting the size of the window.

         @param size    width and height to set
         */
        void setSize(const Size& size) override;

        /** Function for setting the minimum size of the window.

         @param size    minimum width and height to set
         */
        void setMinSize(const Size& size) override;

        /** Function for setting the maximum size of the window.

         @param size    maximum width and height to set
         */
        void setMaxSize(const Size& size) override;
        
        /** Function for checking if the window should close.

         @return     close status
         */
        bool shouldClose();

        /** Function for closing the window.
         
         */
        void close();
        
        /** Function for showing the window.

         */
        void show() override;

        /** Function for hiding the window.

         */
        void hide() override;
        
        /** Function for polling the window events.

         */
        void pollEvents();
        
        /** Function for handling the window events.
         
         */
        void handleEvents();
        
        /** Templated function for sending events to the window to
         be handled.

         */
        template<typename T, typename... Args>
        void sendEvent(Args&& ...args)
        {
            Ref<T> event = create<T>(std::forward<Args>(args)...);
            m_eventQueue.push(event);
        }
        
    private:
        // Window
        GLFWwindow* m_window;
        Vec2f m_scale;
        std::unique_ptr<Canvas> m_drawCanvas;
        std::queue<Ref<Event>> m_eventQueue;
        bool m_draw;

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

        void forceDrawAll();
        void onChildDraw(uint) override;
        
        void swapBuffers();
        void setLimits(const Size& minSize, const Size& maxSize);
        
        // Hide functions
        void setPosHint(const Point&) override {}
        using Widget::setParent;
        using Widget::getParent;
    };
}

#endif // PTK_WINDOW_HPP
