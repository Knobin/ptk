//
//  util/Texture.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-23.
//

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
    /** Texture class implementation.

        Represents a 2D array of drawable pixels created by either:
            - ImmutableBuffer
            - Pixmap

        Data will be copied to appropriate format for drawing and input data can therefore
        be destructed right after initialization.

        The data is not meant to be changed after initialization and is final, if changes
        are desired a new Texture must be created.
    */
    class PTK_API Texture : public NonCopyable, public NonMovable
    {
    public:
        /** Constructs empty Texture.

            @return    empty initialized Texture
        */
        Texture() = default;

        /** Constructs Texture from ImmutableBuffer.

            Note: Data from ImmutableBuffer will be copied.

            @param buffer   data buffer
            @return         initialized Texture with buffer
        */
        explicit Texture(const ImmutableBuffer& buffer);

        /** Constructs Texture from Pixmap.

            Note: Data from Pixmap will be copied.

           @param pixmap   pixel buffer
           @return         initialized Texture with pixels
        */
        explicit Texture(const Pixmap& pixmap);

        /** Destructor for Texture.

        */
        ~Texture() override;

        /** Function for checking if the Texture has been created successfully.

            @return     status
        */
        [[nodiscard]] bool isValid() const;

        /** Function for retrieving the size of the Texture.

            Width and Height are zero if not a valid Texture.

            @return     size of the Texture
        */
        [[nodiscard]] Size size() const noexcept { return m_size; }

        /** Function for retrieving the width of the Texture.

            Width is zero if not a valid Texture.

            @return     width of the Texture
        */
        [[nodiscard]] uint32_t width() const noexcept { return m_size.width; }

        /** Function for retrieving the height of the Texture.

            Height is zero if not a valid Texture.

            @return     height of the Texture
        */
        [[nodiscard]] uint32_t height() const noexcept { return m_size.height; }

        /** Function for retrieving the height of the Texture.

            SkImage pointer is nullptr if not a valid Texture.

            @return     pointer to SkImage
        */
        [[nodiscard]] SkImage *skImage() const noexcept;

    private:
        std::unique_ptr<sk_sp<SkImage>> m_image{};
        Size m_size{};
    };
} // namespace pTK

#endif // PTK_UTIL_TEXTURE_HPP
