//
//  util/Pixmap.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

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
    /** Pixmap class implementation.

        Represents a 2D array of pixels with specific color types that can easily
        be manipulated with functions specifically for handling pixels and 2D arrays.
    */
    class PTK_API Pixmap : public NonCopyable, public NonMovable
    {
    public:
        /** Constructs empty Pixmap.

            @return    empty initialized Pixmap
        */
        Pixmap() = default;

        /** Constructs Pixmap with size and color type.

            Allocates and array with width * height * bytesPerPixel.
            Where bytesPerPixel is the amount of bytes required per pixel by the
            provided color type.

            Note: Pixmap will not throw if allocation failed.

            @param width        width in pixels
            @param height       height in pixels
            @param colorType    pixel channel information
            @return             initialized Pixmap with width, height and color type
        */
        Pixmap(uint32_t width, uint32_t height, ColorType colorType);

        /** Destructor for Pixmap.

        */
        ~Pixmap() override = default;

        /** Function for checking if the Pixmap has been created successfully.

            Requirements for valid:
                - pointer is not nullptr.
                - color type is not ColorType::Unknown.
                - width > 0
                - height > 0

            @return     status
        */
        [[nodiscard]] bool isValid() const noexcept;

        /** Function for retrieving the size of the Pixmap.

            @return     size (width and height in pixels)
        */
        [[nodiscard]] Size size() const noexcept { return m_size; }

        /** Function for retrieving the width (in pixels) of the Pixmap.

            @return     width in pixels
        */
        [[nodiscard]] uint32_t width() const noexcept { return m_size.width; }

        /** Function for retrieving the height (in pixels) of the Pixmap.

            @return     height in pixels
        */
        [[nodiscard]] uint32_t height() const noexcept { return m_size.height; }

        /** Function for retrieving the number of bytes in the Pixmap.

            @return     number of bytes
        */
        [[nodiscard]] uint32_t length() const noexcept;

        /** Function for retrieving the number of bytes required per pixel in the Pixmap.

            @return     number of bytes per pixel
        */
        [[nodiscard]] uint32_t bytesPerPixel() const noexcept;

        /** Function for retrieving the color type of the pixels in the Pixmap.

            @return     color type of the pixels
        */
        [[nodiscard]] ColorType colorType() const noexcept { return m_colorType; }

        /** Function for setting the color type of the pixels in the Pixmap.

            Note: Does not change the bytes only change how the array is indexed.

            @param colorType    color type to set
        */
        void setColorType(ColorType colorType) noexcept { m_colorType = colorType; }

        /** Function for retrieving pointer to first byte in the Pixmap.

            Note: Pointer may be nullptr.

           @return     pointer to first byte in array
        */
        [[nodiscard]] uint8_t* bytes() noexcept { return m_bytes.get(); }

        /** Function for retrieving pointer to first byte in the Pixmap.

            Note: Pointer may be nullptr.

            @return     pointer to first byte in array
        */
        [[nodiscard]] const uint8_t* bytes() const noexcept { return m_bytes.get(); }

        /** Function for retrieving void pointer to the array of the Pixmap.

            Note: Pointer may be nullptr.

            @return     void pointer to start of the array
        */
        [[nodiscard]] void* data() noexcept { return static_cast<void*>(m_bytes.get()); }

        /** Function for retrieving void pointer to the array of the Pixmap.

            Note: Pointer may be nullptr.

           @return     void pointer to start of the array
        */
        [[nodiscard]] const void* data() const noexcept { return static_cast<const void*>(m_bytes.get()); }

        /** Function for retrieving a pointer to specific pixel of the Pixmap.

            Note: Pointer may be nullptr.

            @param x    pixel column index
            @param y    pixel row index
            @return     pointer to specific pixel at x and y
        */
        [[nodiscard]] uint8_t* at(std::ptrdiff_t x, std::ptrdiff_t y) { return internal_at(x, y); }

        /** Function for retrieving a pointer to specific pixel of the Pixmap.

            Note: Pointer may be nullptr.

           @param x    pixel column index
           @param y    pixel row index
           @return     pointer to specific pixel at x and y
        */
        [[nodiscard]] const uint8_t* at(std::ptrdiff_t x, std::ptrdiff_t y) const { return internal_at(x, y); }

        /** Function for retrieving a pointer to specific row of pixels in the Pixmap.

            Note: Pointer may be nullptr.

            @param y    pixel row index
            @return     pointer to start of row of pixels
        */
        [[nodiscard]] uint8_t* row(std::ptrdiff_t y) { return internal_at(0, y); }

        /** Function for retrieving a pointer to specific row of pixels in the Pixmap.

            Note: Pointer may be nullptr.

           @param y    pixel row index
           @return     pointer to start of row of pixels
        */
        [[nodiscard]] const uint8_t* row(std::ptrdiff_t y) const { return internal_at(0, y); }

        /** Function for copying the full buffer to another buffer.

            Note: user must ensure that the destination buffer has space to fit the
                  data before calling this function.

            Note: if data or destination pointer is null no bytes are copied.

            @param destination  pointer to copy to
            @return             number of bytes copied
        */
        std::size_t copy(uint8_t* destination) const;

        /** Function for copying the full buffer to another buffer.

            Note: user must ensure that the destination buffer has space to fit the
                  data before calling this function.

           Note: if data or destination pointer is null no bytes are copied.

            @param offset       data buffer offset to start the copy at
            @param destination  pointer to copy to
            @param count        number of bytes to copy
            @return             number of bytes copied
        */
        std::size_t copyRange(std::ptrdiff_t offset, std::size_t count, uint8_t* destination) const;

    private:
        // Helper function to index pixels.
        [[nodiscard]] uint8_t* internal_at(std::ptrdiff_t x, std::ptrdiff_t y) const;

    private:
        std::unique_ptr<uint8_t[]> m_bytes{};
        Size m_size{};
        ColorType m_colorType{};
    };
} // namespace pTK

#endif // PTK_UTIL_PIXMAP_HPP
