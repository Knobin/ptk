#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/util/SizePolicy.hpp"

TEST_CASE("Constructors")
{
    SECTION("SizePolicy{}")
    {
        pTK::SizePolicy policy{};
        REQUIRE(policy.horizontal == pTK::SizePolicy::Policy::Fixed);
        REQUIRE(policy.vertical == pTK::SizePolicy::Policy::Fixed);
    }

    SECTION("SizePolicy{Policy, Policy}")
    {
        pTK::SizePolicy p1{pTK::SizePolicy::Policy::Expanding, pTK::SizePolicy::Policy::Expanding};
        REQUIRE(p1.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(p1.vertical == pTK::SizePolicy::Policy::Expanding);

        pTK::SizePolicy p2{pTK::SizePolicy::Policy::Fixed, pTK::SizePolicy::Policy::Expanding};
        REQUIRE(p2.horizontal == pTK::SizePolicy::Policy::Fixed);
        REQUIRE(p2.vertical == pTK::SizePolicy::Policy::Expanding);

        pTK::SizePolicy p3{pTK::SizePolicy::Policy::Expanding, pTK::SizePolicy::Policy::Fixed};
        REQUIRE(p3.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(p3.vertical == pTK::SizePolicy::Policy::Fixed);
    }
}

TEST_CASE("Copy and Assignment")
{
    pTK::SizePolicy policy{pTK::SizePolicy::Policy::Expanding, pTK::SizePolicy::Policy::Fixed};

    SECTION("Copy")
    {
        pTK::SizePolicy tmp = policy;

        REQUIRE(policy.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(policy.vertical == pTK::SizePolicy::Policy::Fixed);

        REQUIRE(tmp.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(tmp.vertical == pTK::SizePolicy::Policy::Fixed);
    }

    SECTION("Assignment")
    {
        pTK::SizePolicy tmp;
        tmp = policy;

        REQUIRE(policy.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(policy.vertical == pTK::SizePolicy::Policy::Fixed);

        REQUIRE(tmp.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(tmp.vertical == pTK::SizePolicy::Policy::Fixed);
    }
}

TEST_CASE ("Comparison")
{
    pTK::SizePolicy p1{pTK::SizePolicy::Policy::Expanding, pTK::SizePolicy::Policy::Fixed};
    pTK::SizePolicy p2{pTK::SizePolicy::Policy::Fixed, pTK::SizePolicy::Policy::Expanding};

    SECTION("Equal")
    {
        REQUIRE(p1 == p1);
        REQUIRE(p2 == p2);
        REQUIRE_FALSE(p1 == p2);
        REQUIRE_FALSE(p2 == p1);
    }

    SECTION("Not Equal")
    {
        REQUIRE(p1 != p2);
        REQUIRE(p2 != p1);
        REQUIRE_FALSE(p1 != p1);
        REQUIRE_FALSE(p2 != p2);
    }
}
