// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
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

TEST_CASE("Comparison")
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

TEST_CASE("Predefined Types")
{
    SECTION("Fixed")
    {
        pTK::SizePolicy fixed{pTK::SizePolicy::Type::Fixed};
        REQUIRE(fixed.horizontal == pTK::SizePolicy::Policy::Fixed);
        REQUIRE(fixed.vertical == pTK::SizePolicy::Policy::Fixed);
    }

    SECTION("Expanding")
    {
        pTK::SizePolicy expanding{pTK::SizePolicy::Type::Expanding};
        REQUIRE(expanding.horizontal == pTK::SizePolicy::Policy::Expanding);
        REQUIRE(expanding.vertical == pTK::SizePolicy::Policy::Expanding);
    }
}

TEST_CASE("IsSizePolicyFlagSet")
{
    using pTK::SizePolicy;
    using policy_utype = std::underlying_type<SizePolicy::Policy>::type;

    SECTION("Fixed")
    {
        SizePolicy::Policy fixed{SizePolicy::Policy::Fixed};

        // (SizePolicy::Policy policy, Flags&&... flags)
        REQUIRE(pTK::IsSizePolicyFlagSet(fixed, SizePolicy::PolicyFlag::Fixed));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(fixed, SizePolicy::PolicyFlag::Fixed, SizePolicy::PolicyFlag::Grow));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(fixed, SizePolicy::PolicyFlag::Grow));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(fixed, SizePolicy::PolicyFlag::Shrink));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(fixed, SizePolicy::PolicyFlag::Grow, SizePolicy::PolicyFlag::Shrink));

        // (std::underlying_type<SizePolicy::Policy>::type policy, Flags&&... flags)
        policy_utype f = static_cast<policy_utype>(fixed);
        REQUIRE(pTK::IsSizePolicyFlagSet(f, SizePolicy::PolicyFlag::Fixed));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(f, SizePolicy::PolicyFlag::Fixed, SizePolicy::PolicyFlag::Grow));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(f, SizePolicy::PolicyFlag::Grow));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(f, SizePolicy::PolicyFlag::Shrink));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(f, SizePolicy::PolicyFlag::Grow, SizePolicy::PolicyFlag::Shrink));
    }

    SECTION("Expanding")
    {
        SizePolicy::Policy expanding{SizePolicy::Policy::Expanding};

        // (SizePolicy::Policy policy, Flags&&... flags)
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(expanding, SizePolicy::PolicyFlag::Fixed));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(expanding, SizePolicy::PolicyFlag::Fixed, SizePolicy::PolicyFlag::Grow));
        REQUIRE(pTK::IsSizePolicyFlagSet(expanding, SizePolicy::PolicyFlag::Grow));
        REQUIRE(pTK::IsSizePolicyFlagSet(expanding, SizePolicy::PolicyFlag::Shrink));
        REQUIRE(pTK::IsSizePolicyFlagSet(expanding, SizePolicy::PolicyFlag::Grow, SizePolicy::PolicyFlag::Shrink));

        // (std::underlying_type<SizePolicy::Policy>::type policy, Flags&&... flags)
        policy_utype e = static_cast<policy_utype>(expanding);
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(e, SizePolicy::PolicyFlag::Fixed));
        REQUIRE_FALSE(pTK::IsSizePolicyFlagSet(e, SizePolicy::PolicyFlag::Fixed, SizePolicy::PolicyFlag::Grow));
        REQUIRE(pTK::IsSizePolicyFlagSet(e, SizePolicy::PolicyFlag::Grow));
        REQUIRE(pTK::IsSizePolicyFlagSet(e, SizePolicy::PolicyFlag::Shrink));
        REQUIRE(pTK::IsSizePolicyFlagSet(e, SizePolicy::PolicyFlag::Grow, SizePolicy::PolicyFlag::Shrink));
    }
}
