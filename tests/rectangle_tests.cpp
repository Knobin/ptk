#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/widgets/rectangle.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Rectangle()")
    {
        pTK::Rectangle rect;
        
        REQUIRE(rect.get_color() == pTK::Color(0, 0, 0));
        REQUIRE(rect.get_outline_color() == pTK::Color(0, 0, 0));
        REQUIRE(rect.get_outline_thickness() == 0);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    pTK::Color correct_colors{50, 100, 150, 200};
    
    SECTION("color")
    {
        pTK::Rectangle rect;
        rect.set_color(correct_colors);
        REQUIRE(rect.get_color() == correct_colors);
    }
    
    SECTION("outline_color")
    {
        pTK::Rectangle rect;
        rect.set_outline_color(correct_colors);
        REQUIRE(rect.get_outline_color() == correct_colors);
    }
    
    SECTION("outline_thickness")
    {
        pTK::Rectangle rect;
        rect.set_outline_thickness(10);
        REQUIRE(rect.get_outline_thickness() == 10);
    }
    
    SECTION("corner_radius")
    {
        pTK::Rectangle rect;
        rect.set_corner_radius(10);
        REQUIRE(rect.get_corner_radius() == 10);
        
        rect.set_corner_radius(-10);
        REQUIRE(rect.get_corner_radius() == 10);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outline_color{75, 125, 175, 225};
    float       outline_thickness = 50;
    float       corner_radius = 10;
    
    pTK::Rectangle rect;
    rect.set_color(color);
    rect.set_outline_color(outline_color);
    rect.set_outline_thickness(outline_thickness);
    rect.set_corner_radius(corner_radius);
    
    SECTION("Copy")
    {
        pTK::Rectangle tmp = rect;
        REQUIRE(tmp.get_color() == color);
        REQUIRE(tmp.get_outline_color() == outline_color);
        REQUIRE(tmp.get_outline_thickness() == outline_thickness);
        REQUIRE(tmp.get_corner_radius() == corner_radius);
    }
    
    SECTION("Assignment")
    {
        pTK::Rectangle tmp;
        tmp = rect;
        REQUIRE(tmp.get_color() == color);
        REQUIRE(tmp.get_outline_color() == outline_color);
        REQUIRE(tmp.get_outline_thickness() == outline_thickness);
        REQUIRE(tmp.get_corner_radius() == corner_radius);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outline_color{75, 125, 175, 225};
    float       outline_thickness = 50;
    float       corner_radius = 10;
    
    pTK::Rectangle rect;
    rect.set_color(color);
    rect.set_outline_color(outline_color);
    rect.set_outline_thickness(outline_thickness);
    rect.set_corner_radius(corner_radius);
    pTK::Rectangle r1 = rect;
    
    pTK::Rectangle r2;
    r2.set_outline_color(color);
    
    pTK::Rectangle r3;
    r3.set_color(outline_color);
    
    pTK::Rectangle r4;
    r4.set_corner_radius(corner_radius);
    
    SECTION("Equal")
    {
        REQUIRE(rect == r1);
        REQUIRE_FALSE(rect == r2);
        REQUIRE_FALSE(rect == r3);
        REQUIRE_FALSE(r2 == r3);
        REQUIRE_FALSE(rect == r4);
    }
    
    SECTION("Not Equal")
    {
        REQUIRE(rect != r2);
        REQUIRE(rect != r3);
        REQUIRE(r2 != r3);
        REQUIRE(rect != r4);
        REQUIRE_FALSE(rect != r1);
    }
}

