//
//  Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

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
        // More commands might be pushed while being in here.
        // Therefore, the lock has to be acquired multiple times
        // to be safe that no deadlocks will occur.

        // Number of commands to run.
        m_commandBuffer.lock();
        std::size_t cmdCount{m_commandBuffer.size()};
        m_commandBuffer.unlock();

        // Run commands.
        while (cmdCount > 0)
        {
            m_commandBuffer.lock();
            std::function<void()> command = std::move(m_commandBuffer.front());
            m_commandBuffer.pop();
            m_commandBufferSize = m_commandBuffer.size();
            m_commandBuffer.unlock();
            command();
            --cmdCount;
        }
    }

    bool Window::shouldClose() const
    {
        return m_close;
    }

    void Window::invalidate()
    {
        if (!m_contentInvalidated)
        {
            m_contentInvalidated = true;
            m_handle->invalidate();
        }
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
