// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/core/Widget.hpp"

/**
    All these tests are performed without a parent present.
    Parent will decide its child Size and Parent dynamically and
    if hard to test.

    More tests will be added in the future to ensure that the Widget class
    is working correctly.
 */

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.

    SECTION("Widget()")
    {
        pTK::Widget t;
        pTK::Size cSize{0, 0};
        pTK::Point cPos{0, 0};
        REQUIRE(t.getSize() == cSize);
        REQUIRE(t.getPosition() == cPos);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Setters.
    pTK::Size cSize{10, 20};
    pTK::Point cPos{55, 85};

    SECTION("setSize(const Size& size)")
    {
        pTK::Widget t;
        t.setSize(cSize);
        REQUIRE(t.getSize() == cSize);
    }

    SECTION("setPosHint(const Position& pos)")
    {
        pTK::Widget t;
        t.setPosHint(cPos);
        REQUIRE(t.getPosition() == cPos);
    }

    SECTION("Name")
    {
        pTK::Widget t;
        t.setName("testName");
        REQUIRE(t.getName() == "testName");
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Widget Copy and Assignment.

    // TODO: Add tests.
}

/**
    More tests in Comparison should be added in the future.
 */

TEST_CASE ("Comparison")
{
    // Testing Widget Comparison.
    pTK::Widget t;
    pTK::Widget t1;
    pTK::Widget t2;
    t2.setSize({42, 14});
    pTK::Widget t3;
    t3.setPosHint({10, 20});
    pTK::Widget t4;
    t4.setPosHint({10, 20});
    t4.setSize({42, 14});

    SECTION("Equal")
    {
        REQUIRE(t == t1);
        REQUIRE_FALSE(t == t2);
        REQUIRE_FALSE(t == t3);
        REQUIRE_FALSE(t == t4);
        REQUIRE_FALSE(t2 == t4);
        REQUIRE_FALSE(t3 == t4);
    }

    SECTION("Not Equal")
    {
        REQUIRE(t != t2);
        REQUIRE(t != t3);
        REQUIRE(t != t4);
        REQUIRE(t2 != t4);
        REQUIRE(t3 != t4);
        REQUIRE_FALSE(t != t1);
    }
}
