//
//  util/Pixmap.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

// Local Headers
#include "../include/PointerOverlapDefine.hpp"

// pTK Headers
#include "ptk/util/ColorType.hpp"
#include "ptk/util/Pixmap.hpp"

namespace pTK
{
    Pixmap::Pixmap(uint32_t width, uint32_t height, ColorType colorType)
        : m_size{width, height},
          m_colorType{colorType}
    {
        const auto pixelCount{static_cast<std::size_t>(m_size.width * m_size.height)};
        m_bytes = std::make_unique<uint8_t[]>(pixelCount * bytesPerPixel());
    }

    bool Pixmap::isValid() const noexcept
    {
        return m_bytes && m_colorType != ColorType::Unknown && m_size.width > 0 && m_size.height > 0;
    }

    uint32_t Pixmap::length() const noexcept
    {
        const auto pixelCount{static_cast<uint32_t>(m_size.width * m_size.height)};
        return pixelCount * bytesPerPixel();
    }

    uint32_t Pixmap::bytesPerPixel() const noexcept
    {
        return static_cast<uint32_t>(GetColorTypeInfo(m_colorType).totalBits / 8);
    }

    uint8_t* Pixmap::internal_at(std::ptrdiff_t x, std::ptrdiff_t y) const
    {
        PTK_ASSERT(m_bytes, "pointer is nullptr");

        if (!m_bytes)
            return nullptr;

        PTK_ASSERT(x < width(), "x outside of range");
        PTK_ASSERT(y < height(), "y outside of range");

        const auto bpp{static_cast<std::ptrdiff_t>(bytesPerPixel())};
        const auto width{static_cast<std::ptrdiff_t>(m_size.width) * bpp};
        const auto index{(y * width) + (x * bpp)};

        return m_bytes.get() + index;
    }

    std::size_t Pixmap::copy(uint8_t* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        uint8_t* bytes{m_bytes.get()};
        const auto pixelCount{static_cast<std::size_t>(m_size.width * m_size.height)};
        const auto byteCount{pixelCount * bytesPerPixel()};

        PTK_DEBUG_POINTER_OVERLAP_CHECK(bytes, byteCount, destination);

        std::memcpy(destination, bytes, byteCount);
        return byteCount;
    }

    std::size_t Pixmap::copyRange(std::ptrdiff_t offset, std::size_t count, uint8_t* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        uint8_t* bytes{m_bytes.get()};

        PTK_DEBUG_POINTER_OVERLAP_CHECK(bytes, static_cast<std::size_t>(length()), destination);

        std::memcpy(destination, bytes + offset, count);
        return count;
    }
} // namespace pTK