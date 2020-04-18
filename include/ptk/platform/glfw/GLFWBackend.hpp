//
//  platform/GLFWWindow.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_GLFWWINDOW_HPP
#define PTK_GLFWWINDOW_HPP

// Local Headers
#include "ptk/core/WindowBackend.hpp"
#include "ptk/platform/glfw/GLContext.hpp"

// GLFW Headers
#include <GLFW/glfw3.h>

namespace pTK
{
    class Window;

    /** GLFWBackend class implementation.

    */
    class GLFWBackend : public WindowBackend
    {
    public:
        /** Window Button with default values with name, width and height.

            Note: Window width or height cant be 0, if either one of them is 0, it will set
            that value to 1.

            @param name    name of the window
            @param width   width of the window
            @param height  height of the window
            @return        default initialized Window
        */
        GLFWBackend(Window *window, const std::string& name, const Size& size, Backend backend);
        virtual ~GLFWBackend();

        /** Function for closing the window.

        */
        void close() override;

        /** Function for showing the window.

        */
        void show() override;

        /** Function for hiding the window.

        */
        void hide() override;

        /** Function for polling the window events.

        */
        void pollEvents() override;

        /** Function for setting the window position.

        */
        void setPosHint(const Point& pos) override;

        ContextBase *getContext() const override;

        void swapBuffers() override;

        Vec2f getDPIScale() const override;

        void resize(const Size& size) override;
        void setLimits(const Size& min, const Size& max) override;

    private:
        // Window
        GLFWwindow* m_window;
        std::unique_ptr<GLContext> m_drawCanvas;
        Window *m_parentWindow;
        Vec2f m_scale;

        // Init Functions
        void initGLFW();

        // Set Event Callbacks
        void setWindowCallbacks();
        void setMouseCallbacks();
        void setKeyCallbacks();

        void setLimits2(const Size& minSize, const Size& maxSize);
    };
}

#endif // PTK_GLFWWINDOW_HPP
