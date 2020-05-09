//
//  core/platform/WindowBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_CORE_PLATFORM_WINDOWBASE_HPP
#define PTK_CORE_PLATFORM_WINDOWBASE_HPP

// Local Headers
#include "ptk/util/Point.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/core/platform/ContextBase.hpp"
#include "ptk/Core.hpp"

namespace pTK
{
    /** Backend enum class implementation

        Specifies which backend that should be used.
    */
    enum class BackendType
    {
        SOFTWARE = 1,
        HARDWARE
    };

    /** WindowBackend class implementation

        Specifies the backend API that the platform must implement.
    */
    class WindowBackend
    {
    public:
        WindowBackend() = delete;

        /** Constructs WindowBackend with default values.

            @param backend  type of backend
            @return         default initialized WindowBackend
        */
        explicit WindowBackend(BackendType backend)
            : m_backend{backend}
        {

        }

        /** Deconstructor for WindowBackend.

        */
        virtual ~WindowBackend() = default;

        /** Function for showing the window.

        */
        virtual void show() = 0;

        /** Function for closing the window.

        */
        virtual void close() = 0;

        /** Function for hiding the window.

        */
        virtual void hide() = 0;

        /** Function for retrieving if the window is hidden.

        */
        virtual bool isHidden() const = 0;

        /** Function for polling the events.

        */
        virtual void pollEvents() = 0;

        /** Function for setting the position of the window.

            @param pos  position to set
        */
        virtual void setPosHint(const Point& UNUSED(pos)) = 0;

        /** Function for resizing the window.

            @param size  size to set
        */
        virtual void resize(const Size& UNUSED(size)) = 0;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
        */
        virtual void setLimits(const Size& UNUSED(min), const Size& UNUSED(max)) = 0;

        /** Function for swapping the buffers.

        */
        virtual void swapBuffers() = 0;

        /** Function for retrieving the Context of the Backend.

            @return     Context
        */
        virtual ContextBase *getContext() const = 0;

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        virtual Vec2f getDPIScale() const { return {1.0f, 1.0f}; }

        /** Function for initiating the drawing.

            This function will be called before any drawing is supposed to be done.
            Note: It is not mandatory to implement this function, every platform may not need it.
        */
        virtual void beginPaint() {}

        /** Function for ending the drawing.

            This function will be called after the drawing is completed.
            Note: It is not mandatory to implement this function, every platform may not need it.
        */
        virtual void endPaint() {}

        /** Function for setting the title of the window.

            @param name     title to show
        */
        virtual void setTitle(const std::string& UNUSED(name)) = 0;

        /** Function for setting the icon of the window.

            Note: The function expects the pixels to be in a RGBA format with size of 32 bits (4 bytes).
            Therefore, size of the pixel array is width * height * 32.
            This is why a size parameter is not needed.

            @param width    width of the image
            @param height   height of the image
            @param pixels   image pixels in a RGBA format.
        */
        virtual void setIcon(int32 UNUSED(width), int32 UNUSED(height), byte* UNUSED(pixels)) = 0;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        virtual void notifyEvent() {};

        /** Function for retrieving the backend type of the Window.

            @return     backend type
        */
        BackendType getBackendType() const
        {
            return m_backend;
        }

        /** Function for retrieving the window position.

            @return     Window Position
        */
        virtual Point getWinPos() const = 0;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        virtual Size getWinSize() const = 0;

        /** Function for minimizing the window.

        */
        virtual void minimize() = 0;

        /** Function for retrieving the minimizing status of the window.

            @return     status
        */
        virtual bool isMinimized() const = 0;

        /** Function for restoring a window from the minimized state.

        */
        virtual void restore() = 0;

        /** Function for retrieving the focus status of the window.

            @return     status
        */
        virtual bool isFocused() const = 0;

        /** Function for hinting the scale of the window.

            @param scale    scale hint
        */
        virtual void setScaleHint(const Vec2f& UNUSED(scale)) {}

    private:
        BackendType m_backend;
    };
}

#endif // PTK_CORE_PLATFORM_WINDOWBASE_HPP
