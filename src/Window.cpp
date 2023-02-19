//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

// Local Headers
#include "Log.hpp"

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/Application.hpp"
#include "ptk/platform/base/ContextFactory.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkData.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Window::Window(const std::string& name, const Size& size, const WindowInfo& flags)
        : WindowBase(),
          SingleObject(),
          m_threadID{std::this_thread::get_id()}
    {
        // Create handle and context for platform.
        m_handle = Platform::WindowHandle::Make(this, name, size, flags);

        PTK_INFO("ContextBackendType Available:");
        PTK_INFO("\tRaster: {}", (Platform::ContextFactory::IsAvailable(ContextBackendType::Raster)) ? "Yes" : "No");
        PTK_INFO("\tGL:     {}", (Platform::ContextFactory::IsAvailable(ContextBackendType::GL)) ? "Yes" : "No");
        PTK_INFO("\tMetal:  {}", (Platform::ContextFactory::IsAvailable(ContextBackendType::Metal)) ? "Yes" : "No");

        m_context = Platform::ContextFactory::Make(this, size, getDPIScale(), flags);

        // Size policy.
        setSizePolicy(flags.sizePolicy);
        updateSize(size);

        // Set Widget properties.
        setName(name);

        // Show/Hide Window depending on flag.
        // Window should be created hidden by default!
        if (flags.visibility == WindowInfo::Visibility::Windowed)
            m_handle->show();

        PTK_INFO("Initialized Window");
    }

    Window::~Window()
    {
        // Remove the Window from the Application if it exists, in case it is still there.
        if (auto app = Application::Get())
            app->removeWindow(this);

        m_context.reset();
        m_handle.reset();

        PTK_INFO("Destroyed Window");
    }

    void Window::onChildDraw([[maybe_unused]] size_type index)
    {
        invalidate();
    }

    void Window::runCommands()
    {
        // Switch buffer.
        const std::size_t currentIndex = m_activeCmdBufferIndex;
        if (m_activeCmdBufferIndex == 0)
            m_activeCmdBufferIndex = 1;
        else
            m_activeCmdBufferIndex = 0;

        // Run commands.
        m_commandBuffers[currentIndex].batchInvoke();
    }

    bool Window::shouldClose() const
    {
        return m_close;
    }

    void Window::invalidate()
    {
        if (!m_contentInvalidated)
        {
            // Check time since last draw here.
            m_contentInvalidated = true;
            m_handle->notifyEvent();
        }
    }

    bool Window::drawContent()
    {
        if (!isContentValid())
        {
            m_handle->invalidate();
            return true;
        }

        return false;
    }

    std::size_t Window::timeSinceLastDraw() const
    {
        using namespace std::chrono;
        const time_point<steady_clock> now = steady_clock::now();
        return duration_cast<milliseconds>(now - m_lastDrawTime).count();
    }

    void Window::onSizeChange(const Size& size)
    {
        m_handle->resize(size);

        auto scale = getDPIScale();
        const Size scaledSize{static_cast<Size::value_type>(size.width * scale.x),
                              static_cast<Size::value_type>(size.height * scale.y)};
        if (scaledSize != m_context->getSize())
            m_context->resize(scaledSize);

        refitContent(size);
        invalidate();
    }

    void Window::regionInvalidated(const PaintEvent&)
    {
        // Just assume that the entire window needs to be painted here (for now).
        // Another (better) solution would be to find what children needs to be
        // painted and just paint those. But currently there isn't any support
        // for rendering individual widgets anyway.
        paint();
    }

    void Window::onLimitChange(const Size&, const Size&)
    {
        setLimitsWithSizePolicy();
    }

    void Window::setLimitsWithSizePolicy()
    {
        Limits limits{getLimitsWithSizePolicy()};
        m_handle->setLimits(limits.min, limits.max);
    }

    void Window::paint()
    {
        ContextBase* context{getContext()};
        sk_sp<SkSurface> surface = context->surface();
        SkCanvas* canvas{surface->getCanvas()};

        // Apply monitor scale.
        SkMatrix matrix{};
        Vec2f scale{getDPIScale()};
        matrix.setScale(scale.x, scale.y);
        canvas->setMatrix(matrix);

        // Will paint background and then children.
        onDraw(canvas);

        surface->flushAndSubmit();
        m_context->swapBuffers();

        // Painting is done, enable invalidation again.
        markContentValid();
    }

    void Window::markContentValid()
    {
        m_contentInvalidated = false;
        m_lastDrawTime = std::chrono::steady_clock::now();

#if 1
        using namespace std::chrono;
        static time_point<steady_clock> last = steady_clock::now();
        time_point<steady_clock> now = std::chrono::steady_clock::now();

        std::size_t elapsed = duration_cast<milliseconds>(now - last).count();
        static int fps = 1;
        if (elapsed >= 1000)
        {
            PTK_INFO("Window: \"{}\" [DRAWING] FPS = {}", getName(), fps);
            last = m_lastDrawTime;
            fps = 1;
        }
        else
            ++fps;
#endif
    }

    void Window::setSizePolicy(SizePolicy policy)
    {
        Widget::setSizePolicy(policy);
        setLimitsWithSizePolicy();
    }

    void Window::show()
    {
        if (isHidden())
            m_handle->show();
    }

    void Window::close()
    {
        if (!isClosed())
        {
            m_handle->close();
            m_closed = true;
        }
    }

    void Window::hide()
    {
        if (!isHidden())
            m_handle->hide();
    }

    bool Window::minimize()
    {
        if (!isMinimized())
            return m_handle->minimize();

        return false;
    }

    bool Window::restore()
    {
        if (isMinimized())
            return m_handle->restore();

        return false;
    }

    bool Window::setIconFromFile(const std::string& path)
    {
        // Load file.
        sk_sp<SkData> imageData{SkData::MakeFromFileName(path.c_str())};
        if (imageData)
        {
            // Convert file to image.
            sk_sp<SkImage> image{SkImage::MakeFromEncoded(imageData)};
            if (image)
            {
                // Read pixels in an RGBA format.
                const SkImageInfo imageInfo{image->imageInfo().makeColorType(SkColorType::kRGBA_8888_SkColorType)};
                const size_t storageSize{imageInfo.computeMinByteSize()};
                std::unique_ptr<uint8_t[]> pixelData{std::make_unique<uint8_t[]>(storageSize)};

                if (image->readPixels(imageInfo, pixelData.get(), imageInfo.minRowBytes(), 0, 0))
                    return m_handle->setIcon(static_cast<int32_t>(image->width()),
                                             static_cast<int32_t>(image->height()), pixelData.get());
#ifdef PTK_DEBUG
                else
                {
                    PTK_WARN("Failed to convert image \"{}\" to a RGBA format", path);
                }
#endif
            }
#ifdef PTK_DEBUG
            else
            {
                PTK_WARN("Could not decode image \"{}\"", path);
            }
#endif
        }
#ifdef PTK_DEBUG
        else
        {
            PTK_WARN("Failed to open \"{}\"", path);
        }
#endif
        return false;
    }

} // namespace pTK
