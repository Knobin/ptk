//
//  platform/unix/MainWindow_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "MainWindow_unix.hpp"
#include "ptk/Window.hpp"
#include "Application_unix.hpp"
#include "ptk/core/Exception.hpp"
#include "../common/RasterContext.hpp"
#include "RasterPolicy_unix.hpp"

// X11 Headers
#include <X11/Xutil.h>

namespace pTK
{
    MainWindow_unix::MainWindow_unix(Window *window, const std::string& name, const Size& size, BackendType backend)
        : MainWindowBase(window, backend), m_display{Application_unix::getDisplay()}
    {
        m_size = size;
        int root = DefaultRootWindow(m_display);
        int defaultScreen = DefaultScreen(m_display);
        int screenBitDepth{24};
        XVisualInfo info{};

        if (!XMatchVisualInfo(m_display, defaultScreen, screenBitDepth, TrueColor, &info))
            throw WindowError("No matching visual info");

        const unsigned long eventMask = ExposureMask | StructureNotifyMask |
                                        KeyPressMask | KeyReleaseMask |
                                        PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

        XSetWindowAttributes attr{};
        attr.event_mask = eventMask;
        attr.colormap = XCreateColormap(m_display, root, info.visual, AllocNone);
        const unsigned long attrMask = CWEventMask | CWColormap;
        
        const unsigned int width{static_cast<unsigned int>(size.width)};
        const unsigned int height{static_cast<unsigned int>(size.height)};
        m_window = XCreateWindow(m_display, root, 0, 0, width, height, 0, 24, InputOutput,info.visual, attrMask, &attr);

        if (!m_window)
            throw WindowError("Failed to create Window");

        setTitle(name);

        XMapWindow(m_display, m_window);
        XSaveContext(m_display, m_window, Application_unix::getXContext(), reinterpret_cast<XPointer>(window));
        XFlush(m_display);

        m_atomWmDeleteWindow = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
        if (!XSetWMProtocols(m_display, m_window, &m_atomWmDeleteWindow, 1))
        {
            PTK_WARN("Could not register WM_DELETE_WINDOW property");   
        }

        RasterPolicy_unix policy{m_display, m_window, info};
        m_context = std::make_unique<RasterContext<RasterPolicy_unix>>(size, policy);
    }

    bool MainWindow_unix::close() 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::show() 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::hide() 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::isHidden() const 
    {
        // TODO
        return false;
    }

    bool MainWindow_unix::setPosHint(const Point& pos) 
    {
        // TODO
        return true;
    }

    ContextBase *MainWindow_unix::getContext() const 
    {
        return m_context.get();
    }

    void MainWindow_unix::swapBuffers() 
    {
        /* sk_sp<SkImage> skimage = m_surface->makeImageSnapshot();
        sk_sp<SkData> filedata = skimage->encodeToData(SkEncodedImageFormat::kPNG, 80);
        std::ofstream file;
        file.open("data.png", std::ios::app | std::ios::binary);
        file.write(reinterpret_cast<const char*>(filedata->data()), filedata->size()); */

        m_context->swapBuffers();
    }

    Vec2f MainWindow_unix::getDPIScale() const 
    {
        return {1.0f, 1.0f};
    }

    bool MainWindow_unix::resize(const Size& size) 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::setLimits(const Size&, const Size&) 
    {
        // TODO
        return true;
    }

    void MainWindow_unix::beginPaint() 
    {

    }

    void MainWindow_unix::endPaint() 
    {

    }

    bool MainWindow_unix::setTitle(const std::string& name) 
    {
        XStoreName(m_display, m_window, name.c_str());
        return true;
    }

    bool MainWindow_unix::setIcon(int32 width, int32 height, byte* pixels)  
    {
        // TODO
        return true;
    }

    void MainWindow_unix::notifyEvent() 
    {
        // TODO
    }

    Point MainWindow_unix::getWinPos() const 
    {
        // TODO
        return {};
    }

    Size MainWindow_unix::getWinSize() const 
    {
        // TODO
        return {};
    }

    bool MainWindow_unix::minimize() 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::isMinimized() const 
    {
        // TODO
        return false;
    }

    bool MainWindow_unix::restore() 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::isFocused() const 
    {
        // TODO
        return true;
    }

    bool MainWindow_unix::setScaleHint(const Vec2f& scale) 
    {
        // TODO
        return true;
    }

    ::Window MainWindow_unix::xWindow() const
    {
        return m_window;
    }

    Atom MainWindow_unix::deleteAtom() const
    {
        return m_atomWmDeleteWindow;
    }
}
