//
//  core/ImmutableBuffer.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-20.
//

// pTK Headers
#include "ptk/core/ImmutableBuffer.hpp"
#include "ptk/core/Defines.hpp"

// C++ Headers
#include <fstream>

// TODO(knobin): Add separation lines (############).

#ifdef PTK_DEBUG
    #include "Assert.hpp"
// PTK_DEBUG_OVERLAP_CHECK(m_bytes, m_size, destination);
static bool CheckOverlap(const std::byte* source, std::size_t count, const std::byte* destination)
{
    const std::byte* start = source;
    const std::byte* end = start + static_cast<std::ptrdiff_t>(count);

    return ((destination >= start) && (end >= destination));
}

    #define PTK_DEBUG_OVERLAP_CHECK(...) PTK_ASSERT(!CheckOverlap(__VA_ARGS__), "Source and Destination pointer overlap")
#else
    #define PTK_DEBUG_OVERLAP_CHECK(...)
#endif

namespace pTK
{
    static void ByteArrayReleaseProcedure(const void* ptr)
    {
        const auto bytes = static_cast<const std::byte*>(ptr);
        delete[] bytes;
    }

    ImmutableBuffer ImmutableBuffer::MakeWithCopy(const std::byte* source, std::size_t count)
    {
        if (!source)
            return MakeEmpty();

        std::byte* bytes{new (std::nothrow) std::byte[count]};
        if (!bytes)
            return MakeEmpty();

        std::memcpy(bytes, source, count);
        return {bytes, count, ByteArrayReleaseProcedure};
    }

    ImmutableBuffer ImmutableBuffer::MakeWithoutCopy(const std::byte* source, std::size_t count)
    {
        if (!source)
            return MakeEmpty();

        return {source, count};
    }

    ImmutableBuffer ImmutableBuffer::MakeFromStream(std::istream& is)
    {
        is.seekg(0, std::ios_base::end); // Go to end of stream.
        const std::size_t size{static_cast<std::size_t>(is.tellg())};
        is.seekg(0, std::ios_base::beg); // Go to beginning of stream.

        std::byte* bytes{new (std::nothrow) std::byte[size]};
        if (bytes)
            if (is.read(reinterpret_cast<std::istream::char_type*>(bytes), static_cast<std::streamsize>(size)))
                return {bytes, size, ByteArrayReleaseProcedure};

        return MakeEmpty();
    }

    ImmutableBuffer ImmutableBuffer::MakeFromStream(std::istream& is, std::size_t count)
    {
        is.seekg(0, std::ios_base::end); // Go to end of stream.
        const std::size_t streamSize{static_cast<std::size_t>(is.tellg())};
        is.seekg(0, std::ios_base::beg); // Go to beginning of stream.

        const std::size_t size{(count > streamSize) ? streamSize : count};
        std::byte* bytes{new (std::nothrow) std::byte[size]};

        if (bytes)
            if (is.read(reinterpret_cast<std::istream::char_type*>(bytes), static_cast<std::streamsize>(size)))
                return {bytes, size, ByteArrayReleaseProcedure};

        return MakeEmpty();
    }

    ImmutableBuffer ImmutableBuffer::MakeFromFileName(std::string_view path)
    {
        if (std::ifstream fs{path.data(), std::ios::in | std::ios::binary})
        {
            fs.seekg(0, std::ios_base::end); // Go to end of stream.
            const std::size_t size{static_cast<std::size_t>(fs.tellg())};
            fs.seekg(0, std::ios_base::beg); // Go to beginning of stream.

            std::byte* bytes{new (std::nothrow) std::byte[size]};
            if (bytes)
                if (fs.read(reinterpret_cast<std::istream::char_type*>(bytes), static_cast<std::streamsize>(size)))
                    return {bytes, size, ByteArrayReleaseProcedure};
        }

        return MakeEmpty();
    }

    ImmutableBuffer ImmutableBuffer::MakeFromPath(const std::filesystem::path& path)
    {
        if (std::ifstream fs{path.c_str(), std::ios::in | std::ios::binary})
        {
            fs.seekg(0, std::ios_base::end); // Go to end of stream.
            const std::size_t size{static_cast<std::size_t>(fs.tellg())};
            fs.seekg(0, std::ios_base::beg); // Go to beginning of stream.

            std::byte* bytes{new (std::nothrow) std::byte[size]};
            if (bytes)
                if (fs.read(reinterpret_cast<std::istream::char_type*>(bytes), static_cast<std::streamsize>(size)))
                    return {bytes, size, ByteArrayReleaseProcedure};
        }

        return MakeEmpty();
    }

    std::size_t ImmutableBuffer::copy(std::byte* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        PTK_DEBUG_OVERLAP_CHECK(m_bytes, m_size, destination);

        std::memcpy(destination, m_bytes, m_size);
        return m_size;
    }

    std::size_t ImmutableBuffer::copyRange(std::ptrdiff_t offset, std::size_t count, std::byte* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        PTK_DEBUG_OVERLAP_CHECK(m_bytes, m_size, destination);

        std::memcpy(destination, m_bytes + offset, count);
        return count;
    }
} // namespace pTK