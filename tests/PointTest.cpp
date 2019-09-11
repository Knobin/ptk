#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/util/Point.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    int cX = 50;
    int cY = 300;
    
    SECTION("Position()")
    {
        pTK::Point pos;
        
        REQUIRE(pos.x == 0);
        REQUIRE(pos.y == 0);
    }
    
    SECTION("Position(int width, int height)")
    {
        pTK::Point pos{cX, cY};
        
        REQUIRE(pos.x == cX);
        REQUIRE(pos.y == cY);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    int cX = 55;
    int cY = 350;
    
    SECTION("x (int x)")
    {
        pTK::Point pos;
        pos.x = cX;
        REQUIRE(pos.x == cX);
    }
    
    SECTION("y (int y)")
    {
        pTK::Point pos;
        pos.y = cY;
        REQUIRE(pos.y == cY);
    }
    
    SECTION("setPosition(int x, int y)")
    {
        pTK::Point pos;
        pos.set(cX, cY);
        REQUIRE(pos.x == cX);
        REQUIRE(pos.y == cY);
    }
    
    SECTION("setPosition(const Position& pos)")
    {
        pTK::Point pos;
        pos.set(cX, cY);
        REQUIRE(pos.x == cX);
        REQUIRE(pos.y == cY);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    int cX = 949;
    int cY = 84;
    
    pTK::Point pos;
    pos.set(cX, cY);
    
    SECTION("Copy")
    {
        pTK::Point tmp = pos;
        REQUIRE(tmp.x == cX);
        REQUIRE(tmp.y == cY);
    }
    
    SECTION("Assignment")
    {
        pTK::Point tmp;
        tmp = pos;
        REQUIRE(tmp.x == cX);
        REQUIRE(tmp.y == cY);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    int cX = 57;
    int cY = 384;
    
    pTK::Point pos;
    pos.set(cX, cY);
    
    pTK::Point p1 = pos;
    
    pTK::Point p2;
    p2.y = cY;
    
    pTK::Point p3;
    p3.x = cX;
    
    SECTION("Equal")
    {
        REQUIRE(pos == p1);
        REQUIRE_FALSE(pos == p2);
        REQUIRE_FALSE(pos == p3);
    }
    
    SECTION("Not Equal")
    {
        REQUIRE(pos != p2);
        REQUIRE(pos != p3);
        REQUIRE(p2 != p3);
        REQUIRE_FALSE(pos != p1);
    }
}



