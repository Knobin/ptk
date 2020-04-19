//
//  core/WindowBackend.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_CORE_WINDOWBACKEND_HPP
#define PTK_CORE_WINDOWBACKEND_HPP

// Local Headers
#include "ptk/util/Point.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/core/ContextBase.hpp"
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
        WindowBackend(BackendType backend)
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

        /** Function for retrieving the backend type of the Window.

            @return     backend type
        */
        BackendType getBackendType() const
        {
            return m_backend;
        }

    private:
        BackendType m_backend;
    };
}

#endif // PTK_CORE_WINDOWBACKEND_HPP
