// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/ImmutableBuffer.hpp"

// C++ Headers
#include <cstdint>
#include <initializer_list>
#include <sstream>

// TODO(knobin): Add more checks to copyRange() with varying offset, count and destination.
// TODO(knobin): Add tests for MakeFromFileName and MakeFromPath.

static bool CheckEqual(const uint8_t* d1, const uint8_t* d2, std::size_t count)
{
    for (std::size_t i{0}; i < count; ++i)
        if (d1[i] != d2[i])
            return false;

    return true;
}

static bool CheckBytesContent(const uint8_t* data, std::initializer_list<uint8_t> values)
{
    std::size_t i{0};
    for (const uint8_t byte : values)
    {
        if (data[i] != byte)
            return false;
        ++i;
    }

    return true;
}

static bool CopyRequire(const pTK::ImmutableBuffer& buffer, std::initializer_list<uint8_t> values)
{
    uint8_t d1[]{uint8_t{0}, uint8_t{0}, uint8_t{0}};
    REQUIRE(CheckBytesContent(d1, {uint8_t{0}, uint8_t{0}, uint8_t{0}}));

    std::size_t copied = buffer.copy(d1);
    REQUIRE(copied == 3);
    REQUIRE(CheckBytesContent(buffer.bytes(), values));
    REQUIRE(CheckBytesContent(d1, values));
    REQUIRE(CheckEqual(d1, buffer.bytes(), buffer.size()));

    uint8_t d2[]{uint8_t{0}, uint8_t{0}, uint8_t{0}};
    REQUIRE(CheckBytesContent(d2, {uint8_t{0}, uint8_t{0}, uint8_t{0}}));

    copied = buffer.copyRange(0, 3, d2);
    REQUIRE(copied == 3);
    REQUIRE(CheckBytesContent(buffer.bytes(), values));
    REQUIRE(CheckBytesContent(d2, values));
    REQUIRE(CheckEqual(d2, buffer.bytes(), buffer.size()));

    uint8_t d3[]{uint8_t{0}, uint8_t{0}, uint8_t{0}};
    uint8_t d3_expected[]{uint8_t{0}, uint8_t{0}, uint8_t{0}};
    std::size_t i{0};
    std::size_t j{0};
    for (const uint8_t byte : values)
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
        uint8_t destination[]{uint8_t{1}, uint8_t{2}, uint8_t{3}};

        std::size_t copied = buffer.copy(destination);
        REQUIRE(copied == 0);
        REQUIRE(CheckBytesContent(destination, {uint8_t{1}, uint8_t{2}, uint8_t{3}}));

        copied = buffer.copyRange(0, 3, destination);
        REQUIRE(copied == 0);
        REQUIRE(CheckBytesContent(destination, {uint8_t{1}, uint8_t{2}, uint8_t{3}}));
    }
}

TEST_CASE("MakeWithCopy")
{
    uint8_t data[]{uint8_t{1}, uint8_t{2}, uint8_t{3}};
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
        CopyRequire(buffer, {uint8_t{1}, uint8_t{2}, uint8_t{3}});
    }
}

TEST_CASE("MakeWithoutCopy")
{
    uint8_t data[]{uint8_t{1}, uint8_t{2}, uint8_t{3}};
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
        CopyRequire(buffer, {uint8_t{1}, uint8_t{2}, uint8_t{3}});
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
        CopyRequire(buffer, {uint8_t{49}, uint8_t{50}, uint8_t{51}});
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
        CopyRequire(buffer, {uint8_t{49}, uint8_t{50}, uint8_t{51}});
    }
}

#include "../src/include/Log.hpp"
#include <catch2/catch_session.hpp>

int main(int argc, char* argv[])
{
    // Logger is needed for Assert() in copy() and copyRange().
    PTK_INIT_LOGGING();

    return Catch::Session().run(argc, argv);
}