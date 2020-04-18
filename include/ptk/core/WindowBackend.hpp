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

namespace pTK
{
    enum class Backend
    {
        NONE = 0,
        SOFTWARE,
        HARDWARE
    };

    class WindowBackend
    {
    public:
        WindowBackend() = delete;
        WindowBackend(Backend backend)
            : m_backend{backend}
        {

        }
        virtual ~WindowBackend() = default;

        // API
        virtual void show() = 0;
        virtual void close() = 0;
        virtual void hide() = 0;
        virtual void pollEvents() = 0;
        virtual void setPosHint(const Point&) = 0;
        virtual void resize(const Size&) = 0;
        virtual void setLimits(const Size&, const Size&) = 0;
        virtual void swapBuffers() = 0;
        virtual ContextBase *getContext() const = 0;
        virtual Vec2f getDPIScale() const { return {1.0f, 1.0f}; }
        virtual void beginPaint() {}
        virtual void endPaint() {}

        Backend getBackendType() const
        {
            return m_backend;
        }

    private:
        Backend m_backend;
    };
}

#endif // PTK_CORE_WINDOWBACKEND_HPP
