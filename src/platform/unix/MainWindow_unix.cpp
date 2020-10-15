//
//  platform/unix/MainWindow_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "MainWindow_unix.hpp"
#include "ptk/Window.hpp"

namespace pTK
{
    MainWindow_unix::MainWindow_unix(Window *window, const std::string& name, const Size& size, BackendType backend)
        : MainWindowBase(window, backend)
    {
        // TODO
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

    void MainWindow_unix::pollEvents() 
    {
        // TODO
    }

    void MainWindow_unix::waitEvents() 
    {
        // TODO
    }

    void MainWindow_unix::waitEventsTimeout(uint ms) 
    {
        // TODO
    }

    bool MainWindow_unix::setPosHint(const Point& pos) 
    {
        // TODO
        return true;
    }

    ContextBase *MainWindow_unix::getContext() const 
    {
        // TODO
        return nullptr;
    }

    void MainWindow_unix::swapBuffers() 
    {

    }

    Vec2f MainWindow_unix::getDPIScale() const 
    {
        // TODO
        return {};
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
        // TODO
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
}
