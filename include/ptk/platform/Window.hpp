//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

// Local Headers
#include "ptk/core/WindowBase.hpp"
#include "ptk/platform/GLCanvas.hpp"

// GLFW Headers
#include <GLFW/glfw3.h>

namespace pTK
{
    /** Window class implementation.

        Implements the Window and is also responsible
        for bridging the events to the Widgets.

    */
    class GLFWWindow : public WindowBase
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
        GLFWWindow(const std::string& name, const Vec2u& size);
        virtual ~GLFWWindow();

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

        /** Function for forcing the window to redraw everything.

        */
        void forceDrawAll() override;

        /** Function for setting the window position.

        */
        void setPosHint(const Point& pos) override;

    private:
        void onResize(const Size& size) override;
        void onLimitChange(const Size& min, const Size& max) override;

    private:
        // Window
        GLFWwindow* m_window;
        std::unique_ptr<GLCanvas> m_drawCanvas;

        // Init Functions
        void initGLFW();

        // Set Event Callbacks
        void setWindowCallbacks();
        void setMouseCallbacks();
        void setKeyCallbacks();

        void swapBuffers();
        void setLimits2(const Size& minSize, const Size& maxSize);
    };
}

#endif // PTK_WINDOW_HPP
