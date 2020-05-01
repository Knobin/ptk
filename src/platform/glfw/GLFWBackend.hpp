//
//  platform/glfw/GLFWWindow.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-12.
//

#ifndef PTK_PlATFORM_GLFW_GLFWBACKEND_HPP
#define PTK_PlATFORM_GLFW_GLFWBACKEND_HPP

// Local Headers
#include "ptk/core/platform/WindowBackend.hpp"
#include "GLContext.hpp"

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
        GLFWBackend(Window *window, const std::string& name, const Size& size, BackendType backend);
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

             @param pos  requested position of the Window.
         */
        void setPosHint(const Point& pos) override;

        /** Function for retrieving the Context.

            @return context
        */
        ContextBase *getContext() const override;

        /** Function for swapping the buffers.

        */
        void swapBuffers() override;

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        Vec2f getDPIScale() const override;

        /** Function for resizing the window.

            @param size  size to set
        */
        void resize(const Size& size) override;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        void setLimits(const Size& min, const Size& max) override;

        /** Function for setting the title of the window.

            @param name     title to show
        */
        void setTitle(const std::string& name) override;

        /** Function for setting the icon of the window.

            Note: The function expects the pixels to be in a RGBA format with size of 32 bits (4 bytes).
            Therefore, size of the pixel array is width * height * 32.
            This is why a size parameter is not needed.

            @param width    width of the image
            @param height   height of the image
            @param pixels   image pixels in a RGBA format.
        */
        void setIcon(int32 width, int32 height, byte* pixels) override;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        void notifyEvent() override;

        /** Function for retrieving the window position.

            @return     Window Position
        */
        Point getWinPos() const override;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        Size getWinSize() const override;

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

#endif // PTK_PlATFORM_GLFW_GLFWBACKEND_HPP
