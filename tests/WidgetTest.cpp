#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/Widget.hpp"

TEST_CASE("Widget Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Widget()")
    {
        pTK::Widget t;
        pTK::Vec2<float> correct{0, 0};
        REQUIRE(t.getSize() == correct);
    }
}

TEST_CASE("Widget Setters")
{
    // Testing Setters.
    pTK::Vec2<float> correct{10, 20};
    
    SECTION("setSizeRequest(float width, float height)")
    {
        pTK::Widget t;
        t.setSizeRequest(10, 20);
        REQUIRE(t.getSize() == correct);
    }
    
    SECTION("setSizeRequest(const Vec2<float>& size)")
    {
        pTK::Widget t;
        t.setSizeRequest(correct);
        REQUIRE(t.getSize() == correct);
    }
    
    SECTION("setPositionRequest(float x, float y)")
    {
        pTK::Widget t;
        t.setPositionRequest(10, 20);
        REQUIRE(t.getPosition() == correct);
    }
    
    SECTION("setSizeRequest(const Vec2<float>& size)")
    {
        pTK::Widget t;
        t.setPositionRequest(correct);
        REQUIRE(t.getPosition() == correct);
    }
}

TEST_CASE("Widget Getters")
{
    // Testing Getters.
    pTK::Vec2<float> correct{10, 20};
    
    SECTION("const Vec2<float>& getSize() const")
    {
        pTK::Widget t;
        t.setSizeRequest(correct);
        REQUIRE(t.getSize().x == correct.x);
        REQUIRE(t.getSize().y == correct.y);
        REQUIRE(t.getSize() == correct);
    }
    
    SECTION("const Vec2<float>& getPosition() const")
    {
        pTK::Widget t;
        t.setPositionRequest(correct);
        REQUIRE(t.getPosition().x == correct.x);
        REQUIRE(t.getPosition().y == correct.y);
        REQUIRE(t.getPosition() == correct);
    }
}

TEST_CASE("Widget Copy and Assignment")
{
    // Testing Widget Copy and Assignment.
    pTK::Vec2<float> pos{10, 20};
    pTK::Vec2<float> size{15, 45};
    pTK::Widget t;
    t.setSizeRequest(size);
    t.setPositionRequest(pos);
    
    SECTION("Copy")
    {
        pTK::Widget tmp = t;
        REQUIRE(t.getPosition() == pos);
        REQUIRE(t.getSize() == size);
    }
    
    SECTION("Assignment")
    {
        pTK::Widget tmp;
        tmp = t;
        REQUIRE(t.getPosition() == pos);
        REQUIRE(t.getSize() == size);
    }
    
    SECTION("Check if pos and size has changed")
    {
        REQUIRE(pos.x == 10);
        REQUIRE(pos.y == 20);
        
        REQUIRE(size.x == 15);
        REQUIRE(size.y == 45);
    }
}

TEST_CASE ("Widget Comparison")
{
    // Testing Widget Comparison.
    pTK::Widget t;
    pTK::Widget t1;
    pTK::Widget t2;
    t2.setSizeRequest(42, 14);
    pTK::Widget t3;
    t3.setPositionRequest(10, 20);
    pTK::Widget t4;
    t4.setPositionRequest(10, 20);
    t4.setSizeRequest(42, 14);
    
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
