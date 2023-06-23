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
        const auto byteCount{pixelCount * GetColorTypeInfo(colorType).totalBits};
        m_bytes = std::make_unique<uint8_t[]>(byteCount);
    }

    bool Pixmap::isValid() const noexcept
    {
        return m_bytes && m_colorType != ColorType::Unknown && m_size.width > 0 && m_size.height > 0;
    }

    uint8_t* Pixmap::internal_at(std::ptrdiff_t x, std::ptrdiff_t y) const
    {
        const auto width{static_cast<std::ptrdiff_t>(m_size.width)};
        const auto index{(y * width) + x};
        return m_bytes.get() + index;
    }

    std::size_t Pixmap::copy(uint8_t* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        uint8_t* bytes{m_bytes.get()};
        const auto pixelCount{static_cast<std::size_t>(m_size.width * m_size.height)};
        const auto byteCount{pixelCount * GetColorTypeInfo(colorType()).totalBits};

        PTK_DEBUG_POINTER_OVERLAP_CHECK(bytes, byteCount, destination);

        std::memcpy(destination, bytes, byteCount);
        return byteCount;
    }

    std::size_t Pixmap::copyRange(std::ptrdiff_t offset, std::size_t count, uint8_t* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        uint8_t* bytes{m_bytes.get()};

        PTK_DEBUG_POINTER_OVERLAP_CHECK(
            bytes, static_cast<std::size_t>(m_size.width * m_size.height) * GetColorTypeInfo(colorType()).totalBits,
            destination);

        std::memcpy(destination, bytes + offset, count);
        return count;
    }
} // namespace pTK