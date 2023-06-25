//
//  tests/ColorTypeTest.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-24.
//

// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/ColorType.hpp"

TEST_CASE("ColorType")
{
    // Just a test to see if they exist and if the order changed (or index of the enum).

    using ct_utype = std::underlying_type<pTK::ColorType>::type;
    ct_utype index{};

    REQUIRE(static_cast<ct_utype>(pTK::ColorType::Unknown) == index++);
    REQUIRE(static_cast<ct_utype>(pTK::ColorType::RGBA_8888) == index++);
    REQUIRE(static_cast<ct_utype>(pTK::ColorType::RGB_888x) == index++);
    REQUIRE(static_cast<ct_utype>(pTK::ColorType::BGRA_8888) == index++);
}

TEST_CASE("ColorTypeInfo")
{
    SECTION("ColorType::Unknown")
    {
        pTK::ColorTypeInfo info{pTK::GetColorTypeInfo(pTK::ColorType::Unknown)};

        REQUIRE(info.channelIndex.a == 0);
        REQUIRE(info.channelIndex.r == 0);
        REQUIRE(info.channelIndex.g == 0);
        REQUIRE(info.channelIndex.b == 0);

        REQUIRE(info.channelBits.a == 0);
        REQUIRE(info.channelBits.r == 0);
        REQUIRE(info.channelBits.g == 0);
        REQUIRE(info.channelBits.b == 0);

        REQUIRE(info.totalBits == 0);
    }

    SECTION("ColorType::RGBA_8888")
    {
        pTK::ColorTypeInfo info{pTK::GetColorTypeInfo(pTK::ColorType::RGBA_8888)};

        REQUIRE(info.channelIndex.a == 3);
        REQUIRE(info.channelIndex.r == 0);
        REQUIRE(info.channelIndex.g == 1);
        REQUIRE(info.channelIndex.b == 2);

        REQUIRE(info.channelBits.a == 8);
        REQUIRE(info.channelBits.r == 8);
        REQUIRE(info.channelBits.g == 8);
        REQUIRE(info.channelBits.b == 8);

        REQUIRE(info.totalBits == 32);
    }

    SECTION("ColorType::RGB_888x")
    {
        pTK::ColorTypeInfo info{pTK::GetColorTypeInfo(pTK::ColorType::RGB_888x)};

        REQUIRE(info.channelIndex.a == 3);
        REQUIRE(info.channelIndex.r == 0);
        REQUIRE(info.channelIndex.g == 1);
        REQUIRE(info.channelIndex.b == 2);

        REQUIRE(info.channelBits.a == 0);
        REQUIRE(info.channelBits.r == 8);
        REQUIRE(info.channelBits.g == 8);
        REQUIRE(info.channelBits.b == 8);

        REQUIRE(info.totalBits == 32);
    }

    SECTION("ColorType::BGRA_8888")
    {
        pTK::ColorTypeInfo info{pTK::GetColorTypeInfo(pTK::ColorType::BGRA_8888)};

        REQUIRE(info.channelIndex.a == 3);
        REQUIRE(info.channelIndex.r == 2);
        REQUIRE(info.channelIndex.g == 1);
        REQUIRE(info.channelIndex.b == 0);

        REQUIRE(info.channelBits.a == 8);
        REQUIRE(info.channelBits.r == 8);
        REQUIRE(info.channelBits.g == 8);
        REQUIRE(info.channelBits.b == 8);

        REQUIRE(info.totalBits == 32);
    }
}
