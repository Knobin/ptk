//
//  util/ImmutableBuffer.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-20.
//

// Local Headers
#include "../include/PointerOverlapDefine.hpp"

// pTK Headers
#include "ptk/util/ImmutableBuffer.hpp"

// C++ Headers
#include <fstream>

namespace pTK
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    static void ByteArrayReleaseProcedure(const void* ptr)
    {
        const auto bytes = static_cast<const uint8_t*>(ptr);
        delete[] bytes;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    ImmutableBuffer::~ImmutableBuffer()
    {
        if (m_deleter)
            m_deleter(static_cast<const void*>(m_bytes));
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    ImmutableBuffer ImmutableBuffer::MakeWithCopy(const uint8_t* source, std::size_t length)
    {
        if (!source)
            return MakeEmpty();

        uint8_t* bytes{new (std::nothrow) uint8_t[length]};
        if (!bytes)
            return MakeEmpty();

        std::memcpy(bytes, source, length);
        return {bytes, length, ByteArrayReleaseProcedure};
    }

    ImmutableBuffer ImmutableBuffer::MakeWithoutCopy(const uint8_t* source, std::size_t length)
    {
        if (!source)
            return MakeEmpty();

        return {source, length};
    }

    ImmutableBuffer ImmutableBuffer::MakeWithReleaseProcedure(const uint8_t* source, std::size_t length,
                                                              ReleaseProcedure procedure)
    {
        if (!source)
            return MakeEmpty();

        return {source, length, procedure};
    }

    ImmutableBuffer ImmutableBuffer::MakeFromStream(std::istream& is)
    {
        is.seekg(0, std::ios_base::end); // Go to end of stream.
        const std::size_t size{static_cast<std::size_t>(is.tellg())};
        is.seekg(0, std::ios_base::beg); // Go to beginning of stream.

        uint8_t* bytes{new (std::nothrow) uint8_t[size]};
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
        uint8_t* bytes{new (std::nothrow) uint8_t[size]};

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

            uint8_t* bytes{new (std::nothrow) uint8_t[size]};
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

            uint8_t* bytes{new (std::nothrow) uint8_t[size]};
            if (bytes)
                if (fs.read(reinterpret_cast<std::istream::char_type*>(bytes), static_cast<std::streamsize>(size)))
                    return {bytes, size, ByteArrayReleaseProcedure};
        }

        return MakeEmpty();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t ImmutableBuffer::copy(uint8_t* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        PTK_DEBUG_POINTER_OVERLAP_CHECK(m_bytes, m_size, destination);

        std::memcpy(destination, m_bytes, m_size);
        return m_size;
    }

    std::size_t ImmutableBuffer::copyRange(std::ptrdiff_t offset, std::size_t count, uint8_t* destination) const
    {
        if (!m_bytes || !destination)
            return 0;

        PTK_DEBUG_POINTER_OVERLAP_CHECK(m_bytes, m_size, destination);

        std::memcpy(destination, m_bytes + offset, count);
        return count;
    }
} // namespace pTK