//
//  util/ImmutableBuffer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-20.
//

// TODO(knobin): Add documentation.

#ifndef PTK_UTIL_IMMUTABLE_BUFFER_HPP
#define PTK_UTIL_IMMUTABLE_BUFFER_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"

// C++ Headers
#include <cstddef>
#include <filesystem>
#include <istream>

namespace pTK
{
    class PTK_API ImmutableBuffer : public NonCopyable, public NonMovable
    {
    public:
        using ReleaseProcedure = void (*)(const void* ptr);

    public:
        ImmutableBuffer() = delete;
        ~ImmutableBuffer() override;

    public:
        [[nodiscard]] static ImmutableBuffer MakeEmpty() { return {nullptr, 0}; }
        [[nodiscard]] static ImmutableBuffer MakeWithCopy(const uint8_t* source, std::size_t length);
        [[nodiscard]] static ImmutableBuffer MakeWithoutCopy(const uint8_t* source, std::size_t length);
        [[nodiscard]] static ImmutableBuffer MakeWithReleaseProcedure(const uint8_t* source, std::size_t length,
                                                                      ReleaseProcedure procedure);
        [[nodiscard]] static ImmutableBuffer MakeFromStream(std::istream& is);
        [[nodiscard]] static ImmutableBuffer MakeFromStream(std::istream& is, std::size_t count);
        [[nodiscard]] static ImmutableBuffer MakeFromFileName(std::string_view path);
        [[nodiscard]] static ImmutableBuffer MakeFromPath(const std::filesystem::path& path);

    public:
        [[nodiscard]] bool isEmpty() const noexcept { return !m_bytes; }
        [[nodiscard]] bool hasReleaseProcedure() const noexcept { return m_deleter; }
        [[nodiscard]] std::size_t size() const noexcept { return m_size; }
        [[nodiscard]] const void* data() const noexcept { return static_cast<const void*>(m_bytes); }
        [[nodiscard]] const uint8_t* bytes() const noexcept { return m_bytes; }

        std::size_t copy(uint8_t* destination) const;
        std::size_t copyRange(std::ptrdiff_t offset, std::size_t count, uint8_t* destination) const;

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
