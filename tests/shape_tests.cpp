#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/shape.hpp"

TEST_CASE("Shape Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Shape()")
    {
        pTK::Shape s;
        pTK::Color correctColors{0, 0, 0, 255};
        REQUIRE(s.getColor() == correctColors);
        REQUIRE(s.getOutlineColor() == correctColors);
        REQUIRE(s.getOutlineThickness() == 0);
    }
}

TEST_CASE("Shape Getters and Setters")
{
    // Testing Getters and Setters.
    pTK::Color correctColors{50, 100, 150, 200};
    
    SECTION("color")
    {
        pTK::Shape s;
        s.setColor(correctColors);
        REQUIRE(s.getColor() == correctColors);
    }
    
    SECTION("outline_color")
    {
        pTK::Shape s;
        s.setOutlineColor(correctColors);
        REQUIRE(s.getOutlineColor() == correctColors);
    }
    
    SECTION("outline_thickness")
    {
        pTK::Shape s;
        s.setOutlineThickness(10);
        REQUIRE(s.getOutlineThickness() == 10);
    }
}

TEST_CASE("Shape Copy and Assignment")
{
    // Testing Shape Copy and Assignment.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outline_color{75, 125, 175, 225};
    uint32_t    outline_thickness = 50;
    
    pTK::Shape s;
    s.setColor(color);
    s.setOutlineColor(outline_color);
    s.setOutlineThickness(outline_thickness);
    
    SECTION("Copy")
    {
        pTK::Shape tmp = s;
        REQUIRE(tmp.getColor() == color);
        REQUIRE(tmp.getOutlineColor() == outline_color);
        REQUIRE(tmp.getOutlineThickness() == outline_thickness);
    }
    
    SECTION("Assignment")
    {
        pTK::Shape tmp;
        tmp = s;
        REQUIRE(tmp.getColor() == color);
        REQUIRE(tmp.getOutlineColor() == outline_color);
        REQUIRE(tmp.getOutlineThickness() == outline_thickness);
    }
}

TEST_CASE ("Shape Comparison")
{
    // Testing Transformable Comparison.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outline_color{75, 125, 175, 225};
    uint32_t    outline_thickness = 50;
    
    pTK::Shape s;
    s.setColor(color);
    s.setOutlineColor(outline_color);
    s.setOutlineThickness(outline_thickness);
    pTK::Shape s1 = s;
    
    pTK::Shape s2;
    s2.setOutlineColor(color);
    
    pTK::Shape s3;
    s3.setColor(outline_color);
    
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
