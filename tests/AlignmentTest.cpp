// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/core/Alignment.hpp"

using align_utype = std::underlying_type<pTK::Align>::type;

static constexpr align_utype operator|(pTK::Align a, pTK::Align b) noexcept
{
    return static_cast<align_utype>(a) | static_cast<align_utype>(b);
}

TEST_CASE("IsAlignSet")
{
    constexpr align_utype align{ pTK::Align::Top | pTK::Align::Left };

    SECTION("true")
    {
        REQUIRE(pTK::IsAlignSet(align, pTK::Align::Top));
        REQUIRE(pTK::IsAlignSet(align, pTK::Align::Left));
        REQUIRE(pTK::IsAlignSet(align, pTK::Align::Top, pTK::Align::Left));
        REQUIRE(pTK::IsAlignSet(align, pTK::Align::Left, pTK::Align::Top));
    }
    
    SECTION("false")
    {
        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::Bottom));
        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::Right));

        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::HCenter));
        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::VCenter));
        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::Center));

        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::Top, pTK::Align::Bottom));
        REQUIRE_FALSE(pTK::IsAlignSet(align, pTK::Align::Bottom, pTK::Align::Top));
    }
}

TEST_CASE("Alignment()")
{
    pTK::Alignment alignment{};

    REQUIRE(alignment.getAlign() == static_cast<align_utype>(pTK::Align::Center));
    REQUIRE(pTK::IsAlignSet(alignment.getAlign(), pTK::Align::Center));
}

template<typename... Aligns>
static bool CheckAlign(const pTK::Alignment& align, Aligns&&... aligns) noexcept
{
    return pTK::IsAlignSet(align.getAlign(), std::forward<Aligns>(aligns)...);
}

TEST_CASE("Alignment::setAlign")
{
    SECTION("single arg")
    {
        pTK::Alignment alignment{};

        alignment.setAlign(pTK::Align::Bottom);
        REQUIRE(CheckAlign(alignment, pTK::Align::Bottom));

        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Top));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Left));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Right));

        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::HCenter));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::VCenter));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Center));

        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Center, pTK::Align::Bottom));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Bottom, pTK::Align::Center));
    }

    SECTION("double arg")
    {
        pTK::Alignment alignment{};

        alignment.setAlign(pTK::Align::Top, pTK::Align::Left);
        REQUIRE(CheckAlign(alignment, pTK::Align::Top));
        REQUIRE(CheckAlign(alignment, pTK::Align::Left));
        REQUIRE(CheckAlign(alignment, pTK::Align::Top, pTK::Align::Left));
        REQUIRE(CheckAlign(alignment, pTK::Align::Left, pTK::Align::Top));

        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Bottom));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Right));

        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::HCenter));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::VCenter));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Center));

        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Top, pTK::Align::Bottom));
        REQUIRE_FALSE(CheckAlign(alignment, pTK::Align::Bottom, pTK::Align::Top));
    }
}
