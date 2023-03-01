// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/Size.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    constexpr int width = 50;
    constexpr int height = 300;

    SECTION("Size()")
    {
        constexpr pTK::Size size{};

        REQUIRE(size.width == 0);
        REQUIRE(size.height == 0);
    }

    SECTION("Size(int width, int height)")
    {
        constexpr pTK::Size size{width, height};

        REQUIRE(size.width == width);
        REQUIRE(size.height == height);
    }

    SECTION("MakeNarrow")
    {
        constexpr unsigned int tx{1};
        constexpr float ty{1.0f};

        constexpr auto size{pTK::Size::MakeNarrow(tx, ty)};
        REQUIRE(size.width == tx);
        REQUIRE(size.height == ty);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    constexpr int width = 55;
    constexpr int height = 350;

    SECTION("set(int width, int height)")
    {
        pTK::Size size{};
        size.set(width, height);
        REQUIRE(size.width == width);
        REQUIRE(size.height == height);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    constexpr int width = 949;
    constexpr int height = 84;

    pTK::Size size{};
    size.set(width, height);

    SECTION("Copy")
    {
        pTK::Size tmp = size;
        REQUIRE(tmp.width == width);
        REQUIRE(tmp.height == height);
    }

    SECTION("Assignment")
    {
        pTK::Size tmp;
        tmp = size;
        REQUIRE(tmp.width == width);
        REQUIRE(tmp.height == height);
    }
}

TEST_CASE("Comparison")
{
    // Testing Transformable Comparison.
    constexpr int width = 57;
    constexpr int height = 384;

    pTK::Size size{};
    size.set(width, height);

    pTK::Size s1 = size;

    pTK::Size s2;
    s2.height = height;

    pTK::Size s3;
    s3.width = width;

    SECTION("Equal")
    {
        REQUIRE(size == s1);
        REQUIRE_FALSE(size == s2);
        REQUIRE_FALSE(size == s3);
    }

    SECTION("Not Equal")
    {
        REQUIRE(size != s2);
        REQUIRE(size != s3);
        REQUIRE(s2 != s3);
        REQUIRE_FALSE(size != s1);
    }
}
