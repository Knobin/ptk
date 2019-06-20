#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/shape.hpp"

TEST_CASE("Shape Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Shape()")
    {
        pTK::Shape s;
        pTK::Color correct_colors{0, 0, 0, 255};
        REQUIRE(s.get_color() == correct_colors);
        REQUIRE(s.get_outline_color() == correct_colors);
        REQUIRE(s.get_outline_thickness() == 0);
    }
}

TEST_CASE("Shape Getters and Setters")
{
    // Testing Getters and Setters.
    pTK::Color correct_colors{50, 100, 150, 200};
    
    SECTION("color")
    {
        pTK::Shape s;
        s.set_color(correct_colors);
        REQUIRE(s.get_color() == correct_colors);
    }
    
    SECTION("outline_color")
    {
        pTK::Shape s;
        s.set_outline_color(correct_colors);
        REQUIRE(s.get_outline_color() == correct_colors);
    }
    
    SECTION("outline_thickness")
    {
        pTK::Shape s;
        s.set_outline_thickness(10);
        REQUIRE(s.get_outline_thickness() == 10);
    }
}

TEST_CASE("Shape Copy and Assignment")
{
    // Testing Shape Copy and Assignment.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outline_color{75, 125, 175, 225};
    uint32_t    outline_thickness = 50;
    
    pTK::Shape s;
    s.set_color(color);
    s.set_outline_color(outline_color);
    s.set_outline_thickness(outline_thickness);
    
    SECTION("Copy")
    {
        pTK::Shape tmp = s;
        REQUIRE(tmp.get_color() == color);
        REQUIRE(tmp.get_outline_color() == outline_color);
        REQUIRE(tmp.get_outline_thickness() == outline_thickness);
    }
    
    SECTION("Assignment")
    {
        pTK::Shape tmp;
        tmp = s;
        REQUIRE(tmp.get_color() == color);
        REQUIRE(tmp.get_outline_color() == outline_color);
        REQUIRE(tmp.get_outline_thickness() == outline_thickness);
    }
}

TEST_CASE ("Shape Comparison")
{
    // Testing Transformable Comparison.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outline_color{75, 125, 175, 225};
    uint32_t    outline_thickness = 50;
    
    pTK::Shape s;
    s.set_color(color);
    s.set_outline_color(outline_color);
    s.set_outline_thickness(outline_thickness);
    pTK::Shape s1 = s;
    
    pTK::Shape s2;
    s2.set_outline_color(color);
    
    pTK::Shape s3;
    s3.set_color(outline_color);
    
    SECTION("Equal")
    {
        REQUIRE(s == s1);
        REQUIRE_FALSE(s == s2);
        REQUIRE_FALSE(s == s3);
        REQUIRE_FALSE(s2 == s3);
    }
    
    SECTION("Not Equal")
    {
        REQUIRE(s != s2);
        REQUIRE(s != s3);
        REQUIRE(s2 != s3);
        REQUIRE_FALSE(s != s1);
    }
}
