// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/core/ImmutableBuffer.hpp"

// C++ Headers
#include <initializer_list>
#include <sstream>

// TODO(knobin): Add more checks to copyRange() with varying offset, count and destination.
// TODO(knobin): Add tests for MakeFromFileName and MakeFromPath.

static bool CheckSame(const std::byte* d1, const std::byte* d2, std::size_t count)
{
    for (std::size_t i{0}; i < count; ++i)
        if (d1[i] != d2[i])
            return false;

    return true;
}

static bool CheckBytesContent(const std::byte* data, std::initializer_list<std::byte> values)
{
    std::size_t i{0};
    for (const std::byte byte : values)
    {
        if (data[i] != byte)
            return false;
        ++i;
    }

    return true;
}

static bool CopyRequire(const pTK::ImmutableBuffer& buffer, std::initializer_list<std::byte> values)
{
    std::byte d1[]{std::byte{0}, std::byte{0}, std::byte{0}};
    REQUIRE(CheckBytesContent(d1, {std::byte{0}, std::byte{0}, std::byte{0}}));

    std::size_t copied = buffer.copy(d1);
    REQUIRE(copied == 3);
    REQUIRE(CheckBytesContent(buffer.bytes(), values));
    REQUIRE(CheckBytesContent(d1, values));
    REQUIRE(CheckSame(d1, buffer.bytes(), buffer.size()));

    std::byte d2[]{std::byte{0}, std::byte{0}, std::byte{0}};
    REQUIRE(CheckBytesContent(d2, {std::byte{0}, std::byte{0}, std::byte{0}}));

    copied = buffer.copyRange(0, 3, d2);
    REQUIRE(copied == 3);
    REQUIRE(CheckBytesContent(buffer.bytes(), values));
    REQUIRE(CheckBytesContent(d2, values));
    REQUIRE(CheckSame(d2, buffer.bytes(), buffer.size()));

    std::byte d3[]{std::byte{0}, std::byte{0}, std::byte{0}};
    std::byte d3_expected[]{std::byte{0}, std::byte{0}, std::byte{0}};
    std::size_t i{0};
    std::size_t j{0};
    for (const std::byte byte : values)
        if (i++ != 0)
            d3_expected[j++] = byte;

    copied = buffer.copyRange(1, 2, d3);
    REQUIRE(copied == 2);
    REQUIRE(CheckBytesContent(buffer.bytes(), values));
    REQUIRE(CheckBytesContent(d3, {d3_expected[0], d3_expected[1], d3_expected[2]}));

    return true;
}

TEST_CASE("MakeEmpty")
{
    const auto buffer{pTK::ImmutableBuffer::MakeEmpty()};

    SECTION("Get")
    {
        REQUIRE_FALSE(buffer);
        REQUIRE(buffer.isEmpty());
        REQUIRE_FALSE(buffer.hasReleaseProcedure());
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.data() == nullptr);
        REQUIRE(buffer.bytes() == nullptr);
    }

    SECTION("Copy")
    {
        std::byte destination[]{std::byte{1}, std::byte{2}, std::byte{3}};

        std::size_t copied = buffer.copy(destination);
        REQUIRE(copied == 0);
        REQUIRE(CheckBytesContent(destination, {std::byte{1}, std::byte{2}, std::byte{3}}));

        copied = buffer.copyRange(0, 3, destination);
        REQUIRE(copied == 0);
        REQUIRE(CheckBytesContent(destination, {std::byte{1}, std::byte{2}, std::byte{3}}));
    }
}

TEST_CASE("MakeWithCopy")
{
    std::byte data[]{std::byte{1}, std::byte{2}, std::byte{3}};
    const auto buffer{pTK::ImmutableBuffer::MakeWithCopy(&data[0], 3)};

    SECTION("Get")
    {
        REQUIRE(buffer);
        REQUIRE_FALSE(buffer.isEmpty());
        REQUIRE(buffer.hasReleaseProcedure());
        REQUIRE(buffer.size() == 3);
        REQUIRE(buffer.data());
        REQUIRE(buffer.bytes());
    }

    SECTION("Copy")
    {
        CopyRequire(buffer, {std::byte{1}, std::byte{2}, std::byte{3}});
    }
}

TEST_CASE("MakeWithoutCopy")
{
    std::byte data[]{std::byte{1}, std::byte{2}, std::byte{3}};
    const auto buffer{pTK::ImmutableBuffer::MakeWithoutCopy(&data[0], 3)};

    SECTION("Get")
    {
        REQUIRE(buffer);
        REQUIRE_FALSE(buffer.isEmpty());
        REQUIRE_FALSE(buffer.hasReleaseProcedure());
        REQUIRE(buffer.size() == 3);
        REQUIRE(buffer.data() == static_cast<const void*>(data));
        REQUIRE(buffer.bytes() == data);
    }

    SECTION("Copy")
    {
        CopyRequire(buffer, {std::byte{1}, std::byte{2}, std::byte{3}});
    }
}

TEST_CASE("MakeFromStream")
{
    std::istringstream iss{"123"};
    const auto buffer{pTK::ImmutableBuffer::MakeFromStream(iss)};

    SECTION("Get")
    {
        REQUIRE(buffer);
        REQUIRE_FALSE(buffer.isEmpty());
        REQUIRE(buffer.hasReleaseProcedure());
        REQUIRE(buffer.size() == 3);
        REQUIRE(buffer.data());
        REQUIRE(buffer.bytes());
    }

    SECTION("Copy")
    {
        CopyRequire(buffer, {std::byte{49}, std::byte{50}, std::byte{51}});
    }
}

TEST_CASE("MakeFromStream (count)")
{
    std::istringstream iss{"1234"};
    const auto buffer{pTK::ImmutableBuffer::MakeFromStream(iss, 3)};

    SECTION("Get")
    {
        REQUIRE(buffer);
        REQUIRE_FALSE(buffer.isEmpty());
        REQUIRE(buffer.hasReleaseProcedure());
        REQUIRE(buffer.size() == 3);
        REQUIRE(buffer.data());
        REQUIRE(buffer.bytes());
    }

    SECTION("Copy")
    {
        CopyRequire(buffer, {std::byte{49}, std::byte{50}, std::byte{51}});
    }
}

#include <catch2/catch_session.hpp>
#include "../src/Log.hpp"

int main(int argc, char* argv[])
{
    // Logger is needed for Assert() in copy() and copyRange().
    PTK_INIT_LOGGING();

    return Catch::Session().run(argc, argv);
}