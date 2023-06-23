//
//  util/Pixmap.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

// TODO(knobin): Add documentation.

#ifndef PTK_UTIL_PIXMAP_HPP
#define PTK_UTIL_PIXMAP_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/ColorType.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"
#include "ptk/util/Size.hpp"

// C++ Headers
#include <cstdint>
#include <memory>

namespace pTK
{
    class PTK_API Pixmap : public NonCopyable, public NonMovable
    {
    public:
        Pixmap() = delete;
        Pixmap(uint32_t width, uint32_t height, ColorType colorType);
        ~Pixmap() override = default;

        [[nodiscard]] bool isValid() const noexcept { return m_bytes && m_size.width > 0 && m_size.height > 0; }

        [[nodiscard]] Size size() const noexcept { return m_size; }
        [[nodiscard]] uint32_t width() const noexcept { return m_size.width; }
        [[nodiscard]] uint32_t height() const noexcept { return m_size.height; }

        [[nodiscard]] ColorType colorType() const noexcept { return m_colorType; }
        void setColorType(ColorType colorType) noexcept { m_colorType = colorType; }

        [[nodiscard]] uint8_t* bytes() noexcept { return m_bytes.get(); }
        [[nodiscard]] const uint8_t* bytes() const noexcept { return m_bytes.get(); }

        [[nodiscard]] void* data() noexcept { return static_cast<void*>(m_bytes.get()); }
        [[nodiscard]] const void* data() const noexcept { return static_cast<const void*>(m_bytes.get()); }

        [[nodiscard]] uint8_t* at(std::ptrdiff_t x, std::ptrdiff_t y) { return internal_at(x, y); }
        [[nodiscard]] const uint8_t* at(std::ptrdiff_t x, std::ptrdiff_t y) const { return internal_at(x, y); }

        [[nodiscard]] uint8_t* row(std::ptrdiff_t y) { return internal_at(0, y); }
        [[nodiscard]] const uint8_t* row(std::ptrdiff_t y) const { return internal_at(0, y); }

        std::size_t copy(uint8_t* destination) const;
        std::size_t copyRange(std::ptrdiff_t offset, std::size_t count, uint8_t* destination) const;

    private:
        [[nodiscard]] uint8_t* internal_at(std::ptrdiff_t x, std::ptrdiff_t y) const;

    private:
        std::unique_ptr<uint8_t[]> m_bytes{};
        Size m_size{};
        ColorType m_colorType{};
    };
} // namespace pTK

#endif // PTK_UTIL_PIXMAP_HPP
