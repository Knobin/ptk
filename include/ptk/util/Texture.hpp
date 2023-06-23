//
//  util/Texture.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-23.
//

// TODO(knobin): Add documentation.

#ifndef PTK_UTIL_TEXTURE_HPP
#define PTK_UTIL_TEXTURE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/ImmutableBuffer.hpp"
#include "ptk/util/Pixmap.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"
#include "ptk/util/Size.hpp"

// C++ Headers
#include <cstdint>
#include <memory>

// Skia forward declarations.
template <typename T>
class sk_sp;
class SkImage;

namespace pTK
{
    class PTK_API Texture : public NonCopyable, public NonMovable
    {
    public:
        Texture() = default;
        explicit Texture(const ImmutableBuffer& buffer);
        explicit Texture(const Pixmap& pixmap);
        ~Texture() override;

        [[nodiscard]] bool isValid() const;

        [[nodiscard]] Size size() const noexcept { return m_size; }
        [[nodiscard]] uint32_t width() const noexcept { return m_size.width; }
        [[nodiscard]] uint32_t height() const noexcept { return m_size.height; }

        [[nodiscard]] SkImage *skImage() const noexcept;

    private:
        std::unique_ptr<sk_sp<SkImage>> m_image{};
        Size m_size{};
    };
} // namespace pTK

#endif // PTK_UTIL_TEXTURE_HPP
