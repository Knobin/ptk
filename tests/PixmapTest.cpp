//
//  tests/PixmapTest.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-25.
//

// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/ColorType.hpp"
#include "ptk/util/Pixmap.hpp"
#include <cstddef>

///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Custom operator new[] to test for when new operator fails (std::bad_alloc).
//
// Note: Pixmap will not throw if allocation failed.
//

static bool EnableAllocation{true};
void* operator new[](size_t size, const std::nothrow_t&) noexcept
{
    return (EnableAllocation) ? malloc(size) : nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Constructors")
{
    SECTION("Pixmap()")
    {
        pTK::Pixmap pixmap{};
        REQUIRE_FALSE(pixmap.isValid());
        REQUIRE(pixmap.size() == pTK::Size{});
        REQUIRE(pixmap.width() == 0);
        REQUIRE(pixmap.height() == 0);
        REQUIRE(pixmap.length() == 0);
        REQUIRE(pixmap.bytesPerPixel() == 0);
        REQUIRE(pixmap.colorType() == pTK::ColorType::Unknown);
        REQUIRE(pixmap.bytes() == nullptr);
        REQUIRE(pixmap.data() == nullptr);
    }

    SECTION("Pixmap(width, height, colorType)")
    {
        constexpr uint32_t width{96};
        constexpr uint32_t height{54};
        constexpr pTK::ColorType type{pTK::ColorType::RGBA_8888};

        pTK::Pixmap pixmap{width, height, type};
        REQUIRE(pixmap.isValid());
        REQUIRE(pixmap.size() == pTK::Size::MakeNarrow(width, height));
        REQUIRE(pixmap.width() == width);
        REQUIRE(pixmap.height() == height);
        REQUIRE(pixmap.length() == static_cast<uint32_t>(width * height * 4));
        REQUIRE(pixmap.bytesPerPixel() == 4);
        REQUIRE(pixmap.colorType() == type);
        REQUIRE(pixmap.bytes());
        REQUIRE(pixmap.data());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static void RequirePixmapIsValidTest(bool allShouldWork = true)
{
    constexpr uint32_t width{96};
    constexpr uint32_t height{54};
    constexpr pTK::ColorType type{pTK::ColorType::RGBA_8888};

    // All other valid.
    pTK::Pixmap all{width, height, type};
    if (allShouldWork)
        REQUIRE(all.isValid());
    else
        REQUIRE_FALSE(all.isValid());

    // Width non valid.
    pTK::Pixmap w{0, height, type};
    REQUIRE_FALSE(w.isValid());

    // Height non valid.
    pTK::Pixmap h{width, 0, type};
    REQUIRE_FALSE(h.isValid());

    // Color type non valid.
    pTK::Pixmap c{width, height, pTK::ColorType::Unknown};
    REQUIRE_FALSE(c.isValid());

    // Width and Height non valid.
    pTK::Pixmap wh{0, 0, type};
    REQUIRE_FALSE(wh.isValid());

    // Width and Color type non valid.
    pTK::Pixmap wc{0, height, pTK::ColorType::Unknown};
    REQUIRE_FALSE(wc.isValid());

    // Height and Color type non valid.
    pTK::Pixmap hc{width, 0, pTK::ColorType::Unknown};
    REQUIRE_FALSE(hc.isValid());

    // Height and Color type non valid.
    pTK::Pixmap whc{0, 0, pTK::ColorType::Unknown};
    REQUIRE_FALSE(whc.isValid());
}

TEST_CASE("isValid()")
{
    RequirePixmapIsValidTest();

    SECTION("new operator fail")
    {
        EnableAllocation = false;
        RequirePixmapIsValidTest(false);
        EnableAllocation = true;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("colorType()")
{
    pTK::Pixmap pixmap{96, 54, pTK::ColorType::RGBA_8888};
    REQUIRE(pixmap.colorType() == pTK::ColorType::RGBA_8888);

    pixmap.setColorType(pTK::ColorType::RGB_888x);
    REQUIRE(pixmap.colorType() == pTK::ColorType::RGB_888x);

    pixmap.setColorType(pTK::ColorType::Unknown);
    REQUIRE(pixmap.colorType() == pTK::ColorType::Unknown);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("at")
{
    pTK::Pixmap pixmap{96, 54, pTK::ColorType::RGBA_8888};
    const uint8_t* bytes{pixmap.bytes()};
    const auto bpp{static_cast<std::ptrdiff_t>(pixmap.bytesPerPixel())};

    REQUIRE(bytes == pixmap.at(0, 0));

    REQUIRE(bytes + (5 * bpp) == pixmap.at(5, 0));
    REQUIRE(bytes + (96 * bpp) == pixmap.at(0, 1));

    REQUIRE(bytes + ((5 + 96) * bpp) == pixmap.at(5, 1));
    REQUIRE(bytes + ((96 * 2) * bpp) == pixmap.at(0, 2));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("row")
{
    pTK::Pixmap pixmap{96, 54, pTK::ColorType::RGBA_8888};
    const uint8_t* bytes{pixmap.bytes()};
    const auto bpp{static_cast<std::ptrdiff_t>(pixmap.bytesPerPixel())};

    REQUIRE(bytes == pixmap.row(0));

    for (std::ptrdiff_t i{0}; i < static_cast<std::ptrdiff_t>(pixmap.height()); ++i)
    {
        REQUIRE(bytes + (96 * bpp * i) == pixmap.row(i));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("copy")
{
    pTK::Pixmap pixmap{16, 4, pTK::ColorType::RGBA_8888};

    for (std::ptrdiff_t i{0}; i < 256; ++i)
        pixmap.bytes()[i] = static_cast<uint8_t>(i);

    uint8_t buffer[256]{};
    REQUIRE(pixmap.copy(buffer) == 256);

    for (std::ptrdiff_t i{0}; i < 256; ++i)
    {
        REQUIRE(buffer[i] == pixmap.bytes()[i]);
    }
}

TEST_CASE("copyRange")
{
    pTK::Pixmap pixmap{16, 4, pTK::ColorType::RGBA_8888};

    for (std::ptrdiff_t i{0}; i < 256; ++i)
        pixmap.bytes()[i] = static_cast<uint8_t>(i);

    constexpr std::size_t count{80};
    uint8_t buffer[count]{};
    constexpr std::ptrdiff_t offset{120};

    REQUIRE(pixmap.copyRange(offset, count, buffer) == count);

    for (std::ptrdiff_t i{0}; i < count; ++i)
    {
        REQUIRE(buffer[i] == pixmap.bytes()[offset + i]);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../src/include/Log.hpp"
#include <catch2/catch_session.hpp>

int main(int argc, char* argv[])
{
    // Logger is needed for Assert() in copy() and copyRange().
    PTK_INIT_LOGGING();

    return Catch::Session().run(argc, argv);
}