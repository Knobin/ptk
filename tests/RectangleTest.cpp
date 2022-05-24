// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/widgets/Rectangle.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Rectangle()")
    {
        pTK::Rectangle rect;
        
        REQUIRE(rect.getColor() == pTK::Color(0xf5f5f5ff));
        REQUIRE(rect.getOutlineColor() == pTK::Color(0xf5f5f5ff));
        REQUIRE(rect.getOutlineThickness() == 0.0f);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    pTK::Color correct_colors{50, 100, 150, 200};
    
    SECTION("color")
    {
        pTK::Rectangle rect;
        rect.setColor(correct_colors);
        REQUIRE(rect.getColor() == correct_colors);
    }
    
    SECTION("outlineColor")
    {
        pTK::Rectangle rect;
        rect.setOutlineColor(correct_colors);
        REQUIRE(rect.getOutlineColor() == correct_colors);
    }
    
    SECTION("outlineThickness")
    {
        pTK::Rectangle rect;
        rect.setOutlineThickness(10.0f);
        REQUIRE(rect.getOutlineThickness() == 10.0f);
    }
    
    SECTION("cornerRadius")
    {
        pTK::Rectangle rect;
        rect.setCornerRadius(10.0f);
        REQUIRE(rect.getCornerRadius() == 10.0f);
        
        rect.setCornerRadius(-10.0f);
        REQUIRE(rect.getCornerRadius() == 10.0f);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outlineColor{75, 125, 175, 225};
    float       outlineThickness = 50.0f;
    float       cornerRadius = 10.0f;
    
    pTK::Rectangle rect;
    rect.setColor(color);
    rect.setOutlineColor(outlineColor);
    rect.setOutlineThickness(outlineThickness);
    rect.setCornerRadius(cornerRadius);
    
    SECTION("Copy")
    {
        pTK::Rectangle tmp = rect;
        REQUIRE(tmp.getColor() == color);
        REQUIRE(tmp.getOutlineColor() == outlineColor);
        REQUIRE(tmp.getOutlineThickness() == outlineThickness);
        REQUIRE(tmp.getCornerRadius() == cornerRadius);
    }
    
    SECTION("Assignment")
    {
        pTK::Rectangle tmp;
        tmp = rect;
        REQUIRE(tmp.getColor() == color);
        REQUIRE(tmp.getOutlineColor() == outlineColor);
        REQUIRE(tmp.getOutlineThickness() == outlineThickness);
        REQUIRE(tmp.getCornerRadius() == cornerRadius);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    pTK::Color  color{50, 100, 150, 200};
    pTK::Color  outlineColor{75, 125, 175, 225};
    float       outlineThickness = 50.0f;
    float       cornerRadius = 10.0f;
    
    pTK::Rectangle rect;
    rect.setColor(color);
    rect.setOutlineColor(outlineColor);
    rect.setOutlineThickness(outlineThickness);
    rect.setCornerRadius(cornerRadius);
    pTK::Rectangle r1 = rect;
    
    pTK::Rectangle r2;
    r2.setOutlineColor(color);
    
    pTK::Rectangle r3;
    r3.setColor(outlineColor);
    
    pTK::Rectangle r4;
    r4.setCornerRadius(cornerRadius);
    
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

