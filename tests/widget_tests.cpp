#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/widget.hpp"

TEST_CASE("Widget Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Widget()")
    {
        pTK::Widget t;
        pTK::Vec2<float> correct{0, 0};
        REQUIRE(t.get_size() == correct);
    }
}

TEST_CASE("Widget Setters")
{
    // Testing Setters.
    pTK::Vec2<float> correct{10, 20};
    
    SECTION("set_size(float width, float height)")
    {
        pTK::Widget t;
        t.set_size(10, 20);
        REQUIRE(t.get_size() == correct);
    }
    
    SECTION("set_size(const Vec2<float>& size)")
    {
        pTK::Widget t;
        t.set_size(correct);
        REQUIRE(t.get_size() == correct);
    }
    
    SECTION("set_position(float x, float y)")
    {
        pTK::Widget t;
        t.set_position(10, 20);
        REQUIRE(t.get_position() == correct);
    }
    
    SECTION("set_size(const Vec2<float>& size)")
    {
        pTK::Widget t;
        t.set_position(correct);
        REQUIRE(t.get_position() == correct);
    }
}

TEST_CASE("Widget Getters")
{
    // Testing Getters.
    pTK::Vec2<float> correct{10, 20};
    
    SECTION("const Vec2<float>& get_size() const")
    {
        pTK::Widget t;
        t.set_size(correct);
        REQUIRE(t.get_size().x == correct.x);
        REQUIRE(t.get_size().y == correct.y);
        REQUIRE(t.get_size() == correct);
    }
    
    SECTION("const Vec2<float>& get_position() const")
    {
        pTK::Widget t;
        t.set_position(correct);
        REQUIRE(t.get_position().x == correct.x);
        REQUIRE(t.get_position().y == correct.y);
        REQUIRE(t.get_position() == correct);
    }
}

TEST_CASE("Widget Copy and Assignment")
{
    // Testing Widget Copy and Assignment.
    pTK::Vec2<float> pos{10, 20};
    pTK::Vec2<float> size{15, 45};
    pTK::Widget t;
    t.set_size(size);
    t.set_position(pos);
    
    SECTION("Copy")
    {
        pTK::Widget tmp = t;
        REQUIRE(t.get_position() == pos);
        REQUIRE(t.get_size() == size);
    }
    
    SECTION("Assignment")
    {
        pTK::Widget tmp;
        tmp = t;
        REQUIRE(t.get_position() == pos);
        REQUIRE(t.get_size() == size);
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
    t2.set_size(42, 14);
    pTK::Widget t3;
    t3.set_position(10, 20);
    pTK::Widget t4;
    t4.set_position(10, 20);
    t4.set_size(42, 14);
    
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
