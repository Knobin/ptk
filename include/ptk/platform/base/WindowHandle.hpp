//
//  platform/base/WindowHandle.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_BASE_WINDOWHANDLE_HPP
#define PTK_PLATFORM_BASE_WINDOWHANDLE_HPP

// pTK Headers
#include "ptk/util/Point.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/platform/base/ContextBase.hpp"
#include "ptk/Core.hpp"
#include "ptk/core/Event.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/widgets/VBox.hpp"

namespace pTK
{
    /** WindowHandle class implementation

        Specifies the backend API that the platform must implement.
    */
    class WindowHandle : public VBox
    {
    public:
        WindowHandle() = default;

        /** Destructor for WindowHandle.

        */
        virtual ~WindowHandle() = default;

        /** Function for showing the window.

        */
        virtual void show() {}

        /** Function for closing the window.

            @return     true if operation is successful, otherwise false
        */
        virtual bool close() = 0;

        /** Function for hiding the window.
        
        */
        virtual void hide() {}

        /** Function for checking if it is visible.

            @return    status
        */
        virtual bool visible() const { return false; };

        /** Function for retrieving if the window is hidden.

            @return     true if window is hidden, otherwise false
        */
        [[nodiscard]] virtual bool isHidden() const = 0;

        /** Function for setting the position of the window.

            @param pos  position to set
            @return     true if operation is successful, otherwise false
        */
        // virtual bool setPosHint(const Point& UNUSED(pos)) = 0;
        void setPosHint(const Point& pos) override {};

        /** Function for resizing the window.

            @param size  size to set
            @return     true if operation is successful, otherwise false
        */
        virtual bool resize(const Size& UNUSED(size)) = 0;

        /** Function for setting the size limits the window.

            @param min  minimal size of the window
            @param max  maximum size of the window
            @return     true if operation is successful, otherwise false
        */
        // virtual bool setLimits(const Size& UNUSED(min), const Size& UNUSED(max)) = 0;

        /** Function for swapping the buffers.

        */
        virtual void swapBuffers() = 0;

        /** Function for retrieving the Context of the Backend.

            @return     Context
        */
        [[nodiscard]] virtual ContextBase *getContext() const = 0;

        /** Function for retrieving the scaling of the Window.

            @return     scaling
        */
        [[nodiscard]] virtual Vec2f getDPIScale() const { return {1.0f, 1.0f}; }

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
            @return         true if operation is successful, otherwise false
        */
        virtual bool setTitle(const std::string& UNUSED(name)) = 0;

        /** Function for setting the icon of the window.

            Note: The function expects the pixels to be in a RGBA format with size of 32 bits (4 bytes).
            Therefore, size of the pixel array is width * height * 32.
            This is why a size parameter is not needed.

            @param width    width of the image
            @param height   height of the image
            @param pixels   image pixels in a RGBA format.
            @return         true if operation is successful, otherwise false
        */
        virtual bool setIcon(int32 UNUSED(width), int32 UNUSED(height), byte* UNUSED(pixels)) = 0;

        /** Function for notifying the backend that an event has been pushed from
            a different thread.

            Depending on the implementation, pollEvents() might need to be exited.
            This function will notify when that is needed.
        */
        virtual void notifyEvent() {};

        /** Function for retrieving the window position.

            @return     Window Position
        */
        [[nodiscard]] virtual Point getWinPos() const = 0;

        /** Function for retrieving the window size.

            @return     Window Size
        */
        [[nodiscard]] virtual Size getWinSize() const = 0;

        /** Function for minimizing the window.

            @return     true if operation is successful, otherwise false
        */
        virtual bool minimize() = 0;

        /** Function for retrieving the minimizing status of the window.

            @return     true if window is minimized, otherwise false
        */
        [[nodiscard]] virtual bool isMinimized() const = 0;

        /** Function for restoring a window from the minimized state.

            @return     true if operation is successful, otherwise false
        */
        virtual bool restore() = 0;

        /** Function for retrieving the focus status of the window.

            @return     true if window is focused, otherwise false
        */
        [[nodiscard]] virtual bool isFocused() const = 0;

        /** Function for hinting the scale of the window.

            @param scale    scale hint
            @return         true if operation is successful, otherwise false
        */
        virtual bool setScaleHint(const Vec2f& UNUSED(scale)) {
            return true;}

    };
}

#endif // PTK_PLATFORM_BASE_WINDOWHANDLE_HPP
