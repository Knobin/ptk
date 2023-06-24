//
//  util/ImmutableBuffer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-20.
//

#ifndef PTK_UTIL_IMMUTABLE_BUFFER_HPP
#define PTK_UTIL_IMMUTABLE_BUFFER_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"

// C++ Headers
#include <cstddef>
#include <filesystem>

namespace pTK
{
    /** ImmutableBuffer class implementation.

        Represents a constant buffer of data that is never meant to change during
        the lifetime of this object.

        Buffer can either be created by the class or by the user.
    */
    class PTK_API ImmutableBuffer : public NonCopyable, public NonMovable
    {
    public:
        // Callback function to delete the buffer pointer is needed.
        using ReleaseProcedure = void (*)(const void* ptr);

    public:
        /** Deleted constructor.

            Note: This may change in the future, for now, use MakeEmpty() function.
        */
        ImmutableBuffer() = delete;

        /** Destructor for ImmutableBuffer.

            Note: Will call ReleaseProcedure with the buffer pointer if not nullptr.
        */
        ~ImmutableBuffer() override;

    public:
        /** Function for creating an empty ImmutableBuffer.

            @return     initialized empty ImmutableBuffer
        */
        [[nodiscard]] static ImmutableBuffer MakeEmpty() { return {nullptr, 0}; }

        /** Function for creating ImmutableBuffer by copying a provided source.

            Note: if source pointer is nullptr an empty ImmutableBuffer is returned.

            @param source   pointer to copy from
            @param length   number of bytes to copy
            @return         initialized ImmutableBuffer with data from source
        */
        [[nodiscard]] static ImmutableBuffer MakeWithCopy(const uint8_t* source, std::size_t length);

        /** Function for creating ImmutableBuffer with a provided source.

            Note: The user have to assure that the source pointer is valid during the lifetime of
                  this object.

            Note: if source pointer is nullptr an empty ImmutableBuffer is returned.

            @param source   pointer to copy from
            @param length   number of bytes to use
            @return         initialized ImmutableBuffer with data from source
        */
        [[nodiscard]] static ImmutableBuffer MakeWithoutCopy(const uint8_t* source, std::size_t length);

        /** Function for creating ImmutableBuffer with a provided source.

            Note: ReleaseProcedure functions will be called during destruction of this object, be sure
                  to delete the data pointer in there to avoid memory leaks.

            Note: if source pointer is nullptr an empty ImmutableBuffer is returned.

            @param source       pointer to copy from
            @param length       number of bytes to use
            @param procedure    delete callback for the data pointer
            @return             initialized ImmutableBuffer with data from source
        */
        [[nodiscard]] static ImmutableBuffer MakeWithReleaseProcedure(const uint8_t* source, std::size_t length,
                                                                      ReleaseProcedure procedure);

        /** Function for creating ImmutableBuffer with a provided input stream.

            @param is   input stream to copy data from
            @return     initialized ImmutableBuffer with input stream
        */
        [[nodiscard]] static ImmutableBuffer MakeFromStream(std::istream& is);

        /** Function for creating ImmutableBuffer with a provided input stream.

            @param is       input stream to copy data from
            @param length   number of bytes to copy
            @return         initialized ImmutableBuffer with input stream
        */
        [[nodiscard]] static ImmutableBuffer MakeFromStream(std::istream& is, std::size_t count);

        /** Function for creating ImmutableBuffer with filepath.

            @param path     file path to open and copy data from
            @return         initialized ImmutableBuffer with input file
        */
        [[nodiscard]] static ImmutableBuffer MakeFromFileName(std::string_view path);

        /** Function for creating ImmutableBuffer with filesystem path.

            @param path     filesystem path to open and copy data from
            @return         initialized ImmutableBuffer with input file
        */
        [[nodiscard]] static ImmutableBuffer MakeFromPath(const std::filesystem::path& path);

    public:
        /** Function for checking if the buffer is empty.

            @return     status
        */
        [[nodiscard]] bool isEmpty() const noexcept { return !m_bytes; }

        /** Function for checking if the buffer has a release procedure callback.

            Note: Both data pointer and release procedure must be valid for the
                  deleter to be called in destruction.

            @return     status
        */
        [[nodiscard]] bool hasReleaseProcedure() const noexcept { return m_deleter; }

        /** Function for retrieving the number of bytes in the buffer.

            @return     number of bytes
        */
        [[nodiscard]] std::size_t size() const noexcept { return m_size; }

        /** Function for retrieving the data pointer of the buffer.

            @return     void pointer to start of buffer
        */
        [[nodiscard]] const void* data() const noexcept { return static_cast<const void*>(m_bytes); }

        /** Function for retrieving the data pointer of the buffer.

            @return     byte pointer to start of buffer
        */
        [[nodiscard]] const uint8_t* bytes() const noexcept { return m_bytes; }

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

        /** bool operator for checking if buffer contains data.

            @return  status
        */
        explicit operator bool() const noexcept { return !isEmpty(); }

    private:
        ImmutableBuffer(const uint8_t* bytes, std::size_t size, ReleaseProcedure deleter = nullptr)
            : m_size{size},
              m_deleter{deleter},
              m_bytes{bytes}
        {}

    private:
        std::size_t m_size{};
        ReleaseProcedure m_deleter{nullptr};
        const uint8_t* m_bytes{nullptr};
    };
} // namespace pTK

#endif // PTK_UTIL_IMMUTABLE_BUFFER_HPP
