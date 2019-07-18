#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/util/Position.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    int cX = 50;
    int cY = 300;
    
    SECTION("Position()")
    {
        pTK::Position pos;
        
        REQUIRE(pos.x == 0);
        REQUIRE(pos.y == 0);
    }
    
    SECTION("Position(int width, int height)")
    {
        pTK::Position pos{cX, cY};
        
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
        pTK::Position pos;
        pos.x = cX;
        REQUIRE(pos.x == cX);
    }
    
    SECTION("y (int y)")
    {
        pTK::Position pos;
        pos.y = cY;
        REQUIRE(pos.y == cY);
    }
    
    SECTION("setPosition(int x, int y)")
    {
        pTK::Position pos;
        pos.setPosition(cX, cY);
        REQUIRE(pos.x == cX);
        REQUIRE(pos.y == cY);
    }
    
    SECTION("setPosition(const Position& pos)")
    {
        pTK::Position pos;
        pos.setPosition(cX, cY);
        REQUIRE(pos.x == cX);
        REQUIRE(pos.y == cY);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    int cX = 949;
    int cY = 84;
    
    pTK::Position pos;
    pos.setPosition(cX, cY);
    
    SECTION("Copy")
    {
        pTK::Position tmp = pos;
        REQUIRE(tmp.x == cX);
        REQUIRE(tmp.y == cY);
    }
    
    SECTION("Assignment")
    {
        pTK::Position tmp;
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
    
    pTK::Position pos;
    pos.setPosition(cX, cY);
    
    pTK::Position p1 = pos;
    
    pTK::Position p2;
    p2.y = cY;
    
    pTK::Position p3;
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



